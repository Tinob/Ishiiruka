// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2
// Refer to the license.txt file included.

#pragma once

#include <d3d11_2.h>
#include <array>
#include <functional>
namespace DX11
{

namespace D3D
{

// Wrap a ID3D11DeviceContext to prune redundant state changes.
class WrapDeviceContext 
{
	ID3D11DeviceContext*     m_ctx{ nullptr };
	ID3D11DeviceContext1*    m_ctx1{ nullptr };
	struct Cache 
	{
		ID3D11PixelShader *      m_ps{};
		ID3D11GeometryShader *   m_gs{};
		ID3D11VertexShader *     m_vs{};
		D3D11_PRIMITIVE_TOPOLOGY m_topology = D3D11_PRIMITIVE_TOPOLOGY(-1);
		ID3D11InputLayout *      m_il{};

		ID3D11Buffer *           m_ib{};
		DXGI_FORMAT              m_Format = DXGI_FORMAT(-1);
		UINT                     m_Offset = UINT(-1);

		// only cache first vb
		ID3D11Buffer *           m_vb{};
		UINT                     m_vbStride = UINT(-1);
		UINT                     m_vbOffset = UINT(-1);

		ID3D11RasterizerState *  m_rasterizerState{};

		ID3D11BlendState *       m_blendState{};
		std::array<FLOAT, 4>      m_blendFactor;
		UINT                     m_sampleMask = 0xffffffffu;

		ID3D11DepthStencilState * m_depthStencilState{};
		UINT                      m_stencilRef{};

		std::array<ID3D11ShaderResourceView*, 8> m_psSrvs;
		std::array<ID3D11SamplerState*, 8> m_samplerStates;
		std::array<ID3D11Buffer*, 8> m_vsConstants;
		std::array<ID3D11Buffer*, 8> m_psConstants;
		std::array<ID3D11Buffer*, 8> m_gsConstants;

		Cache() 
		{
			m_blendFactor.fill(1.0f);
			m_psSrvs.fill(nullptr);
			m_samplerStates.fill(nullptr);
			m_vsConstants.fill(nullptr);
			m_psConstants.fill(nullptr);
			m_gsConstants.fill(nullptr);
		}
	};

	Cache m_c;

public:

	// helpers to keep wrapper transparent to user code
	WrapDeviceContext*     operator->() { return this; }
	ID3D11DeviceContext**  operator&() { return &m_ctx; }
	void                   operator=(std::nullptr_t) { m_ctx = nullptr; }
	explicit operator bool() const { return m_ctx != nullptr; }
	operator ID3D11DeviceChild* () { return m_ctx; }

	void InitContext1() 
	{
		m_ctx->QueryInterface(__uuidof(ID3D11DeviceContext1), (void**)&m_ctx1);
	}
	//
	inline ULONG Release() 
	{
		m_c = Cache{}; // in case of restart, as i am a global variable.
		if (m_ctx1 != nullptr)
		{
			m_ctx1->Release();
		}
		return m_ctx->Release();
	}

	inline void Flush()
	{
		m_ctx->Flush();
	}

	//
	inline void Unmap(ID3D11Resource *pResource, UINT Subresource)
	{
		m_ctx->Unmap(pResource, Subresource);
	}
	inline HRESULT Map(
		ID3D11Resource *pResource,
		UINT Subresource, D3D11_MAP MapType,
		UINT MapFlags,
		D3D11_MAPPED_SUBRESOURCE *pMappedResource)
	{
		return m_ctx->Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
	}

	//
	inline void ClearState()
	{
		m_c = Cache{};
		m_ctx->ClearState();
	}

	inline void OMSetBlendState(ID3D11BlendState *pBlendState, const FLOAT BlendFactor[4], UINT SampleMask)
	{
		static float const defaultBlendFactors[4]{1.0f, 1.0f, 1.0f, 1.0f};
		if (m_c.m_blendState != pBlendState 
			|| m_c.m_sampleMask != SampleMask 
			|| memcmp(m_c.m_blendFactor.data(), BlendFactor ? BlendFactor : defaultBlendFactors, sizeof(m_c.m_blendFactor))) 
		{
			m_c.m_blendState = pBlendState;
			m_c.m_sampleMask = SampleMask;
			memcpy(m_c.m_blendFactor.data(), BlendFactor ? BlendFactor : defaultBlendFactors, sizeof(m_c.m_blendFactor));
			m_ctx->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
		}
	}

	inline void OMSetDepthStencilState(ID3D11DepthStencilState *pDepthStencilState, UINT StencilRef)
	{
		if (m_c.m_depthStencilState != pDepthStencilState || m_c.m_stencilRef != StencilRef) 
		{
			m_c.m_depthStencilState = pDepthStencilState;
			m_c.m_stencilRef = StencilRef;
			m_ctx->OMSetDepthStencilState(pDepthStencilState, StencilRef);
		}
	}

	inline void RSSetState(ID3D11RasterizerState *pRasterizerState)
	{
		if (m_c.m_rasterizerState != pRasterizerState) 
		{
			m_c.m_rasterizerState = pRasterizerState;
			m_ctx->RSSetState(pRasterizerState);
		}
	}

	inline void RSSetScissorRects(UINT NumRects, const D3D11_RECT *pRects)
	{
		m_ctx->RSSetScissorRects(NumRects, pRects);
	}

	inline void RSSetViewports(UINT NumViewports, const D3D11_VIEWPORT *pViewports)
	{
		m_ctx->RSSetViewports(NumViewports, pViewports);
	}

	inline void IASetIndexBuffer(ID3D11Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
	{
		if (m_c.m_ib != pIndexBuffer || m_c.m_Format != Format || m_c.m_Offset != Offset) 
		{
			m_c.m_ib = pIndexBuffer;
			m_c.m_Format = Format;
			m_c.m_Offset = Offset;
			m_ctx->IASetIndexBuffer(pIndexBuffer, Format, Offset);
		}
	}

	inline void IASetInputLayout(ID3D11InputLayout *pInputLayout) {
		if (pInputLayout != m_c.m_il) 
		{
			m_c.m_il = pInputLayout;
			m_ctx->IASetInputLayout(pInputLayout);
		}
	}

	inline void IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology)
	{
		if (m_c.m_topology != Topology) 
		{
			m_c.m_topology = Topology;
			m_ctx->IASetPrimitiveTopology(Topology);
		}
	}

	inline void OMSetRenderTargets(
		UINT NumViews,
		ID3D11RenderTargetView *const *ppRenderTargetViews,
		ID3D11DepthStencilView *pDepthStencilView)
	{
		std::array<ID3D11ShaderResourceView*, 8> nils;
		nils.fill(nullptr);
		PSSetShaderResources(0, 8, nils.data());
		m_ctx->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
	}

	inline void IASetVertexBuffers(
		UINT StartSlot,
		UINT NumBuffers,
		ID3D11Buffer *const *ppVertexBuffers,
		const UINT *pStrides,
		const UINT *pOffsets)
	{
		if (m_c.m_vb != *ppVertexBuffers || m_c.m_vbStride != *pStrides || m_c.m_vbOffset != *pOffsets) 
		{
			m_c.m_vb = *ppVertexBuffers;
			m_c.m_vbStride = *pStrides;
			m_c.m_vbOffset = *pOffsets;
			m_ctx->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
		}
	}

	// Shader states
	inline void PSSetShader(
		ID3D11PixelShader *pShader,
		ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances)
	{
		if (pShader != m_c.m_ps) 
		{
			m_c.m_ps = pShader;
			m_ctx->PSSetShader(pShader, ppClassInstances, NumClassInstances);
		}
	}
	
	inline void GSSetShader(
		ID3D11GeometryShader *pShader,
		ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances)
	{
		if (pShader != m_c.m_gs) 
		{
			m_c.m_gs = pShader;
			m_ctx->GSSetShader(pShader, ppClassInstances, NumClassInstances);
		}
	}

	inline void VSSetShader(
		ID3D11VertexShader *pShader,
		ID3D11ClassInstance *const *ppClassInstances,
		UINT NumClassInstances)
	{
		if (pShader != m_c.m_vs) 
		{
			m_c.m_vs = pShader;
			m_ctx->VSSetShader(pShader, ppClassInstances, NumClassInstances);
		}
	}

	inline void CSSetShader(
		ID3D11ComputeShader *pShader, 
		ID3D11ClassInstance *const *ppClassInstances, 
		UINT NumClassInstances){
		m_ctx->CSSetShader(pShader, ppClassInstances, NumClassInstances);
	}

	inline void VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers)
	{
		NumBuffers += StartSlot;
		while (StartSlot < NumBuffers)
		{
			if (m_c.m_vsConstants[StartSlot] != *ppConstantBuffers)
			{
				m_c.m_vsConstants[StartSlot] = *ppConstantBuffers;
				m_ctx->VSSetConstantBuffers(StartSlot, 1, ppConstantBuffers);
			}
			++ppConstantBuffers;
			++StartSlot;
		}
	}

	inline void PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer *const *ppConstantBuffers) 
	{
		NumBuffers += StartSlot;
		while (StartSlot < NumBuffers)
		{
			if (m_c.m_psConstants[StartSlot] != *ppConstantBuffers)
			{
				m_c.m_psConstants[StartSlot] = *ppConstantBuffers;
				m_ctx->PSSetConstantBuffers(StartSlot, 1, ppConstantBuffers);
			}
			++ppConstantBuffers;
			++StartSlot;
		}
	}

	inline void GSSetConstantBuffers(
		UINT StartSlot,
		UINT NumBuffers,
		ID3D11Buffer *const *ppConstantBuffers)
	{
		NumBuffers += StartSlot;
		while (StartSlot < NumBuffers)
		{
			if (m_c.m_gsConstants[StartSlot] != *ppConstantBuffers)
			{
				m_c.m_gsConstants[StartSlot] = *ppConstantBuffers;
				m_ctx->GSSetConstantBuffers(StartSlot, 1, ppConstantBuffers);
			}
			++ppConstantBuffers;
			++StartSlot;
		}
	}

	inline void PSSetShaderResources(
		UINT StartSlot,
		UINT NumViews,
		ID3D11ShaderResourceView *const *ppShaderResourceViews)
	{
		NumViews += StartSlot;
		while (StartSlot < NumViews)
		{
			if (m_c.m_psSrvs[StartSlot] != *ppShaderResourceViews)
			{
				m_c.m_psSrvs[StartSlot] = *ppShaderResourceViews;
				m_ctx->PSSetShaderResources(StartSlot, 1, ppShaderResourceViews);
			}
			++ppShaderResourceViews;
			++StartSlot;
		}
	}

	inline void PSSetSamplers(
		UINT StartSlot,
		UINT NumSamplers,
		ID3D11SamplerState *const *ppSamplers)
	{
		NumSamplers += StartSlot;
		while (StartSlot < NumSamplers)
		{
			if (m_c.m_samplerStates[StartSlot] != *ppSamplers)
			{
				m_c.m_samplerStates[StartSlot] = *ppSamplers;
				m_ctx->PSSetSamplers(StartSlot, 1, ppSamplers);
			}
			++ppSamplers;
			++StartSlot;
		}
	}

	// Action Calls
	inline void ClearDepthStencilView(
		ID3D11DepthStencilView *pDepthStencilView,
		UINT ClearFlags,
		FLOAT Depth,
		UINT8 Stencil)
	{
		m_ctx->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
	}
	
	inline void ClearRenderTargetView(
		ID3D11RenderTargetView *pRenderTargetView,
		const FLOAT ColorRGBA[4])
	{
		m_ctx->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
	}

	inline void ClearUnorderedAccessViewUint(
		ID3D11UnorderedAccessView *pRenderTargetView, 
		const UINT ColorRGBA[4]) 
	{
		m_ctx->ClearUnorderedAccessViewUint(pRenderTargetView, ColorRGBA);
	}

	inline void DrawIndexed(
		UINT IndexCount,
		UINT StartIndexLocation,
		INT BaseVertexLocation)
	{
		m_ctx->DrawIndexed(
			IndexCount,
			StartIndexLocation,
			BaseVertexLocation);
	}

	inline void Draw(UINT VertexCount, UINT StartVertexLocation)
	{
		m_ctx->Draw(VertexCount, StartVertexLocation);
	}

	inline void CSSetSamplers(
		UINT StartSlot, 
		UINT NumSamplers, 
		ID3D11SamplerState *const *ppSamplers)
	{
		m_ctx->CSSetSamplers(StartSlot, NumSamplers, ppSamplers);
	}

	inline void CSSetShaderResources(
		UINT StartSlot, 
		UINT NumViews, 
		ID3D11ShaderResourceView *const *ppShaderResourceViews) 
	{
		m_ctx->CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
	}

	inline void CSSetUnorderedAccessViews(
		UINT StartSlot, 
		UINT Num, 
		ID3D11UnorderedAccessView *const *ppUavs)
	{
		m_ctx->CSSetUnorderedAccessViews(StartSlot, Num, ppUavs, nullptr);
	}

	inline void CSSetConstantBuffers(
		UINT StartSlot, 
		UINT NumBuffers, 
		ID3D11Buffer *const *ppConstantBuffers) 
	{
		m_ctx->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
	}


	void Dispatch(UINT X, UINT Y, UINT Z) 
	{
		m_ctx->Dispatch(X, Y, Z);
	}

	inline void UpdateSubresource(
		ID3D11Resource *pDstResource,
		UINT DstSubresource,
		const D3D11_BOX *pDstBox,
		const void *pSrcData,
		UINT SrcRowPitch,
		UINT SrcDepthPitch)
	{
		m_ctx->UpdateSubresource(
			pDstResource,
			DstSubresource,
			pDstBox,
			pSrcData,
			SrcRowPitch,
			SrcDepthPitch);
	}

	inline void OMSetRenderTargetsAndUnorderedAccessViews(
		UINT NumViews,
		ID3D11RenderTargetView *const *ppRenderTargetViews,
		ID3D11DepthStencilView *pDepthStencilView,
		UINT UAVStartSlot,
		UINT NumUAVs,
		ID3D11UnorderedAccessView *const *ppUnorderedAccessView,
		const UINT *pUAVInitialCounts
		)
	{
		std::array<ID3D11ShaderResourceView*, 8> nils;
		nils.fill(nullptr);
		PSSetShaderResources(0, 8, nils.data());
		m_ctx->OMSetRenderTargetsAndUnorderedAccessViews(
			NumViews,
			ppRenderTargetViews,
			pDepthStencilView,
			UAVStartSlot,
			NumUAVs,
			ppUnorderedAccessView,
			pUAVInitialCounts);
	}

	void ClearUnorderedAccessViewFloat(
		ID3D11UnorderedAccessView *pUnorderedAccessView,
		const FLOAT Values[4]
		)
	{
		m_ctx->ClearUnorderedAccessViewFloat(
			pUnorderedAccessView,
			Values);
	}

	inline void UpdateSubresource1(
		ID3D11Resource *pDstResource,
		UINT DstSubresource,
		const D3D11_BOX *pDstBox,
		const void *pSrcData,
		UINT SrcRowPitch,
		UINT SrcDepthPitch,
		UINT copyflags)
	{
		if (m_ctx1)
		{
			m_ctx1->UpdateSubresource1(
				pDstResource,
				DstSubresource,
				pDstBox,
				pSrcData,
				SrcRowPitch,
				SrcDepthPitch, copyflags);
		}
	}

	inline void ResolveSubresource(
		ID3D11Resource *pDstResource, 
		UINT DstSubresource, 
		ID3D11Resource *pSrcResource, 
		UINT SrcSubresource, 
		DXGI_FORMAT Format)
	{
		m_ctx->ResolveSubresource(
			pDstResource,
			DstSubresource,
			pSrcResource,
			SrcSubresource,
			Format);
	}

	inline void CopySubresourceRegion(ID3D11Resource *pDstResource,
		UINT DstSubresource,
		UINT DstX,
		UINT DstY,
		UINT DstZ,
		ID3D11Resource *pSrcResource,
		UINT SrcSubresource,
		const D3D11_BOX *pSrcBox)
	{
		m_ctx->CopySubresourceRegion(
			pDstResource,
			DstSubresource,
			DstX, DstY, DstZ,
			pSrcResource,
			SrcSubresource,
			pSrcBox);
	}

	inline void CopyResource(ID3D11Resource *pDstResource, ID3D11Resource *pSrcResource)
	{
		m_ctx->CopyResource(pDstResource, pSrcResource);
	}

	//
	inline void Begin(ID3D11Asynchronous *pAsync) { m_ctx->Begin(pAsync); }
	inline void End(ID3D11Asynchronous *pAsync) { m_ctx->End(pAsync); }

	inline HRESULT GetData(ID3D11Asynchronous *pAsync, void *pData, UINT DataSize, UINT GetDataFlags)
	{
		return m_ctx->GetData(pAsync, pData, DataSize, GetDataFlags);
	}
};

}  // namespace D3D
}  // namespace DX11
