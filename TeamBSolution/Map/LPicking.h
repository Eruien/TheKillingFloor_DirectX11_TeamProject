#pragma once
#include "LStd.h"
#include "TMath.h"
#include "LInput.h"
#include "LGlobal.h"
//#include "LNode.h"
#include "Collision.h"
#include "MapNode.h"





class LPicking
{
public:
	vector<UINT> UpdateVertexIndexList; //
	vector<SHORT> UpdateNodeIndexList; //

public:
	void FindVertex(TVector3 centerPosition, float range, int leafNodeNum,
					shared_ptr<MapNode>& pickNode,
					vector<SimpleVertex>& vertexList);

	shared_ptr<MapNode>& FindPickedFace(Ray& ray, vector<UINT>& leafNodeIdxList
		, map<int, shared_ptr<MapNode>>& leafNodeMap
		, OUT TVector3& pickPoint);
};

