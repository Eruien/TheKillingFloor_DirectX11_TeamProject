#pragma once
#include "LObject.h"
#include "LMap.h"

class LNode : public LObject
{
public:
	// index ��, ���� ũ�Ⱑ ������ ����ϱ� ���� ���
	DWORD m_RowCellSize = 0;
	DWORD m_ColCellSize = 0;
	// index ����� ����
	std::vector<DWORD> m_Corner;
	// Frustum ����� ���� tbox
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

