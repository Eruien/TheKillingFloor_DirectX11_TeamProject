#pragma once
#include "LObject.h"
#include "LMap.h"

class LNode : public LObject
{
public:
	// index 행, 열의 크기가 얼마인지 계산하기 위한 요소
	DWORD m_RowCellSize = 0;
	DWORD m_ColCellSize = 0;
	// index 행렬의 끝점
	std::vector<DWORD> m_Corner;
	// Frustum 계산을 위한 tbox
	T_BOX  m_tBox;
public:
	int m_iDepth = 0;
	bool m_IsLeaf = false;
	LNode* m_pParent = nullptr;
	std::vector<LNode*> m_pChild;
public:
	void SetParent(LNode* pParent);
public:
	virtual ~LNode();
};

