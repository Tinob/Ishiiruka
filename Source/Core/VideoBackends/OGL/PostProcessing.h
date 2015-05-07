// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "VideoBackends/OGL/GLUtil.h"
#include "VideoBackends/OGL/ProgramShaderCache.h"

#include "VideoCommon/PostProcessing.h"
#include "VideoCommon/VideoCommon.h"

namespace OGL
{

class OpenGLPostProcessing : public PostProcessingShaderImplementation
{
public:
	OpenGLPostProcessing();
	~OpenGLPostProcessing();

	void BlitFromTexture(const TargetRectangle &src, const TargetRectangle &dst,
		void* src_texture_ptr, void* src_depth_texture_ptr, int src_width, int src_height, int layer, float gamma) override;
	void ApplyShader() override;

private:
	void DestroyStageOutput();
	u32 m_prev_width;
	u32 m_prev_height;
	bool m_initialized;
	struct ShaderInstance
	{
		SHADER shader;
		GLuint m_uniform_resolution;
		GLuint m_uniform_gamma;
		GLuint m_uniform_src_rect;
		GLuint m_uniform_time;
		GLuint m_uniform_layer;
		std::unordered_map<std::string, GLuint> m_uniform_bindings;
	};
	std::vector<ShaderInstance> m_shaders;
	std::vector<std::pair<GLuint, GLuint>> m_stageOutput;
	
	std::string m_glsl_header;

	// These are only used when working around Qualcomm's broken attributeless rendering
	GLuint m_attribute_vao;
	GLuint m_attribute_vbo;
	bool m_attribute_workaround = false;
	void CreateHeader();
	std::string LoadShaderOptions(const std::string& code);
};

}  // namespace
