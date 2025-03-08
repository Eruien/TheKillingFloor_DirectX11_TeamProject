#include "Pass.h"

void Pass::Draw(UINT vertexCount, UINT startVertexLocation)
{
	BeginDraw();
	{
		LGlobal::g_pImmediateContext->Draw(vertexCount, startVertexLocation);
	}
	EndDraw();
}

void Pass::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	BeginDraw();
	{
		LGlobal::g_pImmediateContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}
	EndDraw();
}

void Pass::DrawInstanced(UINT vertexCountPerInstance, UINT instanceCount, UINT startVertexLocation, UINT startInstanceLocation)
{
	BeginDraw();
	{
		LGlobal::g_pImmediateContext->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
	}
	EndDraw();
}

void Pass::DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, INT baseVertexLocation, UINT startInstanceLocation)
{
	BeginDraw();
	{
		LGlobal::g_pImmediateContext->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
	}
	EndDraw();
}

void Pass::BeginDraw()
{
	pass->ComputeStateBlockMask(&stateblockMask);

	LGlobal::g_pImmediateContext->IASetInputLayout(inputLayout.Get());
	pass->Apply(0, LGlobal::g_pImmediateContext.Get());
}

void Pass::EndDraw()
{
	if (stateblockMask.RSRasterizerState == 1)
		LGlobal::g_pImmediateContext->RSSetState(stateBlock->RSRasterizerState.Get());

	if (stateblockMask.OMDepthStencilState == 1)
		LGlobal::g_pImmediateContext->OMSetDepthStencilState(stateBlock->OMDepthStencilState.Get(), stateBlock->OMStencilRef);

	if (stateblockMask.OMBlendState == 1)
		LGlobal::g_pImmediateContext->OMSetBlendState(stateBlock->OMBlendState.Get(), stateBlock->OMBlendFactor, stateBlock->OMSampleMask);

	LGlobal::g_pImmediateContext->HSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->DSSetShader(NULL, NULL, 0);
	LGlobal::g_pImmediateContext->GSSetShader(NULL, NULL, 0);
}

void Pass::Dispatch(UINT x, UINT y, UINT z)
{
	pass->Apply(0, LGlobal::g_pImmediateContext.Get());
	LGlobal::g_pImmediateContext->Dispatch(x, y, z);

	ID3D11ShaderResourceView* null[1] = { 0 };
	LGlobal::g_pImmediateContext->CSSetShaderResources(0, 1, null);

	ID3D11UnorderedAccessView* nullUav[1] = { 0 };
	LGlobal::g_pImmediateContext->CSSetUnorderedAccessViews(0, 1, nullUav, NULL);

	LGlobal::g_pImmediateContext->CSSetShader(NULL, NULL, 0);
}