#include "LFbxImport.h"

bool LFbxImport::ParseMeshSkinning(FbxMesh* pFbxMesh, LFbxObj* pMesh)
{
	// �����ӽ�Ų �����ӵ ������ �ִ� ��Ų ���
	// Ŭ�������� ���� ����ȣ��
	// ��Ų �����Ӱ� �ϳ���� �̾߱�� �޽� ����� �ϳ��� ��Ų �����ӿ� ���� ����ȴ�.
	// ��Ų �����Ӵ� �޽��� �� ������ �ϳ� �̻��� ���� �����մϴ�.
	// ���� �޽��� �������� ���� ���� �� �ֽ��ϴ�.
	int iDeformerCount = pFbxMesh->GetDeformerCount(FbxDeformer::eSkin);

	if (iDeformerCount == 0)
	{
		return false;
	}

	// ������ �� ������ ������ ����. 
	int iVertexCount = pFbxMesh->GetControlPointsCount();
	pMesh->m_WeightList.resize(iVertexCount);

	for (int iDeformerIndex = 0; iDeformerIndex < iDeformerCount; iDeformerIndex++)
	{
		FbxSkin* pSkin = 
		reinterpret_cast<FbxSkin*>(pFbxMesh->GetDeformer(iDeformerIndex, FbxDeformer::eSkin));

		// Ŭ�������� ��� �ϳ��� ���� ������ �ִ� �޽������ �Դϴ�.
		DWORD dwClusterCount = pSkin->GetClusterCount();

		for (int iClusterIndex = 0; iClusterIndex < dwClusterCount; iClusterIndex++)
		{
			FbxCluster* pCluster = pSkin->GetCluster(iClusterIndex);

			FbxAMatrix maxXBindPose;
			FbxAMatrix matReferenceGlobalInitPosition;
			// ���� Ŭ�����Ϳ� ���ε� �Ǿ��� ��
			// ����� ���� ��ȯ ��Ʈ���� ��ȯ
			pCluster->GetTransformLinkMatrix(maxXBindPose);
			// Ŭ�����Ͱ� ó�� ������ �� �޽��� ��ȯ ��Ʈ����
			pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
			FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * maxXBindPose;

			int iBoneIndex = 0;
			TMatrix matInvBindPos = ConvertAMatrix(matBindPose);
			matInvBindPos = DxConvertMatrix(matInvBindPos);
			matInvBindPos = matInvBindPos.Invert();

			// ��� fbxnode�� �ʿ� �����صξ���
			// Ŭ�����ʹ� �� �ϳ��� ������ �������� �����ϰ� �ֱ� ������
			// pCluster->GetLink() ��ɾ �̿��ϸ� ���õ� ���� fbxnode �����͸�
			// ��ȯ���� �� �ִ�.
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
				// ���� ���õ� Ŭ�����Ϳ� ���õ� ���� �ε����� ����ġ�� ������ �� �ִ�.
				pMesh->m_WeightList[iVertexIndex].InsertWeight(iBoneIndex, fWeight);
			}
		}
	}

	return true;
}