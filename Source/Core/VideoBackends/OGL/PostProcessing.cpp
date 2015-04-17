// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#include "Common/CommonPaths.h"
#include "Common/FileUtil.h"
#include "Common/StringUtil.h"

#include "VideoBackends/OGL/FramebufferManager.h"
#include "VideoBackends/OGL/GLUtil.h"
#include "VideoBackends/OGL/PostProcessing.h"
#include "VideoBackends/OGL/ProgramShaderCache.h"

#include "VideoCommon/DriverDetails.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VideoConfig.h"

namespace OGL
{

static const char s_vertex_workaround_shader[] =
	"in vec4 rawpos;\n"
	"out vec2 uv0;\n"
	"uniform vec4 src_rect;\n"
	"void main(void) {\n"
	"	gl_Position = vec4(rawpos.xy, 0.0, 1.0);\n"
	"	uv0 = rawpos.zw * src_rect.zw + src_rect.xy;\n"
	"}\n";

static const char s_vertex_shader[] =
	"out vec2 uv0;\n"
	"uniform vec4 src_rect;\n"
	"void main(void) {\n"
	"	vec2 rawpos = vec2(gl_VertexID&1, gl_VertexID&2);\n"
	"	gl_Position = vec4(rawpos*2.0-1.0, 0.0, 1.0);\n"
	"	uv0 = rawpos * src_rect.zw + src_rect.xy;\n"
	"}\n";

OpenGLPostProcessing::OpenGLPostProcessing()
	: m_initialized(false)
{
	CreateHeader();

	m_attribute_workaround = DriverDetails::HasBug(DriverDetails::BUG_BROKENATTRIBUTELESS);
	if (m_attribute_workaround)
	{
		glGenBuffers(1, &m_attribute_vbo);
		glGenVertexArrays(1, &m_attribute_vao);
	}
}

OpenGLPostProcessing::~OpenGLPostProcessing()
{
	m_shader.Destroy();

	if (m_attribute_workaround)
	{
		glDeleteBuffers(1, &m_attribute_vbo);
		glDeleteVertexArrays(1, &m_attribute_vao);
	}
}

void OpenGLPostProcessing::BlitFromTexture(const TargetRectangle &src, const TargetRectangle &dst,
	void* src_texture_ptr, void* src_depth_texture_ptr, int src_width, int src_height, int layer, float gamma)
{
	int src_texture = *((int*)src_texture_ptr);
	int src_texture_depth = *((int*)src_depth_texture_ptr);
	ApplyShader();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glViewport(dst.left, dst.bottom, dst.GetWidth(), dst.GetHeight());

	if (m_attribute_workaround)
		glBindVertexArray(m_attribute_vao);
	else
		OpenGL_BindAttributelessVAO();

	m_shader.Bind();

	glUniform4f(m_uniform_resolution, (float)src_width, (float)src_height, 1.0f / (float)src_width, 1.0f / (float)src_height);
	glUniform4f(m_uniform_src_rect, src.left / (float) src_width, src.bottom / (float) src_height,
		    src.GetWidth() / (float) src_width, src.GetHeight() / (float) src_height);
	glUniform1ui(m_uniform_time, (GLuint)m_timer.GetTimeElapsed());
	glUniform1i(m_uniform_layer, layer);
	glUniform1f(m_uniform_gamma, gamma);

	if (m_config.IsDirty())
	{
		for (auto& it : m_config.GetOptions())
		{
			if (it.second.m_dirty)
			{
				switch (it.second.m_type)
				{
				case PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_BOOL:
					glUniform1i(m_uniform_bindings[it.first], it.second.m_bool_value);
				break;
				case PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_INTEGER:
					switch (it.second.m_integer_values.size())
					{
					case 1:
						glUniform1i(m_uniform_bindings[it.first], it.second.m_integer_values[0]);
					break;
					case 2:
						glUniform2i(m_uniform_bindings[it.first],
								it.second.m_integer_values[0],
						            it.second.m_integer_values[1]);
					break;
					case 3:
						glUniform3i(m_uniform_bindings[it.first],
								it.second.m_integer_values[0],
								it.second.m_integer_values[1],
						            it.second.m_integer_values[2]);
					break;
					case 4:
						glUniform4i(m_uniform_bindings[it.first],
								it.second.m_integer_values[0],
								it.second.m_integer_values[1],
								it.second.m_integer_values[2],
						            it.second.m_integer_values[3]);
					break;
					}
				break;
				case PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_FLOAT:
					switch (it.second.m_float_values.size())
					{
					case 1:
						glUniform1f(m_uniform_bindings[it.first], it.second.m_float_values[0]);
					break;
					case 2:
						glUniform2f(m_uniform_bindings[it.first],
								it.second.m_float_values[0],
						            it.second.m_float_values[1]);
					break;
					case 3:
						glUniform3f(m_uniform_bindings[it.first],
								it.second.m_float_values[0],
								it.second.m_float_values[1],
						            it.second.m_float_values[2]);
					break;
					case 4:
						glUniform4f(m_uniform_bindings[it.first],
								it.second.m_float_values[0],
								it.second.m_float_values[1],
								it.second.m_float_values[2],
						            it.second.m_float_values[3]);
					break;
					}
				break;
				}
				it.second.m_dirty = false;
			}
		}
		m_config.SetDirty(false);
	}

	glActiveTexture(GL_TEXTURE0+9);
	glBindTexture(GL_TEXTURE_2D_ARRAY, src_texture);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_2D_ARRAY, src_texture_depth);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glActiveTexture(GL_TEXTURE0 + 9);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	glActiveTexture(GL_TEXTURE0 + 10);
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void OpenGLPostProcessing::ApplyShader()
{
	// shader didn't changed
	if (m_initialized && m_config.GetShader() == g_ActiveConfig.sPostProcessingShader)
		return;

	m_shader.Destroy();
	m_uniform_bindings.clear();

	// load shader code
	std::string code = m_config.LoadShader();
	code = LoadShaderOptions(code);

	const char* vertex_shader = s_vertex_shader;

	if (m_attribute_workaround)
		vertex_shader = s_vertex_workaround_shader;

	// and compile it
	if (!ProgramShaderCache::CompileShader(m_shader, vertex_shader, code.c_str()))
	{
		ERROR_LOG(VIDEO, "Failed to compile post-processing shader %s", m_config.GetShader().c_str());
		g_Config.sPostProcessingShader.clear();
		g_ActiveConfig.sPostProcessingShader.clear();
		code = m_config.LoadShader();
		code = LoadShaderOptions(code);
		ProgramShaderCache::CompileShader(m_shader, vertex_shader, code.c_str());
	}

	// read uniform locations
	m_uniform_resolution = glGetUniformLocation(m_shader.glprogid, "resolution");
	m_uniform_gamma = glGetUniformLocation(m_shader.glprogid, "native_gamma");
	m_uniform_time = glGetUniformLocation(m_shader.glprogid, "time");
	m_uniform_src_rect = glGetUniformLocation(m_shader.glprogid, "src_rect");
	m_uniform_layer = glGetUniformLocation(m_shader.glprogid, "layer");

	if (m_attribute_workaround)
	{
		GLfloat vertices[] = {
			-1.f, -1.f, 0.f, 0.f,
			 1.f, -1.f, 1.f, 0.f,
			-1.f,  1.f, 0.f, 1.f,
			 1.f,  1.f, 1.f, 1.f,
		};

		glBindBuffer(GL_ARRAY_BUFFER, m_attribute_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_attribute_vao);
		glEnableVertexAttribArray(SHADER_POSITION_ATTRIB);
		glVertexAttribPointer(SHADER_POSITION_ATTRIB, 4, GL_FLOAT, 0, 0, nullptr);
	}

	for (const auto& it : m_config.GetOptions())
	{
		std::string glsl_name = "option_" + it.first;
		m_uniform_bindings[it.first] = glGetUniformLocation(m_shader.glprogid, glsl_name.c_str());
	}
	m_initialized = true;
}

void OpenGLPostProcessing::CreateHeader()
{
	m_glsl_header = R"GLSL(
// Required variables
// Shouldn't be accessed directly by the PP shader
// Texture sampler
SAMPLER_BINDING(8) uniform sampler2D samp8;
SAMPLER_BINDING(9) uniform sampler2DArray samp9;
SAMPLER_BINDING(10) uniform sampler2DArray samp10;

// Output variable
out float4 ocol0;
// Input coordinates
in float2 uv0;
// Resolution
uniform float4 resolution;
// Time
uniform uint time;
// Layer
uniform int layer;
// Gamma
uniform float native_gamma;

// Interfacing functions		
float2 GetFragmentCoord()
{
	return gl_FragCoord.xy;
}

float4 Sample(float2 location, int l)
{
	return texture(samp9, float3(location, l));
}

float SampleDepth(float2 location, int l)
{
	/*float Znear = 0.001;
	float Zfar = 1.0;
	float A  = (1 - ( Zfar / Znear ))/2;
	float B = (1 + ( Zfar / Znear ))/2;*/
	float A  = -499.5;
	float B = 500.5;
	float depth = texture(samp10, float3(location, l)).x;
	depth = 1 / (A * depth + B);
	return depth;
}

float4 Sample()
{
	return Sample(uv0, layer);
}

float SampleDepth()
{
	return SampleDepth(uv0, layer);
}

float4 SampleLocation(float2 location)
{
	return Sample(location, layer);
}

float SampleDepthLocation(float2 location)
{
	return SampleDepth(location, layer);
}

float4 SampleLayer(int l)
{
	return Sample(uv0, l);
}

float SampleDepthLayer(int l)
{
	return SampleDepth(uv0, l);
}

#define SampleOffset(offset) textureOffset(samp9, float3(uv0, layer), offset)
#define SampleDepthOffset(offset) textureOffset(samp10, float3(uv0, layer), offset).x

float4 SampleFontLocation(float2 location)
{
	return texture(samp8, location);
}

float4 ApplyGCGamma(float4 col)
{
	return pow(col, float4(native_gamma, native_gamma, native_gamma, native_gamma));
}

float2 GetResolution()
{
	return resolution.xy;
}

float2 GetInvResolution()
{
	return resolution.zw;
}

float2 GetCoordinates()
{
	return uv0;
}

uint GetTime()
{
	return time;
}

void SetOutput(float4 color)
{
	ocol0 = color;
}

#define GetOption(x) (option_##x)
#define OptionEnabled(x) (option_##x != 0)
)GLSL";
}

std::string OpenGLPostProcessing::LoadShaderOptions(const std::string& code)
{
	std::string glsl_options = "";
	m_uniform_bindings.clear();

	for (const auto& it : m_config.GetOptions())
	{
		if (it.second.m_type == PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_BOOL)
		{
			glsl_options += StringFromFormat("uniform int     option_%s;\n", it.first.c_str());
		}
		else if (it.second.m_type == PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_INTEGER)
		{
			u32 count = static_cast<u32>(it.second.m_integer_values.size());
			if (count == 1)
				glsl_options += StringFromFormat("uniform int     option_%s;\n", it.first.c_str());
			else
				glsl_options += StringFromFormat("uniform int%d   option_%s;\n", count, it.first.c_str());
		}
		else if (it.second.m_type == PostProcessingShaderConfiguration::ConfigurationOption::OptionType::OPTION_FLOAT)
		{
			u32 count = static_cast<u32>(it.second.m_float_values.size());
			if (count == 1)
				glsl_options += StringFromFormat("uniform float   option_%s;\n", it.first.c_str());
			else
				glsl_options += StringFromFormat("uniform float%d option_%s;\n", count, it.first.c_str());
		}

		m_uniform_bindings[it.first] = 0;
	}

	return m_glsl_header + glsl_options + code;
}

}  // namespace OGL
