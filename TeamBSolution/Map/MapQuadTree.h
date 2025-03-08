#pragma once
#include "MapNode.h"

#include "Terrain.h"
#include "RenderMgr.h"
#include "DebugDrawer.h"


class MapQuadTree
{

public:
	int maxDepth = 4;

	std::weak_ptr<Terrain> terrain;
	std::shared_ptr<MapNode> root;

	std::vector<UINT> leafNodeIndexList;
	std::shared_ptr<IndexBuffer> leafNodeIndexBuffer;

	std::map<int, std::shared_ptr<MapNode>> leafNodeMap;
	std::vector<int> drawNodeIndexList;


	// temp : for debug
	std::shared_ptr<DebugDrawer> debugDraw;
private:
	void FindDrawNode();

	// build tree
	void BuildTree(std::shared_ptr<MapNode> pNode);
	void SetNeighborNode();
	bool SubDivide(std::shared_ptr<MapNode> pNode);
	UINT CheckSize(UINT dwSize);
	std::shared_ptr<MapNode> CreateNode(std::shared_ptr<MapNode> pParent, DWORD LT, DWORD RT, DWORD LB, DWORD RB);

	void UpdateVertexList(std::shared_ptr<MapNode> pNode);
	void CreateIndexBuffer(UINT rowNum, UINT colNum);

public:
	void Init();
	void Update();
	void Render();

	void UpdateVertex(std::vector<SHORT> updateNodeIdxList);

	MapQuadTree(std::shared_ptr<Terrain> owner);
	~MapQuadTree();
};

