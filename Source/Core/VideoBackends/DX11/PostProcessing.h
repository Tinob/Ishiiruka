// Copyright 2013 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <vector>

#include "VideoBackends/DX11/D3DBase.h"
#include "VideoBackends/DX11/D3DShader.h"
#include "VideoBackends/DX11/D3DUtil.h"
#include "VideoBackends/DX11/D3DTexture.h"

#include "VideoCommon/PostProcessing.h"
#include "VideoCommon/VideoCommon.h"

namespace DX11
{
	class DX11PostProcessing : public PostProcessingShaderImplementation
	{
	public:
		DX11PostProcessing();
		~DX11PostProcessing();

		void BlitFromTexture(const TargetRectangle &src, const TargetRectangle &dst,
			void* src_texture_ptr, void* src_depth_texture_ptr, int src_width, int src_height, int layer, float gamma) override;
		void ApplyShader() override;

	private:
		std::string InitStages(const std::string &code);
		void UpdateConfiguration();
		u32 m_prev_dst_width;
		u32 m_prev_dst_height;
		u32 m_prev_src_width;
		u32 m_prev_src_height;
		u32 m_prev_samples;
		bool m_initialized;
		D3D::UtilVertexBuffer m_vertexbuffer;
		int m_vertex_buffer_offset;
		bool m_vertex_buffer_observer;
		float pu0, pu1, pv0, pv1;
		D3D::VertexShaderPtr m_vshader;
		std::vector<D3D::PixelShaderPtr> m_pshader;
		std::vector<D3DTexture2D*> m_stageOutput;
		D3D::InputLayoutPtr m_layout;
		D3D::BufferPtr m_params;
		D3D::BufferPtr m_options;
		std::string LoadShaderOptions(const std::string& code);
	};

}  // namespace
