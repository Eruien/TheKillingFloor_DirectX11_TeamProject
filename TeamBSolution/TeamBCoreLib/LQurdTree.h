#pragma once
#include "LNode.h"
#include "LObject.h"

class LQurdtree : public LObject
{
public:
	// map의 행렬을 node마다 계산하기 위한 요소들
	LMap* m_pMap = nullptr;
	DWORD m_Row = 0;
	DWORD m_Col = 0;
	DWORD m_RowCellSize = 0;
	DWORD m_ColCellSize = 0;
	std::vector<LNode*> m_NodeList;
	std::vector<LNode*> m_LeafNodeList;
	std::vector<std::vector<DWORD>> m_LeafIndexList;
public:
	LNode* m_RootNode = nullptr;
	int m_TreeDepth = 0;
	std::queue<LNode*> m_Queue;
public:
	void SetDepth(int depth);
public:
	LNode* NewNode(LNode* pParnet, DWORD topLeft, DWORD topRight, DWORD bottomRight, DWORD bottomLeft);
	void BuildTree(LNode* pNode);
	void BuildQurdTree(LMap* pMap, DWORD row, DWORD col);
	TVector2 GetHeightFormNode(LNode* pNode);
	void ComputeBoundingBox(LNode* pNode);
	void FindNode(LNode* pNode);
	void AddLeafNode(LNode* pNode);
	void UpdateIndexBuffer();
public:
	bool Init() override;
	bool Frame() override;
	bool Render() override;
	bool Release() override;
public:
	virtual ~LQurdtree();
};




