#include "VertexBuffer.h"


VertexBuffer::VertexBuffer()
{
	this->device = LGlobal::g_pDevice;
}

void VertexBuffer::PushData()
{
	LGlobal::g_pImmediateContext->IASetVertexBuffers(slot, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
}
