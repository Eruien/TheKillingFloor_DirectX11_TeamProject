#include "LPicking.h"

void LPicking::FindVertex(TVector3 centerPosition, float range, int leafNodeNum, shared_ptr<MapNode>& pickNode, vector<SimpleVertex>& vertexList)
{
	// 0 : right, 1 : left, 2 : bottom, 3 : top
	// find node by node idx
	DWORD LT, RT, LB;
	DWORD leftTopNodeIdx, leftBottomNodeIdx, rightTopNodeIdx;

	shared_ptr<MapNode> pNode = pickNode;

	
	// lt, lb
	{
		while (true)
		{
			if (pNode->nearNodeList[1] == nullptr)
				break;

			auto& ltConerPos = vertexList[pNode->cornerIndexList[0]].p;

			if (centerPosition.x - range < ltConerPos.x)
				pNode = pNode->nearNodeList[1];
			else
				break;
		}

		// top
		std::shared_ptr<MapNode> ltNode = pNode;

		while (true)
		{
			if (ltNode->nearNodeList[3] == nullptr)
				break;

			auto& ltConerPos = vertexList[ltNode->cornerIndexList[0]].p;

			if (centerPosition.z + range > ltConerPos.z)
				ltNode = ltNode->nearNodeList[3];
			else
				break;
		}
		LT = ltNode->cornerIndexList[0];
		leftTopNodeIdx = ltNode->nodeIndex;

		// bottom
		shared_ptr<MapNode> lbNode = pNode;

		while (true)
		{
			if (lbNode->nearNodeList[2] == nullptr)
				break;

			auto& lbConerPos = vertexList[lbNode->cornerIndexList[2]].p;

			if (centerPosition.z - range < lbConerPos.z)
				lbNode = lbNode->nearNodeList[2];
			else
				break;
		}
		LB = lbNode->cornerIndexList[2];
		leftBottomNodeIdx = lbNode->nodeIndex;
	}


	// rt, rb
	{
		pNode = pickNode;

		while (true)
		{
			if (pNode->nearNodeList[0] == nullptr)
				break;

			auto& rtConerPos = vertexList[pNode->cornerIndexList[1]].p;

			if (centerPosition.x + range > rtConerPos.x)
				pNode = pNode->nearNodeList[0];
			else
				break;
		}


		// top
		shared_ptr<MapNode> rtNode = pNode;

		while (true)
		{
			if (rtNode->nearNodeList[3] == nullptr)
				break;

			auto& rtConerPos = vertexList[rtNode->cornerIndexList[1]].p;

			if (centerPosition.z + range > rtConerPos.z)
				rtNode = rtNode->nearNodeList[3];
			else
				break;
		}
		RT = rtNode->cornerIndexList[1];
		rightTopNodeIdx = rtNode->nodeIndex;
	}

	// get update node idx
	{
		SHORT nodeColNum = sqrt(leafNodeNum);
		SHORT iStartRow = leftTopNodeIdx / nodeColNum;
		SHORT iEndRow = leftBottomNodeIdx / nodeColNum;
		SHORT iStartCol = leftTopNodeIdx % nodeColNum;
		SHORT iEndCol = rightTopNodeIdx % nodeColNum;

		SHORT iIndex = 0;
		UpdateNodeIndexList.clear();
		for (SHORT iRow = iStartRow; iRow <= iEndRow; iRow++)
		{
			for (SHORT iCol = iStartCol; iCol <= iEndCol; iCol++)
			{
				SHORT iCurrentIndex = iRow * nodeColNum + iCol;

				UpdateNodeIndexList.push_back(iCurrentIndex);
			}
		}
	}

	// get update vertex idx
	{
		Circle circle = Circle(TVector2(centerPosition.x, centerPosition.z), range);

		int iNumCols = sqrt(vertexList.size());
		int iStartRow = LT / iNumCols;
		int iEndRow = LB / iNumCols;
		int iStartCol = LT % iNumCols;
		int iEndCol = RT % iNumCols;

		int iIndex = 0;

		UpdateVertexIndexList.clear();
		for (int iRow = iStartRow; iRow <= iEndRow; iRow++)
		{
			for (int iCol = iStartCol; iCol <= iEndCol; iCol++)
			{
				int iCurrentIndex = iRow * iNumCols + iCol;

				if (Collision::CircleToPoint(circle, vertexList[iCurrentIndex].p))
					UpdateVertexIndexList.push_back(iCurrentIndex);
			}
		}
	}
}

shared_ptr<MapNode>& LPicking::FindPickedFace(Ray& ray, vector<UINT>& leafNodeIdxList, map<int, shared_ptr<MapNode>>& leafNodeMap, OUT TVector3& pickPoint)
{
	// find leaf node collision with ray
	vector<shared_ptr<MapNode>> sectionList;
	for (int i = 0; i < leafNodeMap.size(); ++i)
	{
		if (Collision::CubeToRay(leafNodeMap[i]->boundingBox, ray))
			sectionList.push_back(leafNodeMap[i]);
	}

	// find face collision with ray
	for (int i = 0; i < sectionList.size(); ++i)
	{
		auto& leafVertices = sectionList[i]->vertices;
		for (int j = 0; j < leafNodeIdxList.size(); j += 3)
		{
			TVector3 v0 = leafVertices[leafNodeIdxList[j + 0]].p;
			TVector3 v1 = leafVertices[leafNodeIdxList[j + 1]].p;
			TVector3 v2 = leafVertices[leafNodeIdxList[j + 2]].p;

			if (Collision::RayToFace(ray, v0, v1, v2, &pickPoint))
				return leafNodeMap[i];
		}
	}
		
	shared_ptr<MapNode> pickNode = nullptr;
	return pickNode;
}
