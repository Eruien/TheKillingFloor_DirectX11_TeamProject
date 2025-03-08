#include "LFbxImport.h"

bool LFbxImport::ParseMeshSkinning(FbxMesh* pFbxMesh, LFbxObj* pMesh)
{
	// 디포머스킨 움직임등에 영향을 주는 스킨 덩어리
	// 클러스터의 상위 상위호완
	// 스킨 디포머가 하나라는 이야기는 메쉬 덩어리가 하나의 스킨 디포머에 의해 제어된다.
	// 스킨 디포머는 메시의 각 정점을 하나 이상의 본에 연결합니다.
	// 따라서 메쉬는 여러개의 본에 붙을 수 있습니다.
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (iDeformerCount == 0)
	{
		return false;
	}

	// 제어점 총 정점의 갯수와 같다. 
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	pMesh->m_WeightList.resize(iVertexCount);

	for (int iDeformerIndex = 0; iDeformerIndex < iDeformerCount; iDeformerIndex++)
	{
		FbxSkin* pSkin = 
		reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(iDeformerIndex, FbxDeformer::eSkin));

		// 클러스터의 경우 하나의 본에 영향을 주는 메쉬덩어리들 입니다.
		DWORD dwClusterCount = pSkin->GetClusterCount();

		for (int iClusterIndex = 0; iClusterIndex < dwClusterCount; iClusterIndex++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iClusterIndex);

			FbxAMatrix maxXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			// 본이 클러스터에 바인딩 되었을 때
			// 연결된 본의 변환 매트릭스 반환
			pCluster->GetTransformLinkMatrix(maxXBindPose);
			// 클러스터가 처음 생성될 때 메시의 변환 매트릭스
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * maxXBindPose;

			int iBoneIndex = 0;
			TMatrix matInvBindPos = ConvertAMatrix(matBindPose);
			matInvBindPos = DxConvertMatrix(matInvBindPos);
			matInvBindPos = matInvBindPos.Invert();

			// 모든 fbxnode를 맵에 저장해두었다
			// 클러스터는 본 하나에 연관된 정점들을 저장하고 있기 때문에
			// pCluster->GetLink() 명령어를 이용하면 관련된 본의 fbxnode 포인터를
			// 반환받을 수 있다.
			iBoneIndex = m_pFbxNodeNameMap.find(pCluster->GetLink())->second;
			std::wstring name = mtw(pCluster->GetLink()->GetName());
			pMesh->m_dxMatrixBindPoseMap.insert(make_pair(name, matInvBindPos));

			int iClusterSize = pCluster->GetControlPointIndicesCount();
			int* pVertexIndices = pCluster->GetControlPointIndices();
			double* pWeights = pCluster->GetControlPointWeights();

			for (int i = 0; i < iClusterSize; i++)
			{
				int iVertexIndex = pVertexIndices[i];
				float fWeight = pWeights[i];
				// 따라서 관련된 클러스터에 관련된 본의 인덱스와 가중치를 저장할 수 있다.
				pMesh->m_WeightList[iVertexIndex].InsertWeight(iBoneIndex, fWeight);
			}
		}
	}

	return true;
}