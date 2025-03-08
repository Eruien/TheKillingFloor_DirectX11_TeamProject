#include "LFbxImport.h"

void LFbxImport::GetAnimation(LFbxObj* fbxObj)
{
	FbxTime::SetGlobalTimeMode(FbxTime::eFrames60);
	FbxAnimStack* stack = m_pFbxScene->GetSrcObject<FbxAnimStack>(0);

	if (stack == nullptr)
	{
		//MessageBoxA(NULL, "AnimationStack is Null", "Error Box", MB_OK);
		return;
	}

	FbxString takeName = stack->GetName();
	FbxTakeInfo* takeInfo = m_pFbxScene->GetTakeInfo(takeName);
	FbxTimeSpan ts = takeInfo->mLocalTimeSpan;
	FbxTime startTime = ts.GetStart();
	FbxTime endTime = ts.GetStop();

	FbxTime::EMode timeMode = FbxTime::GetGlobalTimeMode();
	FbxLongLong startFrame = startTime.GetFrameCount(timeMode);
	FbxLongLong endFrame = endTime.GetFrameCount(timeMode);

	fbxObj->SetAnimation(startFrame, endFrame, 60, 160);

	FbxTime time;
	fbxObj->m_NameMatrixMap.resize(endFrame);
	for (int t = startFrame; t < endFrame; t++)
	{
		time.SetFrame(t, timeMode);
		std::vector<TMatrix> matArray;
		for (int iMesh = 0; iMesh < fbxObj->m_TreeList.size(); iMesh++)
		{

			// 초당 30프레임 기준으로 프레임1당 fbxMatrix
			// Mesh당 행렬을 얻는다.
			FbxAMatrix fbxMat = m_pNodeList[iMesh]->EvaluateGlobalTransform(time);
			TMatrix mat = ConvertAMatrix(fbxMat);
			mat = DxConvertMatrix(mat);
			matArray.push_back(mat);

			std::wstring nodeName = mtw(m_pNodeList[iMesh]->GetName());
			fbxObj->m_NameMatrixMap[t].insert(std::make_pair(nodeName, mat));
		}
		fbxObj->m_MatrixArray.push_back(matArray);
	}
}