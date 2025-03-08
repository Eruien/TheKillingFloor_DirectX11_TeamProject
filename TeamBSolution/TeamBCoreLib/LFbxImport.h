#pragma once
#include "LObject.h"
#include "LFbxObj.h"
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

class LFbxImport
{
	FbxManager* m_pFbxManager = nullptr;
	FbxImporter* m_pFbxImporter = nullptr;
	FbxScene* m_pFbxScene = nullptr;
	FbxNode* m_RootNode = nullptr;
	std::vector<std::shared_ptr<LFbxObj>> m_MeshNodeList;
	std::vector<FbxNode*> m_pNodeList;
	std::map<FbxNode*, int> m_pFbxNodeNameMap;
public:
	void GetAnimation(LFbxObj* fbxObj);
public:
	FbxVector2 ReadTextureCoord(FbxLayerElementUV* layer, int iVertexIndex, int iIndex);
	FbxColor ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex, int iIndex);
	FbxVector4 ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex);
	std::string ParseMaterial(FbxSurfaceMaterial* pMaterial);
	int GetSubMaterialIndex(FbxLayerElementMaterial* layer, int iPoly);
public:
	TMatrix ConvertAMatrix(FbxMatrix& m);
	TMatrix ConvertAMatrix(FbxAMatrix& m);
	TMatrix DxConvertMatrix(TMatrix& m);
	TMatrix ParseTransform(FbxNode* fbxNode);
public:
	bool ParseMeshSkinning(FbxMesh* pFbxMesh, LFbxObj* pMesh);
public:
	bool Load(std::wstring fbxFilePath, LFbxObj* fbxObj);
	void PreProcess(FbxNode* fbxNode, LFbxObj* fbxObj);
	void MeshLoad(FbxNode* fbxNode, LFbxObj* lMesh);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
};