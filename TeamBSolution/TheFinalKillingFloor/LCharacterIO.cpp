#include "LCharacterIO.h"

void LCharacterIO::CharacterReset()
{
    ZeroMemory(&m_CharacterHeader, sizeof(m_CharacterHeader));
    ZeroMemory(&m_CharacterForm, sizeof(m_CharacterForm));
    ZeroMemory(&m_VertexHeader, sizeof(m_VertexHeader));
    ZeroMemory(&m_VertexForm, sizeof(m_VertexForm));
    ZeroMemory(&m_MeshHeader, sizeof(m_MeshHeader));
    ZeroMemory(&m_MeshForm, sizeof(m_MeshForm));
    ZeroMemory(&m_HierarchyHeaader, sizeof(m_HierarchyHeaader));
    ZeroMemory(&m_HierarchyForm, sizeof(m_HierarchyForm));
    iWritePos = 0;
    iReadPos = 0;
}

void LCharacterIO::WriteCharacterData(const char* data, int dataSize, void* pPos, int& iPos, char* changePtr)
{
    pPos = &changePtr[iPos];
    CopyMemory(pPos, data, dataSize);
    iPos += dataSize;

    return;
}

void LCharacterIO::ReadCharacterData(char* buffer, int bufferSize, void* pReadPos, int& iReadPos, char* changePtr)
{
    pReadPos = &changePtr[iReadPos];
    CopyMemory(buffer, pReadPos, bufferSize);
    iReadPos += bufferSize;

    return;
}

int LCharacterIO::ComputeVertexFormSize(int vertexListSize, int formSize)
{
    return (vertexListSize / formSize) + 1;
}

bool LCharacterIO::ItemWrite(std::wstring fbxFilePath)
{
    LFbxObj* fbxObj = nullptr;
    fbxObj = LFbxMgr::GetInstance().Load(fbxFilePath, L"../../res/hlsl/LightShadowMap.hlsl");

    CharacterReset();

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(fbxFilePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

    std::string saveFilePath = "../../res/UserFile/Item/";
    saveFilePath += FName;
    saveFilePath += ".bin";

    m_WriteFile.open(saveFilePath, std::ios::binary);

    m_HierarchyHeaader.treeSize = fbxObj->m_NameMatrixMap[0].size();
    m_WriteFile.write(reinterpret_cast<char*>(&m_HierarchyHeaader), sizeof(m_HierarchyHeaader));

    int count = 0;
    for (auto iter = fbxObj->m_NameMatrixMap[0].begin(); iter != fbxObj->m_NameMatrixMap[0].end(); iter++)
    {
        WriteCharacterData(wtm(iter->first).c_str(),
            m_TextMaxSize, pWritePos, iWritePos, m_HierarchyForm.textBoneList);
        m_HierarchyForm.matBoneList[count] = iter->second;
        count++;
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_HierarchyForm), sizeof(m_HierarchyForm));

    iWritePos = 0;

    m_CharacterHeader.fbxNodeNameMapSize = fbxObj->m_pFbxNodeNameMap.size();
    m_CharacterHeader.fbxNodeNameListSize = fbxObj->m_pFbxNodeNameList.size();
    m_CharacterHeader.texFileNameListSize = fbxObj->m_DrawList[0]->m_TexFileNameList.size();
    m_CharacterHeader.bindPoseMapSize = fbxObj->m_DrawList[0]->m_dxMatrixBindPoseMap.size();

    /*fbxObj->m_DrawList[0]->m_DefaultFilePath.clear();
    fbxObj->m_DrawList[0]->m_DefaultFilePath = L"../../res/Texture/";*/

    CopyMemory(m_CharacterHeader.defaultPath, wtm(fbxObj->m_DrawList[0]->m_DefaultFilePath).c_str(), 60);

    m_WriteFile.write(reinterpret_cast<char*>(&m_CharacterHeader), sizeof(m_CharacterHeader));

    for (int i = 0; i < fbxObj->m_pFbxNodeNameList.size(); i++)
    {
        WriteCharacterData(wtm(fbxObj->m_pFbxNodeNameList[i]).c_str(),
            m_TextMaxSize, pWritePos, iWritePos, m_CharacterForm.fbxNodeNameList);
    }

    iWritePos = 0;

    count = 0;
    for (auto iter = fbxObj->m_pFbxNodeNameMap.begin(); iter != fbxObj->m_pFbxNodeNameMap.end(); iter++)
    {
        WriteCharacterData(wtm(iter->first).c_str(),
            m_TextMaxSize, pWritePos, iWritePos, m_CharacterForm.fbxNodeNameMapwStr);
        m_CharacterForm.fbxNodeNameMapInt[count] = iter->second;
        count++;
    }

    iWritePos = 0;

    for (int i = 0; i < fbxObj->m_DrawList[0]->m_TexFileNameList.size(); i++)
    {
        WriteCharacterData(wtm(fbxObj->m_DrawList[0]->m_TexFileNameList[i]).c_str(),
            40, pWritePos, iWritePos, m_CharacterForm.textFileNameList);
    }

    iWritePos = 0;

    count = 0;
    for (auto iter = fbxObj->m_DrawList[0]->m_dxMatrixBindPoseMap.begin(); iter != fbxObj->m_DrawList[0]->m_dxMatrixBindPoseMap.end(); iter++)
    {
        WriteCharacterData(wtm(iter->first).c_str(), m_TextMaxSize, pWritePos, iWritePos, m_CharacterForm.bindPoseMapStr);
        m_CharacterForm.bindPoseMapMat[count] = iter->second;
        count++;
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_CharacterForm), sizeof(m_CharacterForm));

    m_VertexHeader.vertexFormSize = ComputeVertexFormSize(fbxObj->m_DrawList[0]->m_VertexList.size(), 100);
    m_VertexHeader.vertexListSize = fbxObj->m_DrawList[0]->m_VertexList.size();
    m_WriteFile.write(reinterpret_cast<char*>(&m_VertexHeader), sizeof(m_VertexHeader));

    int vertexIndex = 0;
    for (int i = 0; i < fbxObj->m_DrawList[0]->m_VertexList.size(); i++)
    {
        m_VertexForm.pnct[vertexIndex] = fbxObj->m_DrawList[0]->m_VertexList[i];
        m_VertexForm.iw[vertexIndex] = fbxObj->m_DrawList[0]->m_pVertexListIW[i];

        vertexIndex++;
        if (vertexIndex >= 100)
        {
            m_WriteFile.write(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));
            ZeroMemory(&m_VertexForm, sizeof(m_VertexForm));
            vertexIndex = 0;
            continue;
        }
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));

    m_MeshHeader.subMtrlCount = fbxObj->m_DrawList[0]->m_TexArray.size();
    m_MeshHeader.offsetListSize = fbxObj->m_DrawList[0]->m_TriangleOffsetList.size();

    m_WriteFile.write(reinterpret_cast<char*>(&m_MeshHeader), sizeof(m_MeshHeader));

    for (int i = 0; i < fbxObj->m_DrawList[0]->m_TriangleOffsetList.size(); i++)
    {
        m_MeshForm.OffsetList[i] = fbxObj->m_DrawList[0]->m_TriangleOffsetList[i];
    }

    for (int i = 0; i < fbxObj->m_DrawList[0]->m_TriangleList.size(); i++)
    {
        m_MeshForm.subMtrlVertexSize[i] = fbxObj->m_DrawList[0]->m_TriangleList[i].size();
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_MeshForm), sizeof(m_MeshForm));

    CharacterReset();
    m_WriteFile.close();
    return true;
}

bool LCharacterIO::ItemRead(std::wstring filePath)
{
    m_TextMaxSize = 24;
    m_ReadFile.open(filePath, std::ios::binary);

    if (!m_ReadFile)
    {
        MessageBoxA(NULL, "파일이 존재하지 않습니다.", "Error Box", MB_OK);
        return true;
    }

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(filePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
    std::wstring key = mtw(FName);
    key += L".fbx";

    std::shared_ptr<LFbxObj> managerObj = std::make_shared<LFbxObj>();
    managerObj->m_NameMatrixMap.resize(1);

    CharacterReset();

    m_ReadFile.read(reinterpret_cast<char*>(&m_HierarchyHeaader), sizeof(m_HierarchyHeaader));

    int treeSize = m_HierarchyHeaader.treeSize;

    m_ReadFile.read(reinterpret_cast<char*>(&m_HierarchyForm), sizeof(m_HierarchyForm));

    iReadPos = 0;

    for (int i = 0; i < treeSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_HierarchyForm.textBoneList);
        std::wstring boneName = mtw(pReadTextBuffer);
        TMatrix boneMat = m_HierarchyForm.matBoneList[i];
        managerObj->m_NameMatrixMap[0].insert(std::make_pair(boneName, boneMat));
    }

    iReadPos = 0;

    m_ReadFile.read(reinterpret_cast<char*>(&m_CharacterHeader), sizeof(m_CharacterHeader));

    int NameListSize = m_CharacterHeader.fbxNodeNameListSize;
    int NameMapSize = m_CharacterHeader.fbxNodeNameMapSize;
    int texFileListSize = m_CharacterHeader.texFileNameListSize;
    int bindPoseMapSize = m_CharacterHeader.bindPoseMapSize;

    managerObj->m_DrawList.resize(1);
    managerObj->m_DrawList[0] = std::make_shared<LFbxObj>();
    managerObj->m_DrawList[0]->m_DefaultFilePath = mtw(m_CharacterHeader.defaultPath);
    managerObj->m_pFbxNodeNameList.resize(NameListSize);
    managerObj->m_DrawList[0]->m_TexFileNameList.resize(texFileListSize);

    m_ReadFile.read(reinterpret_cast<char*>(&m_CharacterForm), sizeof(m_CharacterForm));

    for (int i = 0; i < NameListSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.fbxNodeNameList);
        managerObj->m_pFbxNodeNameList[i] = mtw(pReadTextBuffer);
    }

    iReadPos = 0;

    for (int i = 0; i < NameMapSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.fbxNodeNameMapwStr);
        std::wstring boneName = mtw(pReadTextBuffer);
        int boneIndex = m_CharacterForm.fbxNodeNameMapInt[i];
        managerObj->m_pFbxNodeNameMap.insert(std::make_pair(boneName, boneIndex));
    }

    iReadPos = 0;

    for (int i = 0; i < texFileListSize; i++)
    {
        ReadCharacterData(pReadTextureBuffer, 40, pReadPos, iReadPos, m_CharacterForm.textFileNameList);
        managerObj->m_DrawList[0]->m_TexFileNameList[i] = mtw(pReadTextureBuffer);
    }

    iReadPos = 0;

    for (int i = 0; i < bindPoseMapSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.bindPoseMapStr);
        std::wstring poseName = mtw(pReadTextBuffer);
        TMatrix poseMat = m_CharacterForm.bindPoseMapMat[i];
        managerObj->m_DrawList[0]->m_dxMatrixBindPoseMap.insert(std::make_pair(poseName, poseMat));
    }

    m_ReadFile.read(reinterpret_cast<char*>(&m_VertexHeader), sizeof(m_VertexHeader));

    int vertexFormSize = m_VertexHeader.vertexFormSize;
    int vertexListSize = m_VertexHeader.vertexListSize;

    m_ReadFile.read(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));

    managerObj->m_DrawList[0]->m_VertexList.resize(vertexListSize);
    managerObj->m_DrawList[0]->m_pVertexListIW.resize(vertexListSize);

    int vertexIndex = 0;
    for (int i = 0; i < vertexListSize; i++)
    {
        managerObj->m_DrawList[0]->m_VertexList[i] = m_VertexForm.pnct[vertexIndex];
        managerObj->m_DrawList[0]->m_pVertexListIW[i] = m_VertexForm.iw[vertexIndex];

        vertexIndex++;
        if (vertexIndex >= 100)
        {
            ZeroMemory(&m_VertexForm, sizeof(m_VertexForm));
            vertexIndex = 0;
            m_ReadFile.read(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));
            continue;
        }
    }

    m_ReadFile.read(reinterpret_cast<char*>(&m_MeshHeader), sizeof(m_MeshHeader));

    int subMtrlCount = m_MeshHeader.subMtrlCount;
    int offsetListSize = m_MeshHeader.offsetListSize;

    managerObj->m_DrawList[0]->m_SubMtrlVertexList.resize(subMtrlCount);
    managerObj->m_DrawList[0]->m_TriangleOffsetList.resize(offsetListSize);
    managerObj->m_DrawList[0]->m_SubMtrlCount = subMtrlCount;

    m_ReadFile.read(reinterpret_cast<char*>(&m_MeshForm), sizeof(m_MeshForm));

    for (int i = 0; i < subMtrlCount; i++)
    {
        managerObj->m_DrawList[0]->m_SubMtrlVertexList[i] = m_MeshForm.subMtrlVertexSize[i];
    }

    for (int i = 0; i < offsetListSize; i++)
    {
        managerObj->m_DrawList[0]->m_TriangleOffsetList[i] = m_MeshForm.OffsetList[i];
    }

    for (int i = 0; i < managerObj->m_DrawList[0]->m_TexFileNameList.size(); i++)
    {
        std::wstring textureName = managerObj->m_DrawList[0]->m_DefaultFilePath;
        textureName += managerObj->m_DrawList[0]->m_TexFileNameList[i];
        managerObj->m_DrawList[0]->LoadTexture(textureName);
    }

    managerObj->m_DrawList[0]->Set();
    managerObj->m_DrawList[0]->Create(L"../../res/hlsl/LightShadowMap.hlsl", L"../../res/map/topdownmap.jpg");

    LFbxMgr::GetInstance().m_map.insert(std::make_pair(key, managerObj));

    m_ReadFile.close();
    return true;
}

bool LCharacterIO::CharacterWrite(std::wstring fbxFilePath)
{
    LFbxObj* fbxObj = nullptr;
    fbxObj = LFbxMgr::GetInstance().Load(fbxFilePath, L"../../res/hlsl/CharacterShader.hlsl");

    CharacterReset();

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(fbxFilePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

    std::string saveFilePath = "../../res/UserFile/Character/";
    saveFilePath += FName;
    saveFilePath += ".bin";

    m_WriteFile.open(saveFilePath, std::ios::binary);

    m_HierarchyHeaader.treeSize = fbxObj->m_NameMatrixMap[0].size();
    m_WriteFile.write(reinterpret_cast<char*>(&m_HierarchyHeaader), sizeof(m_HierarchyHeaader));

    int count = 0;
    for (auto iter = fbxObj->m_NameMatrixMap[0].begin(); iter != fbxObj->m_NameMatrixMap[0].end(); iter++)
    {
        WriteCharacterData(wtm(iter->first).c_str(),
            m_TextMaxSize, pWritePos, iWritePos, m_HierarchyForm.textBoneList);
        m_HierarchyForm.matBoneList[count] = iter->second;
        count++;
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_HierarchyForm), sizeof(m_HierarchyForm));

    iWritePos = 0;

    m_CharacterHeader.fbxNodeNameMapSize = fbxObj->m_pFbxNodeNameMap.size();
    m_CharacterHeader.fbxNodeNameListSize = fbxObj->m_pFbxNodeNameList.size();
    m_CharacterHeader.texFileNameListSize = fbxObj->m_DrawList[0]->m_TexFileNameList.size();
    m_CharacterHeader.bindPoseMapSize = fbxObj->m_DrawList[0]->m_dxMatrixBindPoseMap.size();

    /*fbxObj->m_DrawList[0]->m_DefaultFilePath.clear();
    fbxObj->m_DrawList[0]->m_DefaultFilePath = L"../../res/Texture/";*/

    CopyMemory(m_CharacterHeader.defaultPath, wtm(fbxObj->m_DrawList[0]->m_DefaultFilePath).c_str(), 60);

    m_WriteFile.write(reinterpret_cast<char*>(&m_CharacterHeader), sizeof(m_CharacterHeader));

    for (int i = 0; i < fbxObj->m_pFbxNodeNameList.size(); i++)
    {
        WriteCharacterData(wtm(fbxObj->m_pFbxNodeNameList[i]).c_str(),
            m_TextMaxSize, pWritePos, iWritePos, m_CharacterForm.fbxNodeNameList);
    }

    iWritePos = 0;

    count = 0;
    for (auto iter = fbxObj->m_pFbxNodeNameMap.begin(); iter != fbxObj->m_pFbxNodeNameMap.end(); iter++)
    {
        WriteCharacterData(wtm(iter->first).c_str(),
            m_TextMaxSize, pWritePos, iWritePos, m_CharacterForm.fbxNodeNameMapwStr);
        m_CharacterForm.fbxNodeNameMapInt[count] = iter->second;
        count++;
    }

    iWritePos = 0;

    for (int i = 0; i < fbxObj->m_DrawList[0]->m_TexFileNameList.size(); i++)
    {
        WriteCharacterData(wtm(fbxObj->m_DrawList[0]->m_TexFileNameList[i]).c_str(),
            40, pWritePos, iWritePos, m_CharacterForm.textFileNameList);
    }

    iWritePos = 0;

    count = 0;
    for (auto iter = fbxObj->m_DrawList[0]->m_dxMatrixBindPoseMap.begin(); iter != fbxObj->m_DrawList[0]->m_dxMatrixBindPoseMap.end(); iter++)
    {
        WriteCharacterData(wtm(iter->first).c_str(), m_TextMaxSize, pWritePos, iWritePos, m_CharacterForm.bindPoseMapStr);
        m_CharacterForm.bindPoseMapMat[count] = iter->second;
        count++;
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_CharacterForm), sizeof(m_CharacterForm));

    m_VertexHeader.vertexFormSize = ComputeVertexFormSize(fbxObj->m_DrawList[0]->m_VertexList.size(), 100);
    m_VertexHeader.vertexListSize = fbxObj->m_DrawList[0]->m_VertexList.size();
    m_WriteFile.write(reinterpret_cast<char*>(&m_VertexHeader), sizeof(m_VertexHeader));

    int vertexIndex = 0;
    for (int i = 0; i < fbxObj->m_DrawList[0]->m_VertexList.size(); i++)
    {
        m_VertexForm.pnct[vertexIndex] = fbxObj->m_DrawList[0]->m_VertexList[i];
        m_VertexForm.iw[vertexIndex] = fbxObj->m_DrawList[0]->m_pVertexListIW[i];

        vertexIndex++;
        if (vertexIndex >= 100)
        {
            m_WriteFile.write(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));
            ZeroMemory(&m_VertexForm, sizeof(m_VertexForm));
            vertexIndex = 0;
            continue;
        }
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));

    m_MeshHeader.subMtrlCount = fbxObj->m_DrawList[0]->m_TexArray.size();
    m_MeshHeader.offsetListSize = fbxObj->m_DrawList[0]->m_TriangleOffsetList.size();

    m_WriteFile.write(reinterpret_cast<char*>(&m_MeshHeader), sizeof(m_MeshHeader));

    for (int i = 0; i < fbxObj->m_DrawList[0]->m_TriangleOffsetList.size(); i++)
    {
        m_MeshForm.OffsetList[i] = fbxObj->m_DrawList[0]->m_TriangleOffsetList[i];
    }

    for (int i = 0; i < fbxObj->m_DrawList[0]->m_TriangleList.size(); i++)
    {
        m_MeshForm.subMtrlVertexSize[i] = fbxObj->m_DrawList[0]->m_TriangleList[i].size();
    }

    m_WriteFile.write(reinterpret_cast<char*>(&m_MeshForm), sizeof(m_MeshForm));

    CharacterReset();
    m_WriteFile.close();
    return true;
}

bool LCharacterIO::CharacterRead(std::wstring filePath)
{
    m_TextMaxSize = 24;
    m_ReadFile.open(filePath, std::ios::binary);

    if (!m_ReadFile)
    {
        MessageBoxA(NULL, "파일이 존재하지 않습니다.", "Error Box", MB_OK);
        return true;
    }

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(filePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
    std::wstring key = mtw(FName);
    key += L".fbx";

    std::shared_ptr<LFbxObj> managerObj = std::make_shared<LFbxObj>();
    managerObj->m_NameMatrixMap.resize(1);

    CharacterReset();

    m_ReadFile.read(reinterpret_cast<char*>(&m_HierarchyHeaader), sizeof(m_HierarchyHeaader));

    int treeSize = m_HierarchyHeaader.treeSize;

    m_ReadFile.read(reinterpret_cast<char*>(&m_HierarchyForm), sizeof(m_HierarchyForm));

    iReadPos = 0;

    for (int i = 0; i < treeSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_HierarchyForm.textBoneList);
        std::wstring boneName = mtw(pReadTextBuffer);
        TMatrix boneMat = m_HierarchyForm.matBoneList[i];
        managerObj->m_NameMatrixMap[0].insert(std::make_pair(boneName, boneMat));
    }

    iReadPos = 0;

    m_ReadFile.read(reinterpret_cast<char*>(&m_CharacterHeader), sizeof(m_CharacterHeader));

    int NameListSize = m_CharacterHeader.fbxNodeNameListSize;
    int NameMapSize = m_CharacterHeader.fbxNodeNameMapSize;
    int texFileListSize = m_CharacterHeader.texFileNameListSize;
    int bindPoseMapSize = m_CharacterHeader.bindPoseMapSize;

    managerObj->m_DrawList.resize(1);
    managerObj->m_DrawList[0] = std::make_shared<LFbxObj>();
    managerObj->m_DrawList[0]->m_DefaultFilePath = mtw(m_CharacterHeader.defaultPath);
    managerObj->m_pFbxNodeNameList.resize(NameListSize);
    managerObj->m_DrawList[0]->m_TexFileNameList.resize(texFileListSize);

    m_ReadFile.read(reinterpret_cast<char*>(&m_CharacterForm), sizeof(m_CharacterForm));

    for (int i = 0; i < NameListSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.fbxNodeNameList);
        managerObj->m_pFbxNodeNameList[i] = mtw(pReadTextBuffer);
    }

    iReadPos = 0;

    for (int i = 0; i < NameMapSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.fbxNodeNameMapwStr);
        std::wstring boneName = mtw(pReadTextBuffer);
        int boneIndex = m_CharacterForm.fbxNodeNameMapInt[i];
        managerObj->m_pFbxNodeNameMap.insert(std::make_pair(boneName, boneIndex));
    }

    iReadPos = 0;

    for (int i = 0; i < texFileListSize; i++)
    {
        ReadCharacterData(pReadTextureBuffer, 40, pReadPos, iReadPos, m_CharacterForm.textFileNameList);
        managerObj->m_DrawList[0]->m_TexFileNameList[i] = mtw(pReadTextureBuffer);
    }

    iReadPos = 0;

    for (int i = 0; i < bindPoseMapSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.bindPoseMapStr);
        std::wstring poseName = mtw(pReadTextBuffer);
        TMatrix poseMat = m_CharacterForm.bindPoseMapMat[i];
        managerObj->m_DrawList[0]->m_dxMatrixBindPoseMap.insert(std::make_pair(poseName, poseMat));
    }

    m_ReadFile.read(reinterpret_cast<char*>(&m_VertexHeader), sizeof(m_VertexHeader));

    int vertexFormSize = m_VertexHeader.vertexFormSize;
    int vertexListSize = m_VertexHeader.vertexListSize;

    m_ReadFile.read(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));

    managerObj->m_DrawList[0]->m_VertexList.resize(vertexListSize);
    managerObj->m_DrawList[0]->m_pVertexListIW.resize(vertexListSize);

    int vertexIndex = 0;
    for (int i = 0; i < vertexListSize; i++)
    {
        managerObj->m_DrawList[0]->m_VertexList[i] = m_VertexForm.pnct[vertexIndex];
        managerObj->m_DrawList[0]->m_pVertexListIW[i] = m_VertexForm.iw[vertexIndex];

        vertexIndex++;
        if (vertexIndex >= 100)
        {
            ZeroMemory(&m_VertexForm, sizeof(m_VertexForm));
            vertexIndex = 0;
            m_ReadFile.read(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));
            continue;
        }
    }

    m_ReadFile.read(reinterpret_cast<char*>(&m_MeshHeader), sizeof(m_MeshHeader));

    int subMtrlCount = m_MeshHeader.subMtrlCount;
    int offsetListSize = m_MeshHeader.offsetListSize;

    managerObj->m_DrawList[0]->m_SubMtrlVertexList.resize(subMtrlCount);
    managerObj->m_DrawList[0]->m_TriangleOffsetList.resize(offsetListSize);
    managerObj->m_DrawList[0]->m_SubMtrlCount = subMtrlCount;

    m_ReadFile.read(reinterpret_cast<char*>(&m_MeshForm), sizeof(m_MeshForm));

    for (int i = 0; i < subMtrlCount; i++)
    {
        managerObj->m_DrawList[0]->m_SubMtrlVertexList[i] = m_MeshForm.subMtrlVertexSize[i];
    }

    for (int i = 0; i < offsetListSize; i++)
    {
        managerObj->m_DrawList[0]->m_TriangleOffsetList[i] = m_MeshForm.OffsetList[i];
    }

    for (int i = 0; i < managerObj->m_DrawList[0]->m_TexFileNameList.size(); i++)
    {
        std::wstring textureName = managerObj->m_DrawList[0]->m_DefaultFilePath;
        textureName += managerObj->m_DrawList[0]->m_TexFileNameList[i];
        managerObj->m_DrawList[0]->LoadTexture(textureName);
    }

    managerObj->m_DrawList[0]->Set();
    managerObj->m_DrawList[0]->Create(L"../../res/hlsl/CharacterShader.hlsl", L"../../res/map/topdownmap.jpg");

    LFbxMgr::GetInstance().m_map.insert(std::make_pair(key, managerObj));

    m_ReadFile.close();
    return true;
}

bool LCharacterIO::CharacterRead(std::wstring filePath, std::wstring shaderPath)
{
    m_TextMaxSize = 24;
    m_ReadFile.open(filePath, std::ios::binary);

    if (!m_ReadFile)
    {
        MessageBoxA(NULL, "파일이 존재하지 않습니다.", "Error Box", MB_OK);
        return true;
    }

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(filePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);
    std::wstring key = mtw(FName);
    key += L".fbx";

    std::shared_ptr<LFbxObj> managerObj = std::make_shared<LFbxObj>();
    managerObj->m_NameMatrixMap.resize(1);

    CharacterReset();

    m_ReadFile.read(reinterpret_cast<char*>(&m_HierarchyHeaader), sizeof(m_HierarchyHeaader));

    int treeSize = m_HierarchyHeaader.treeSize;

    m_ReadFile.read(reinterpret_cast<char*>(&m_HierarchyForm), sizeof(m_HierarchyForm));

    iReadPos = 0;

    for (int i = 0; i < treeSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_HierarchyForm.textBoneList);
        std::wstring boneName = mtw(pReadTextBuffer);
        TMatrix boneMat = m_HierarchyForm.matBoneList[i];
        managerObj->m_NameMatrixMap[0].insert(std::make_pair(boneName, boneMat));
    }

    iReadPos = 0;

    m_ReadFile.read(reinterpret_cast<char*>(&m_CharacterHeader), sizeof(m_CharacterHeader));

    int NameListSize = m_CharacterHeader.fbxNodeNameListSize;
    int NameMapSize = m_CharacterHeader.fbxNodeNameMapSize;
    int texFileListSize = m_CharacterHeader.texFileNameListSize;
    int bindPoseMapSize = m_CharacterHeader.bindPoseMapSize;

    managerObj->m_DrawList.resize(1);
    managerObj->m_DrawList[0] = std::make_shared<LFbxObj>();
    managerObj->m_DrawList[0]->m_DefaultFilePath = mtw(m_CharacterHeader.defaultPath);
    managerObj->m_pFbxNodeNameList.resize(NameListSize);
    managerObj->m_DrawList[0]->m_TexFileNameList.resize(texFileListSize);

    m_ReadFile.read(reinterpret_cast<char*>(&m_CharacterForm), sizeof(m_CharacterForm));

    for (int i = 0; i < NameListSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.fbxNodeNameList);
        managerObj->m_pFbxNodeNameList[i] = mtw(pReadTextBuffer);
    }

    iReadPos = 0;

    for (int i = 0; i < NameMapSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.fbxNodeNameMapwStr);
        std::wstring boneName = mtw(pReadTextBuffer);
        int boneIndex = m_CharacterForm.fbxNodeNameMapInt[i];
        managerObj->m_pFbxNodeNameMap.insert(std::make_pair(boneName, boneIndex));
    }

    iReadPos = 0;

    for (int i = 0; i < texFileListSize; i++)
    {
        ReadCharacterData(pReadTextureBuffer, 40, pReadPos, iReadPos, m_CharacterForm.textFileNameList);
        managerObj->m_DrawList[0]->m_TexFileNameList[i] = mtw(pReadTextureBuffer);
    }

    iReadPos = 0;

    for (int i = 0; i < bindPoseMapSize; i++)
    {
        ReadCharacterData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_CharacterForm.bindPoseMapStr);
        std::wstring poseName = mtw(pReadTextBuffer);
        TMatrix poseMat = m_CharacterForm.bindPoseMapMat[i];
        managerObj->m_DrawList[0]->m_dxMatrixBindPoseMap.insert(std::make_pair(poseName, poseMat));
    }

    m_ReadFile.read(reinterpret_cast<char*>(&m_VertexHeader), sizeof(m_VertexHeader));

    int vertexFormSize = m_VertexHeader.vertexFormSize;
    int vertexListSize = m_VertexHeader.vertexListSize;

    m_ReadFile.read(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));

    managerObj->m_DrawList[0]->m_VertexList.resize(vertexListSize);
    managerObj->m_DrawList[0]->m_pVertexListIW.resize(vertexListSize);

    int vertexIndex = 0;
    for (int i = 0; i < vertexListSize; i++)
    {
        managerObj->m_DrawList[0]->m_VertexList[i] = m_VertexForm.pnct[vertexIndex];
        managerObj->m_DrawList[0]->m_pVertexListIW[i] = m_VertexForm.iw[vertexIndex];

        vertexIndex++;
        if (vertexIndex >= 100)
        {
            ZeroMemory(&m_VertexForm, sizeof(m_VertexForm));
            vertexIndex = 0;
            m_ReadFile.read(reinterpret_cast<char*>(&m_VertexForm), sizeof(m_VertexForm));
            continue;
        }
    }

    m_ReadFile.read(reinterpret_cast<char*>(&m_MeshHeader), sizeof(m_MeshHeader));

    int subMtrlCount = m_MeshHeader.subMtrlCount;
    int offsetListSize = m_MeshHeader.offsetListSize;

    managerObj->m_DrawList[0]->m_SubMtrlVertexList.resize(subMtrlCount);
    managerObj->m_DrawList[0]->m_TriangleOffsetList.resize(offsetListSize);
    managerObj->m_DrawList[0]->m_SubMtrlCount = subMtrlCount;

    m_ReadFile.read(reinterpret_cast<char*>(&m_MeshForm), sizeof(m_MeshForm));

    for (int i = 0; i < subMtrlCount; i++)
    {
        managerObj->m_DrawList[0]->m_SubMtrlVertexList[i] = m_MeshForm.subMtrlVertexSize[i];
    }

    for (int i = 0; i < offsetListSize; i++)
    {
        managerObj->m_DrawList[0]->m_TriangleOffsetList[i] = m_MeshForm.OffsetList[i];
    }

    for (int i = 0; i < managerObj->m_DrawList[0]->m_TexFileNameList.size(); i++)
    {
        std::wstring textureName = managerObj->m_DrawList[0]->m_DefaultFilePath;
        textureName += managerObj->m_DrawList[0]->m_TexFileNameList[i];
        managerObj->m_DrawList[0]->LoadTexture(textureName);
    }

    managerObj->m_DrawList[0]->Set();
    managerObj->m_DrawList[0]->Create(shaderPath, L"../../res/map/topdownmap.jpg");

    LFbxMgr::GetInstance().m_map.insert(std::make_pair(key, managerObj));

    m_ReadFile.close();
    return true;
}




