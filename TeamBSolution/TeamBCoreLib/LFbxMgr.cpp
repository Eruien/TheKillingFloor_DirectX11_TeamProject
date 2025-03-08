#include "LFbxMgr.h"

LFbxObj* LFbxMgr::Load(std::wstring filePath, std::wstring shaderFilePath)
{
	std::size_t found = filePath.find_last_of(L"/");
	std::wstring path = filePath.substr(0, found + 1);
	std::wstring key = filePath.substr(found + 1);

	LFbxObj* data = GetPtr(key);

	if (data != nullptr)
	{
		return data;
	}

	LFbxObj* ret = nullptr;

	std::shared_ptr<LFbxObj> managerType = std::make_shared<LFbxObj>();
	m_FbxImport.Init();
	if (m_FbxImport.Load(filePath, managerType.get()))
	{
		managerType->Set();

		for (int iSub = 0; iSub < managerType->m_DrawList.size(); iSub++)
		{
			LFbxObj* fbxMesh = managerType->m_DrawList[iSub].get();
			fbxMesh->Set();
			fbxMesh->m_VertexList.resize(fbxMesh->m_NumPoly * 3);
			fbxMesh->m_pVertexListIW.resize(fbxMesh->m_NumPoly * 3);
			UINT iNumSubMaterial = fbxMesh->m_TriangleList.size();

			UINT iSubVertexIndex = 0;
			// 트라이앵글리스트의 수만큼 순환
			for (int iMtrl = 0; iMtrl < iNumSubMaterial; iMtrl++)
			{
				fbxMesh->m_TriangleOffsetList.push_back(iSubVertexIndex);

				// 해당하는 자식의 VertexList에 mesh의 정점을 전달해줌
				for (int iVertex = 0; iVertex < fbxMesh->m_TriangleList[iMtrl].size(); iVertex++)
				{
					fbxMesh->m_VertexList[iVertex + iSubVertexIndex] = fbxMesh->m_TriangleList[iMtrl][iVertex];
					fbxMesh->m_pVertexListIW[iVertex + iSubVertexIndex] = fbxMesh->m_pSubVertexListIW[iMtrl][iVertex];
				}

				iSubVertexIndex += fbxMesh->m_TriangleList[iMtrl].size();
			}

			std::wstring defaultFilePath = fbxMesh->m_DefaultFilePath;
			std::wstring fileName;
			if (fbxMesh->m_TexFileNameList.size() > 1)
			{
				fileName = fbxMesh->m_TexFileNameList[0];
				defaultFilePath += fileName;
				fbxMesh->Create(shaderFilePath, defaultFilePath);
			}
			else
			{
				fbxMesh->Create(shaderFilePath, L"../../res/map/basecolor.jpg"); 
			}

			fbxMesh->m_MatWorld = managerType->m_MatWorld;

			for (int i = 0; i < fbxMesh->m_TexFileNameList.size(); i++)
			{
				std::wstring textureName = fbxMesh->m_DefaultFilePath;
				textureName += fbxMesh->m_TexFileNameList[i];
				fbxMesh->LoadTexture(textureName);
			}

			fbxMesh->m_SubMtrlCount = fbxMesh->m_TexFileNameList.size();
			fbxMesh->m_SubMtrlVertexList.resize(fbxMesh->m_SubMtrlCount);

			for (int i = 0; i < fbxMesh->m_TriangleList.size(); i++)
			{
				fbxMesh->m_SubMtrlVertexList[i] = fbxMesh->m_TriangleList[i].size();
			}
		}

		managerType->m_AnimationTreeSize = managerType->m_TreeList.size();
		m_map.insert(std::make_pair(key, managerType));
		ret = managerType.get();
	}

	m_FbxImport.Release();

	return ret;
}

LFbxObj* LFbxMgr::GetPtr(std::wstring key)
{
	auto iter = m_map.find(key);

	if (iter == m_map.end())
	{
		return nullptr;
	}

	return iter->second.get();
}