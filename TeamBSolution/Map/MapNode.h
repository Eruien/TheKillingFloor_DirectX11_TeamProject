#pragma once
#include "LStd.h"
#include "Collision.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class MapNode;

struct MapNodeDesc
{
	std::shared_ptr<MapNode> pParent;

	UINT LT;
	UINT RT;
	UINT LB;
	UINT RB;

	UINT colNum;
};


class MapNode : public enable_shared_from_this<MapNode>
{
public:
	bool isLeafNode = false;
	int depth = 0;
	int nodeIndex = 0;
	Cube boundingBox; 
	TVector2 element; //

	weak_ptr<MapNode> parentNode;

	shared_ptr<Shader> shader;
	shared_ptr<VertexBuffer> vertexBuffer;
	shared_ptr<IndexBuffer> indexBuffer;
	
	vector<SimpleVertex> vertices;
	vector<UINT> cornerIndexList;
	vector<shared_ptr<MapNode>> childNodeList;
	vector<shared_ptr<MapNode>> nearNodeList;

private:
	TVector2 GetHeight();

public:
	void Render();
	void SetVertexBuffer();
	void UpdateVertexBuffer();
	void SetBoundingBox();
	
	MapNode();
	MapNode(MapNodeDesc& desc);
};

