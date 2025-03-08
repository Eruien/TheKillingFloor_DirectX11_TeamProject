#include "LFbxImport.h"

FbxVector2 LFbxImport::ReadTextureCoord(FbxLayerElementUV* layer, int iVertexIndex, int iIndex)
{
	FbxVector2 texture;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementUV::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		{
			texture = layer->GetDirectArray().GetAt(iVertexIndex);
		}
		break;

		case FbxLayerElementUV::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			texture = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;

	case FbxLayerElementUV::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementUV::eDirect:
		case FbxLayerElementUV::eIndexToDirect:
		{
			texture = layer->GetDirectArray().GetAt(iIndex);
		}
		break;
		}
	}
	break;

	}

	return texture;
}

FbxColor LFbxImport::ReadColor(FbxLayerElementVertexColor* layer, int iVertexIndex, int iIndex)
{
	FbxColor color;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementVertexColor::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			color = layer->GetDirectArray().GetAt(iVertexIndex);
		}
		break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			color = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxLayerElementVertexColor::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementVertexColor::eDirect:
		{
			color = layer->GetDirectArray().GetAt(iIndex);
		}
		break;
		case FbxLayerElementVertexColor::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			color = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return color;
}

FbxVector4 LFbxImport::ReadNormal(FbxLayerElementNormal* layer, int iVertexIndex, int iIndex)
{
	FbxVector4 normal;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementNormal::eByControlPoint:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementNormal::eDirect:
		{
			normal = layer->GetDirectArray().GetAt(iVertexIndex);
		}
		break;
		case FbxLayerElementNormal::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iVertexIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	case FbxLayerElementNormal::eByPolygonVertex:
	{
		switch (layer->GetReferenceMode())
		{
		case FbxLayerElementNormal::eDirect:
		{
			normal = layer->GetDirectArray().GetAt(iIndex);
		}
		break;
		case FbxLayerElementNormal::eIndexToDirect:
		{
			int id = layer->GetIndexArray().GetAt(iIndex);
			normal = layer->GetDirectArray().GetAt(id);
		}
		break;
		}
	}
	break;
	}

	return normal;
}

std::string LFbxImport::ParseMaterial(FbxSurfaceMaterial* pSurfaceMaterial)
{
	std::string materialFullPath;

	FbxProperty fbxProperty = pSurfaceMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);

	if (!fbxProperty.IsValid())
	{
		MessageBoxA(NULL, "FindProperty Error", "Error Box", MB_OK);

		return materialFullPath;
	}

	FbxFileTexture* texFile = fbxProperty.GetSrcObject<FbxFileTexture>(0);

	if (texFile != nullptr)
	{
		materialFullPath = texFile->GetFileName();

		return materialFullPath;
	}

	return materialFullPath;
}

int LFbxImport::GetSubMaterialIndex(FbxLayerElementMaterial* layer, int iPoly)
{
	int SubMaterialIndex = 0;

	switch (layer->GetMappingMode())
	{
	case FbxLayerElementMaterial::eByPolygon:
	{
		switch (layer->GetReferenceMode())
		{
			// eDirect 폐기됨 eDirect 말고 eByPolygon의 eIndexToDirect를 사용할것 해당 폴리곤을 넣으면 서브메터리얼이 바로 나옴
		case FbxLayerElementMaterial::eDirect:
		case FbxLayerElementMaterial::eIndexToDirect:
		{
			SubMaterialIndex = layer->GetIndexArray().GetAt(iPoly);
		}
		break;
		}
	}
	break;
	}

	return SubMaterialIndex;
}