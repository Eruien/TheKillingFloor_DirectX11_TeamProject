#include "MapNode.h"



void MapNode::SetBoundingBox()
{
    TVector3 min = TVector3::Zero;
    TVector3 max = TVector3::Zero;

    TVector3& dwV0Pos = vertices[0].p;
    TVector3& dwV1Pos = vertices[vertices.size() - 1].p;
    TVector2 vHeight = GetHeight();
    if (vHeight.x == vHeight.y)
        vHeight.y -= 1.0f;

    min.x = dwV0Pos.x;
    min.y = vHeight.y;
    min.z = dwV1Pos.z;

    max.x = dwV1Pos.x;
    max.y = vHeight.x;
    max.z = dwV0Pos.z;

    boundingBox.SetCube(min, max);
}

TVector2 MapNode::GetHeight()
{
    TVector2 vHeight;
    vHeight.x = -99999999.0f;
    vHeight.y = 99999999.0f;

    int size = sqrt(vertices.size());

    for (int i = 0; i < vertices.size(); ++i)
    {
        if (vertices[i].p.y > vHeight.x)
            vHeight.x = vertices[i].p.y;
        if (vertices[i].p.y < vHeight.y)
            vHeight.y = vertices[i].p.y;
    }

    return vHeight;
}

void MapNode::Render()
{
    if (vertexBuffer == nullptr || indexBuffer == nullptr || shader == nullptr)
        return;

    UINT stride = vertexBuffer->GetStride();
    UINT offset = vertexBuffer->GetOffset();

    LGlobal::g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    LGlobal::g_pImmediateContext->IASetVertexBuffers(0, 1, vertexBuffer->GetVertexBuffer().GetAddressOf(), &stride, &offset);
    LGlobal::g_pImmediateContext->IASetIndexBuffer(indexBuffer->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

    shader->DrawIndexed(0, 0, indexBuffer->GetIndexCount());
}

void MapNode::SetVertexBuffer()
{
    vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->CreateVertexBuffer(vertices, 0, true, false);
}

void MapNode::UpdateVertexBuffer()
{
    SetBoundingBox();
    vertexBuffer->UpdateVertexBuffer(vertices);
}

MapNode::MapNode()
{
    vertexBuffer = nullptr;
    indexBuffer = nullptr;
    shader = nullptr;
}

MapNode::MapNode(MapNodeDesc& desc)
{
    parentNode = desc.pParent;
    if (desc.pParent != nullptr)
        depth = desc.pParent->depth + 1;

    cornerIndexList.resize(4);
    cornerIndexList[0] = desc.LT;
    cornerIndexList[1] = desc.RT;
    cornerIndexList[2] = desc.LB;
    cornerIndexList[3] = desc.RB;

    //set node index
    ldiv_t divVal = ldiv((long)desc.LT, (long)desc.colNum);
    element.x = divVal.rem / (desc.RT - desc.LT);
    element.y = divVal.quot / (desc.RT - desc.LT);

    UINT dwNumPatchCount = (UINT)pow(2.0f, (float)depth);
    nodeIndex = element.y * dwNumPatchCount + element.x;
}
