#include "LFbxImport.h"

TMatrix LFbxImport::ConvertAMatrix(FbxMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);

	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}

	return mat;
}

TMatrix LFbxImport::ConvertAMatrix(FbxAMatrix& m)
{
	TMatrix mat;
	float* pMatArray = reinterpret_cast<float*>(&mat);
	double* pSrcArray = reinterpret_cast<double*>(&m);

	for (int i = 0; i < 16; i++)
	{
		pMatArray[i] = pSrcArray[i];
	}

	return mat;
}

TMatrix LFbxImport::DxConvertMatrix(TMatrix& m)
{
	TMatrix mat;
	mat._11 = m._11; mat._12 = m._13; mat._13 = m._12;
	mat._21 = m._31; mat._22 = m._33; mat._23 = m._32;
	mat._31 = m._21; mat._32 = m._23; mat._33 = m._22;
	mat._41 = m._41; mat._42 = m._43; mat._43 = m._42;
	mat._14 = mat._24 = mat._34 = 0.0f; mat._44 = 1.0f;

	return mat;
}

TMatrix LFbxImport::ParseTransform(FbxNode* fbxNode)
{
	FbxTime::EMode TimeMode = FbxTime::GetGlobalTimeMode();
	FbxTime time;
	time.SetFrame(0.0f, TimeMode);
	FbxAMatrix matFbx = fbxNode->EvaluateGlobalTransform(time);
	TMatrix mat = ConvertAMatrix(matFbx);
	mat = DxConvertMatrix(mat);

	return mat;
}

bool LFbxImport::Load(std::wstring fbxFilePath, LFbxObj* fbxObj)
{
	bool ret = m_pFbxImporter->Initialize(wtm(fbxFilePath).c_str());
	m_pFbxImporter->Import(m_pFbxScene);

	if (!ret)
	{
		MessageBoxA(NULL, "FbxFile Import Error", "Error Box", MB_OK);
		return false;
	}

	m_RootNode = m_pFbxScene->GetRootNode();

	if (m_RootNode == nullptr)
	{
		MessageBoxA(NULL, "FbxRootNode is Null", "Error Box", MB_OK);
		return false;
	}

	PreProcess(m_RootNode, fbxObj);

	// 파일 이름만 빠진 경로를 뽑아냄 path에 저장되어 있음
	std::size_t found = fbxFilePath.find_last_of(L"/");
	std::wstring path = fbxFilePath.substr(0, found + 1);

	for (int i = 0; i < m_MeshNodeList.size(); i++)
	{
		std::shared_ptr<LFbxObj> fbxMesh = m_MeshNodeList[i];
		fbxMesh->m_DefaultFilePath = path;
		//auto iter = m_pFbxNodeNameMap.find(m_MeshNodeList[i]);
		// 메쉬가 있는 노드에서 메쉬 관련된 정보를 뽑아냄
		MeshLoad(m_pNodeList[fbxMesh->m_iBoneIndex], fbxMesh.get());

		fbxObj->m_DrawList.push_back(fbxMesh);
	}

	GetAnimation(fbxObj);

	return true;
}

void LFbxImport::PreProcess(FbxNode* fbxNode, LFbxObj* fbxObj)
{
	if (fbxNode == nullptr) return;
	if (fbxNode->GetCamera() || fbxNode->GetLight()) return;

	std::shared_ptr<LFbxObj> fbxChildObj = std::make_shared<LFbxObj>();
	fbxChildObj->m_Name = mtw(fbxNode->GetName());
	fbxChildObj->m_iBoneType = LFbxObj::BONE_NODE;
	fbxChildObj->m_iBoneIndex = fbxObj->m_TreeList.size();

	fbxObj->m_TreeList.push_back(fbxChildObj);
	fbxObj->m_pFbxNodeNameList.push_back(fbxChildObj->m_Name);
	fbxObj->m_pFbxNodeNameMap.insert(std::make_pair(fbxChildObj->m_Name, fbxChildObj->m_iBoneIndex));

	m_pNodeList.push_back(fbxNode);
	m_pFbxNodeNameMap.insert(std::make_pair(fbxNode, fbxChildObj->m_iBoneIndex));

	if (fbxNode->GetMesh())
	{
		fbxChildObj->m_iBoneType = LFbxObj::MESH_NODE;
		m_MeshNodeList.push_back(fbxChildObj);
	}

	for (int i = 0; i < fbxNode->GetChildCount(); i++)
	{
		PreProcess(fbxNode->GetChild(i), fbxObj);
	}
}

void LFbxImport::MeshLoad(FbxNode* fbxNode, LFbxObj* lMesh)
{
	// 메쉬가 있는 노드에서 메쉬를 가지고 옴
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	// 메쉬가 가지고 있는 폴리곤의 총 갯수
	int iPolyCount = fbxMesh->GetPolygonCount();

	lMesh->m_bSkinning = ParseMeshSkinning(fbxMesh, lMesh);
	lMesh->m_iBoneType = LFbxObj::SKIN_NODE;


	// 제어점의 배열을 가지고 옴
	FbxVector4* fbxControlPointList = fbxMesh->GetControlPoints();
	// 레이어들의 반복을 돌릴 레이어 카운터
	int iLayerCount = fbxMesh->GetLayerCount();
	// 메쉬가 가지고 있는 재질의 가짓수 사실상 이것대로 정점을 분할해서 넣을것이기 때문에 SubMaterial갯수라고 봐도 상관없다.
	int iSubMaterialCount = fbxNode->GetMaterialCount();
	// 서브메테리얼 인덱스가 담길 공간 기본값은 0으로 하나의 공간에 담긴다.
	int iSubMaterialIndex = 0;

	std::vector<FbxLayerElementNormal*> NormalLayerList;
	std::vector<FbxLayerElementVertexColor*> ColorLayerList;
	std::vector<FbxLayerElementUV*> UVLayerList;
	std::vector<FbxLayerElementMaterial*> MaterialLayerList;

	// Mesh가 가지고 있는 레이어 각각의 레이어 배열 뽑아내기
	for (int iLayer = 0; iLayer < iLayerCount; iLayer++)
	{
		FbxLayer* fbxLayer = fbxMesh->GetLayer(iLayer);

		if (fbxLayer->GetNormals() != nullptr)
		{
			NormalLayerList.push_back(fbxLayer->GetNormals());
		}

		if (fbxLayer->GetVertexColors() != nullptr)
		{
			ColorLayerList.push_back(fbxLayer->GetVertexColors());
		}

		if (fbxLayer->GetUVs() != nullptr)
		{
			UVLayerList.push_back(fbxLayer->GetUVs());
		}

		if (fbxLayer->GetMaterials() != nullptr)
		{
			MaterialLayerList.push_back(fbxLayer->GetMaterials());
		}
	}

	// Mesh의 재질이 여러개면 각각의 정점 리스트에 넣는다.
	if (iSubMaterialCount > 0)
	{
		lMesh->m_TriangleList.resize(iSubMaterialCount);
		lMesh->m_pSubVertexListIW.resize(iSubMaterialCount);
	}

	// 텍스처 파일의 이름을 불러오고 그걸 기반으로 텍스처 배열도 로드한다.
	for (int iSub = 0; iSub < iSubMaterialCount; iSub++)
	{
		std::string filePath;
		FbxSurfaceMaterial* surfaceMaterial = fbxNode->GetMaterial(iSub);
		filePath = ParseMaterial(surfaceMaterial);

		char Drive[MAX_PATH];
		char Dir[MAX_PATH];
		char FName[MAX_PATH];
		char Ext[MAX_PATH];

		_splitpath_s(filePath.c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

		std::string texName = FName;
		texName += Ext;

		lMesh->m_TexFileNameList.push_back(mtw(texName));
	}

	// 행렬 정보를 불러온다. 행렬 정보는 local정점 x 자신과부모의 변환행렬 x 기하학적 행렬
	// 자신과 부모의 변환행렬은 ParseTransform 에서 기하학적 행렬은 바로 아래에서 뽑아낸다.
	FbxAMatrix GeometircMatrix;
	FbxAMatrix normalMatrix;
	{
		FbxVector4 s = fbxNode->GetGeometricScaling(FbxNode::eSourcePivot);
		FbxVector4 r = fbxNode->GetGeometricRotation(FbxNode::eSourcePivot);
		FbxVector4 t = fbxNode->GetGeometricTranslation(FbxNode::eSourcePivot);

		GeometircMatrix.SetS(s);
		GeometircMatrix.SetR(r);
		GeometircMatrix.SetT(t);

		normalMatrix = GeometircMatrix;
		normalMatrix = normalMatrix.Inverse();
		normalMatrix = normalMatrix.Transpose();
	}

	// 제어점 기반이 아니라 폴리곤의 정점 기반 계산이다.
	// 삼각형 기반 cell이 9개일 경우 9 x 2 x 3 = 54개의 정점 계산
	// 폴리곤의 숫자, 삼각형의 숫자, 삼각형의 정점의 숫자
	int iBasePolyIndex = 0;
	for (int iPoly = 0; iPoly < iPolyCount; iPoly++)
	{
		int iPolySize = fbxMesh->GetPolygonSize(iPoly);
		int iTriangleCount = iPolySize - 2;
		// 폴리곤에서 해당 재질의 인덱스를 가져온다.
		iSubMaterialIndex = GetSubMaterialIndex(MaterialLayerList[0], iPoly);

		// 폴리곤 총 갯수의 정보를 저장
		lMesh->m_NumPoly += iPolySize;
		iBasePolyIndex += iPolySize;
		for (int iTriangle = 0; iTriangle < iTriangleCount; iTriangle++)
		{
			int CornerIndices[3];
			CornerIndices[0] = fbxMesh->GetPolygonVertex(iPoly, 0);
			CornerIndices[1] = fbxMesh->GetPolygonVertex(iPoly, iTriangle + 2);
			CornerIndices[2] = fbxMesh->GetPolygonVertex(iPoly, iTriangle + 1);

			int uv[3];
			uv[0] = fbxMesh->GetTextureUVIndex(iPoly, 0);
			uv[1] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 2);
			uv[2] = fbxMesh->GetTextureUVIndex(iPoly, iTriangle + 1);

			int iPolyIndex[3] = { 0, iTriangle + 2, iTriangle + 1 };

			for (int iVertex = 0; iVertex < 3; iVertex++)
			{
				SimpleVertex pnct;

				int iDCIndex = CornerIndices[iVertex];
				FbxVector4 v = fbxControlPointList[iDCIndex];
				FbxVector4 finalPos = GeometircMatrix.MultT(v);
				pnct.p.x = finalPos.mData[0];
				pnct.p.y = finalPos.mData[2];
				pnct.p.z = finalPos.mData[1];

				if (NormalLayerList.size() > 0)
				{
					FbxVector4 normal = { 0.0f, 0.0f, 0.0f };
					normal = ReadNormal(NormalLayerList[0], iDCIndex, uv[iVertex] + iBasePolyIndex);

					pnct.n.x = normal.mData[0];
					pnct.n.y = normal.mData[2];
					pnct.n.z = normal.mData[1];
				}

				if (ColorLayerList.size() > 0)
				{
					FbxColor color = { 1.0f, 1.0f, 1.0f, 1.0f };
					color = ReadColor(ColorLayerList[0], iDCIndex, uv[iVertex] + iBasePolyIndex);

					pnct.c.x = color.mRed;
					pnct.c.y = color.mGreen;
					pnct.c.z = color.mBlue;
					pnct.c.w = 1.0f;
				}

				FbxVector2 textureUV = ReadTextureCoord(UVLayerList[0], iDCIndex, uv[iVertex]);
				pnct.t.x = textureUV.mData[0];
				pnct.t.y = 1.0f - textureUV.mData[1];

				lMesh->m_TriangleList[iSubMaterialIndex].push_back(pnct);

				LVertexIW iwVertex;

				FbxGeometryElementTangent* vertexTangent = fbxMesh->GetElementTangent(0);
				iwVertex.tan = { 0, 0, 0 };

				if (lMesh->m_bSkinning)
				{
					LWeight* weight = &lMesh->m_WeightList[iDCIndex];

					for (int i = 0; i < 4; i++)
					{
						iwVertex.i[i] = weight->index[i];
						iwVertex.w[i] = weight->weight[i];
					}
				}
				else
				{
					iwVertex.i[0] = lMesh->m_iBoneIndex;
					iwVertex.w[0] = 1.0f;
				}
				lMesh->m_pSubVertexListIW[iSubMaterialIndex].push_back(iwVertex);
			}
		}
	}
}

bool LFbxImport::Init()
{
	m_pFbxManager = FbxManager::Create();
	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, IOSROOT);
	m_pFbxScene = FbxScene::Create(m_pFbxManager, "");
	return true;
}

bool LFbxImport::Frame()
{
	return true;
}

bool LFbxImport::Render()
{
	return true;
}

bool LFbxImport::Release()
{
	m_RootNode->Destroy();
	m_pFbxScene->Destroy();
	m_pFbxImporter->Destroy();
	m_pFbxManager->Destroy();

	m_RootNode = nullptr;
	m_pFbxScene = nullptr;
	m_pFbxImporter = nullptr;
	m_pFbxManager = nullptr;

	m_MeshNodeList.clear();
	m_pNodeList.clear();
	m_pFbxNodeNameMap.clear();

	return true;
}
