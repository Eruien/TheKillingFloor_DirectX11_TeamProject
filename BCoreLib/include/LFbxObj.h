#pragma once
#include "LObject.h"

struct LBoneWorld
{
	TMatrix matBoneWorld[200];
};

struct LVertexIW
{
	float i[4];
	float w[4];
	TVector3 tan;

	LVertexIW()
	{
		i[0] = i[1] = i[2] = i[3] = 0.0f;
		w[0] = w[1] = w[2] = w[3] = 0.0f;
	}
};

struct InstanceMatrix
{
	TMatrix mat[255];
};

struct LWeight
{
	std::vector<int> index;
	std::vector<float> weight;

	void InsertWeight(int iBoneIndex, float fBoneWeight)
	{
		for (DWORD i = 0; i < index.size(); i++)
		{
			if (fBoneWeight > weight[i])
			{
				for (DWORD j = (index.size() - 1); j > i; --j)
				{
					index[j] = index[j - 1];
					weight[j] = weight[j - 1];
				}
				index[i] = iBoneIndex;
				weight[i] = fBoneWeight;
				break;
			}
		}
	}

	LWeight()
	{
		index.resize(8);
		weight.resize(8);
	}
};

class LFbxObj : public LObject
{
public:
	enum { BONE_NODE, MESH_NODE, SKIN_NODE, };
public:
	std::wstring m_Name;
	LFbxObj* m_pParent = nullptr;
	bool m_bSkinning = false;
	int m_iBoneType = -1;
	int m_iBoneIndex = -1;
	int m_NumPoly = 0;
	int m_AnimationTreeSize = 0;
	int m_SubMtrlCount = 0;
	TMatrix m_MatWorld;
	std::wstring m_DefaultFilePath;
	ComPtr<ID3D11Buffer> m_pVBWeightList = nullptr;
	TMatrix m_matSocket;
	TMatrix m_matScale;
	TMatrix m_matRotation;
	TMatrix m_matTranslation;
public:
	int m_iStartFrame = 0;
	int m_iEndFrame = 0;
	int m_iFrameSpeed = 60;
	int m_iTickForFrame = 160;
public:
	InstanceMatrix m_matInstanceList;
	ComPtr<ID3D11Buffer> m_pCBInstance;
	int m_InstanceSize = 0;
	bool m_bInstancing = false;
public:
	std::map<std::wstring, int> m_pFbxNodeNameMap;
	std::vector<std::wstring> m_pFbxNodeNameList;
public:
	// fbxNode 전체
	std::vector<std::shared_ptr<LFbxObj>> m_TreeList;
	// Mesh가 들어있는 fbxNode
	std::vector<std::shared_ptr<LFbxObj>> m_DrawList;
	// 매쉬마다 가지고 있는 정점리스트 -> VertexList로 합친다.
	std::vector<std::vector<SimpleVertex>> m_TriangleList;
	// 매쉬바다 가지고 있는 IW
	std::vector<std::vector<LVertexIW>> m_pSubVertexListIW;
	// IW리스트에 합친다.
	std::vector<LVertexIW> m_pVertexListIW;
	std::vector<UINT> m_TriangleOffsetList;
	std::vector<std::wstring> m_TexFileNameList;
	std::vector<LTexture*> m_TexArray;
	std::vector<std::vector<TMatrix>> m_MatrixArray;
	std::vector<std::map<std::wstring, TMatrix>> m_NameMatrixMap;
	std::vector<LWeight> m_WeightList;
	std::vector<int> m_SubMtrlVertexList;
	std::map<std::wstring, TMatrix> m_dxMatrixBindPoseMap;
public:
	void SetInstancing(bool IsIntancing);
	void SetAnimation(int startFrame, int endFrame, int frameSpeed, int tickForFrame);
	void SetDefaultFilePath(std::wstring filePath);
	void LoadTexture(std::wstring texFileName);
public:
	bool CreateVertexData() override;
	bool CreateIndexData() override;
	bool CreateVertexBuffer() override;
	bool CreateLayout() override;
	bool CreateInstance(int numInstance);
	bool CreateInstanceBuffer();
	bool PostRender() override;

};

