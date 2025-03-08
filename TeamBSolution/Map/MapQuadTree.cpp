#include "MapQuadTree.h"



void MapQuadTree::UpdateVertex(std::vector<SHORT> updateNodeIdxList)
{
    for (auto& idx : updateNodeIdxList)
    {
        auto& node = leafNodeMap[idx];

        UpdateVertexList(node);
        node->SetBoundingBox();
        node->UpdateVertexBuffer();
    }
}

MapQuadTree::MapQuadTree(std::shared_ptr<Terrain> owner) : terrain(owner)
{
}

MapQuadTree::~MapQuadTree()
{
}

// -------------------------------------------------------------------------------
// ------------------------------component functions -----------------------------
// -------------------------------------------------------------------------------

void MapQuadTree::Init()
{
    {
        debugDraw = std::make_shared<DebugDrawer>();
    }

    root = CreateNode(nullptr, 0, terrain.lock()->rowCellNum
        , terrain.lock()->rowNum * terrain.lock()->colCellNum
        , terrain.lock()->rowNum * terrain.lock()->colNum - 1);
    BuildTree(root);

    // set neighbor node
    SetNeighborNode();

    auto box = leafNodeMap[0]->boundingBox;
    UINT leafNodeRowNum = box.max.x - box.min.x;
    UINT leafNodeColNum = box.max.z - box.min.z;
    CreateIndexBuffer(leafNodeRowNum, leafNodeColNum);
}

void MapQuadTree::Update()
{
    debugDraw->Update();

    //temp
    if (LInput::GetInstance().m_KeyState[DIK_P] == KEY_PUSH)
    {
        // draw frustum
        {
            auto& frustum = CameraManager::GetInstance().GetMainCamera()->GetFrustum();

            std::vector<TVector3> points1;
            std::vector<TVector3> points2;
            std::vector<TVector3> points3;
            std::vector<TVector3> points4;
            std::vector<TVector3> points5;
            std::vector<TVector3> points6;
            points1.push_back(frustum.frustumCorners[1]);
            points1.push_back(frustum.frustumCorners[2]);
            points1.push_back(frustum.frustumCorners[0]);
            points1.push_back(frustum.frustumCorners[3]);

            points2.push_back(frustum.frustumCorners[5]);
            points2.push_back(frustum.frustumCorners[6]);
            points2.push_back(frustum.frustumCorners[4]);
            points2.push_back(frustum.frustumCorners[7]);

            points3.push_back(frustum.frustumCorners[5]);
            points3.push_back(frustum.frustumCorners[1]);
            points3.push_back(frustum.frustumCorners[4]);
            points3.push_back(frustum.frustumCorners[0]);

            points4.push_back(frustum.frustumCorners[2]);
            points4.push_back(frustum.frustumCorners[6]);
            points4.push_back(frustum.frustumCorners[3]);
            points4.push_back(frustum.frustumCorners[7]);

            points5.push_back(frustum.frustumCorners[5]);
            points5.push_back(frustum.frustumCorners[6]);
            points5.push_back(frustum.frustumCorners[1]);
            points5.push_back(frustum.frustumCorners[2]);

            points6.push_back(frustum.frustumCorners[0]);
            points6.push_back(frustum.frustumCorners[3]);
            points6.push_back(frustum.frustumCorners[4]);
            points6.push_back(frustum.frustumCorners[7]);

            debugDraw->DrawRect(points1, TVector4(0, 1, 0, 0));
            debugDraw->DrawRect(points2, TVector4(0, 1, 0, 0));
            debugDraw->DrawRect(points3, TVector4(0, 1, 0, 0));
            debugDraw->DrawRect(points4, TVector4(0, 1, 0, 0));
            debugDraw->DrawRect(points5, TVector4(0, 1, 0, 0));
            debugDraw->DrawRect(points6, TVector4(0, 1, 0, 0));
        }

        // draw box
        {
            for (auto& box : leafNodeMap)
                debugDraw->DrawBox(box.second->boundingBox, TVector4(1, 0, 0, 0));
        }
    }

    FindDrawNode();
}

void MapQuadTree::Render()
{

    for (auto& index : drawNodeIndexList)
    {
        leafNodeMap[index]->Render();
    }


    // for debug
    {
        debugDraw->Render();
    }
}

void MapQuadTree::FindDrawNode()
{
    drawNodeIndexList.clear();

    bool isDraw = false;
    auto& frustum = CameraManager::GetInstance().GetMainCamera()->GetFrustum();

    for (int i = 0; i < leafNodeMap.size(); ++i)
    {
        isDraw = true;

        for (int j = 0; j < 6; ++j)
        {
            if (!Collision::CubeToPlane(leafNodeMap[i]->boundingBox, frustum.planes[j]))
            {
                isDraw = false;
                break;
            }
        }

        if (isDraw)
            drawNodeIndexList.push_back(leafNodeMap[i]->nodeIndex);
    }
}


// -------------------------------------------------------------------------------
// ------------------------------build tree functions ----------------------------
// -------------------------------------------------------------------------------

void MapQuadTree::BuildTree(std::shared_ptr<MapNode> pNode)
{
    if (SubDivide(pNode))
    {
        for (int iNode = 0; iNode < 4; ++iNode)
            BuildTree(pNode->childNodeList[iNode]);
    }
}

bool MapQuadTree::SubDivide(std::shared_ptr<MapNode> pNode)
{
    if (pNode == nullptr) return false;

    auto& conerIndexList = pNode->cornerIndexList;
    UINT width = terrain.lock()->rowNum;

    // �ִ� ���� �ѵ��� �����Ѵ�.
    if (maxDepth <= pNode->depth)
    {
        pNode->isLeafNode = true;

        UpdateVertexList(pNode);

        pNode->SetVertexBuffer();
        pNode->SetBoundingBox();
        pNode->shader = terrain.lock()->shader;

        leafNodeMap.insert(std::make_pair(pNode->nodeIndex, pNode));

        return false;
    }

    // ���� = �����ڳ� + ���ø�ũ��
    // ���ϱ� = ���� + ( ���ø� ũ�� * ��ü�� ����ũ�� )

    UINT dwOffsetX = (conerIndexList[1] - conerIndexList[0]);
    UINT dwOffsetZ = ((conerIndexList[2] - conerIndexList[0]) / width);

    UINT dwWidthSplit = CheckSize(dwOffsetX);
    UINT dwHeightplit = CheckSize(dwOffsetZ);
    UINT dwSplitCenter = (conerIndexList[0] + dwWidthSplit) + (dwHeightplit * width);

    UINT dwEdgeCenter[4];
    dwEdgeCenter[0] = conerIndexList[0] + dwWidthSplit;
    dwEdgeCenter[1] = conerIndexList[1] + dwHeightplit * width;
    dwEdgeCenter[2] = conerIndexList[2] + dwWidthSplit;
    dwEdgeCenter[3] = conerIndexList[0] + dwHeightplit * width;

    pNode->childNodeList.push_back(CreateNode(pNode, conerIndexList[0],
        dwEdgeCenter[0],
        dwEdgeCenter[3],
        dwSplitCenter));

    pNode->childNodeList.push_back(CreateNode(pNode, dwEdgeCenter[0],
        conerIndexList[1],
        dwSplitCenter,
        dwEdgeCenter[1]));

    pNode->childNodeList.push_back(CreateNode(pNode, dwEdgeCenter[3],
        dwSplitCenter,
        conerIndexList[2],
        dwEdgeCenter[2]));

    pNode->childNodeList.push_back(CreateNode(pNode, dwSplitCenter,
        dwEdgeCenter[1],
        dwEdgeCenter[2],
        conerIndexList[3]));

    return true;
}

void MapQuadTree::SetNeighborNode()
{
    //���ϴ� LowQuality ��ġ�� ����
    for (int iNode = 0; iNode < leafNodeMap.size(); iNode++)
    {
        auto iter = leafNodeMap.find(iNode);
        assert(iter != leafNodeMap.end());

        auto& pNode = iter->second;
        DWORD dwNumPatchCount = (DWORD)pow(2.0f, (float)pNode->depth);
        DWORD dwNeighborCol = 0;
        DWORD dwNeighborRow = 0;

        pNode->nearNodeList.resize(4);

        if (pNode->element.y > 0)  //��
        {
            dwNeighborCol = pNode->element.x;
            dwNeighborRow = (pNode->element.y - 1) * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);

            assert(iter != leafNodeMap.end());

            pNode->nearNodeList[3] = iter->second;
        }
        if (pNode->element.y < dwNumPatchCount - 1) // ��
        {
            dwNeighborCol = pNode->element.x;
            dwNeighborRow = (pNode->element.y + 1) * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->nearNodeList[2] = iter->second;
        }
        if (pNode->element.x > 0) // ��
        {
            dwNeighborCol = pNode->element.x - 1;
            dwNeighborRow = pNode->element.y * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->nearNodeList[1] = iter->second;
        }
        if (pNode->element.x < dwNumPatchCount - 1) // ��
        {
            dwNeighborCol = pNode->element.x + 1;
            dwNeighborRow = pNode->element.y * dwNumPatchCount;
            auto iter = leafNodeMap.find(dwNeighborRow + dwNeighborCol);
            _ASSERT(iter != leafNodeMap.end());
            pNode->nearNodeList[0] = iter->second;
        }
    }
}

void MapQuadTree::UpdateVertexList(std::shared_ptr<MapNode> pNode)
{
    int iNumCols = terrain.lock()->colNum;
    int iStartRow = pNode->cornerIndexList[0] / iNumCols;
    int iEndRow = pNode->cornerIndexList[2] / iNumCols;
    int iStartCol = pNode->cornerIndexList[0] % iNumCols;
    int iEndCol = pNode->cornerIndexList[1] % iNumCols;

    int iNumColCell = iEndCol - iStartCol;
    int iNumRowCell = iEndRow - iStartRow;
    pNode->vertices.resize((iEndCol - iStartCol + 1) * (iEndRow - iStartRow + 1));

    int iIndex = 0;
    auto& terrainVertexList = terrain.lock()->vertices;
    for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
    {
        for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
        {
            int iCurrentIndex = iRow * iNumCols + iCol;
            pNode->vertices[iIndex++] = terrainVertexList[iCurrentIndex];
        }
    }
}

UINT MapQuadTree::CheckSize(UINT dwSize)
{
    float fCount = 0;
    DWORD dwChecker = dwSize / 2;
    if (dwChecker <= 0) return 0;

    while (dwChecker != 0)
    {
        dwChecker >>= 1;
        fCount += 1.0f;
    }
    float fMinimum = pow(2.0f, fCount - 1.0f);
    float fMaximum = pow(2.0f, fCount);
    // fMin = abs( fMin - P0 ) - abs( P1 - fMin );
    // fMax = abs( fMax - P0 ) - abs( P1 - fMax );
    // return min( fMin, fMax );
    float fMin = fabs(fMinimum - fabs(((float)dwSize - fMinimum)));
    float fMax = fabs(fMaximum - fabs(((float)dwSize - fMaximum)));

    // �񱳰��� ���ٸ� ���� ����ũ�⸦ ��ȯ�Ѵ�. ��) fMin=1 : fMax=1 => fMinimum=2 : fMaximum=4
    if (fMin <= fMax)
    {
        return (UINT)fMinimum;
    }
    return (UINT)fMaximum;
}

std::shared_ptr<MapNode> MapQuadTree::CreateNode(std::shared_ptr<MapNode> pParent, DWORD LT, DWORD RT, DWORD LB, DWORD RB)
{
    MapNodeDesc desc;

    desc.pParent = pParent;
    desc.LT = LT;
    desc.RT = RT;
    desc.LB = LB;
    desc.RB = RB;
    desc.colNum = terrain.lock()->colNum;

    std::shared_ptr<MapNode> NewNode = std::make_shared<MapNode>(desc);



    return NewNode;
}

void MapQuadTree::CreateIndexBuffer(UINT rowCellNum, UINT colCellNum)
{
    if (leafNodeIndexBuffer == nullptr)
    {
        leafNodeIndexList;

        UINT faceCount = rowCellNum * colCellNum * 2;
        leafNodeIndexList.resize(faceCount * 3);

        UINT rowNum = rowCellNum + 1;
        UINT colNum = colCellNum + 1;

        UINT iIndex = 0;
        for (UINT iRow = 0; iRow < rowCellNum; ++iRow)
        {
            for (UINT iCol = 0; iCol < colCellNum; ++iCol)
            {
                UINT nextCol = iCol + 1;
                UINT nextRow = iRow + 1;

                leafNodeIndexList[iIndex + 0] = iRow * colNum + iCol;
                leafNodeIndexList[iIndex + 1] = iRow * colNum + nextCol;
                leafNodeIndexList[iIndex + 2] = nextRow * colNum + iCol;

                leafNodeIndexList[iIndex + 3] = leafNodeIndexList[iIndex + 2];
                leafNodeIndexList[iIndex + 4] = leafNodeIndexList[iIndex + 1];
                leafNodeIndexList[iIndex + 5] = nextRow * colNum + nextCol;

                iIndex += 6;
            }
        }

        leafNodeIndexBuffer = std::make_shared<IndexBuffer>();
        leafNodeIndexBuffer->CreateIndexBuffer(leafNodeIndexList);
    }

    for (auto& node : leafNodeMap)
        node.second->indexBuffer = leafNodeIndexBuffer;
}
