#include "LAnimationIO.h"

void LAnimationIO::Reset()
{
    ZeroMemory(&m_DataHeader, sizeof(m_DataHeader));
    ZeroMemory(&m_DataForm, sizeof(m_DataForm));

    pStrWritePos = m_DataForm.textBoneList;
    iStrWritePos = 0;
    pStrReadPos = m_DataForm.textBoneList;
    iStrReadPos = 0;
    IsDataMax = false;
}

void LAnimationIO::WriteBoneNameData(const char* data, int dataSize)
{
    CopyMemory(pStrWritePos, data, dataSize);
    iStrWritePos += m_TextMaxSize;
    pStrWritePos = &m_DataForm.textBoneList[iStrWritePos];

    if (iStrWritePos >= (m_DataMaxSize * m_TextMaxSize))
    {
        IsDataMax = true;
        return;
    }

    IsDataMax = false;
    return;
}

void LAnimationIO::ReadBoneNameData()
{
    CopyMemory(pReadTextBuffer, pStrReadPos, m_TextMaxSize);
    iStrReadPos += m_TextMaxSize;
    pStrReadPos = &m_DataForm.textBoneList[iStrReadPos];

    if (iStrReadPos >= (m_DataMaxSize * m_TextMaxSize))
    {
        IsDataMax = true;
        return;
    }

    IsDataMax = false;
    return;
}

int LAnimationIO::ComputeFormSize(LFbxObj* fbxObj)
{
    int mapSize = fbxObj->m_NameMatrixMap.size();
    int matListSize = fbxObj->m_NameMatrixMap[0].size();

    int totalDataSize = (m_TextMaxSize + sizeof(TMatrix)) * mapSize * matListSize;
    int result = totalDataSize / sizeof(AnimationForm) + 1;

    return result;
}

bool LAnimationIO::AnimationWrite(std::wstring fbxFilePath)
{
    LFbxObj* fbxObj = nullptr;
    fbxObj = LFbxMgr::GetInstance().Load(fbxFilePath, L"../../res/hlsl/CustomizeMap.hlsl");

    Reset();

    m_DataHeader.formSize = ComputeFormSize(fbxObj);
    m_DataHeader.mapSize = fbxObj->m_NameMatrixMap.size();
    m_DataHeader.matListSize = fbxObj->m_NameMatrixMap[0].size();
    m_DataHeader.startFrame = fbxObj->m_iStartFrame;
    m_DataHeader.endFrame = fbxObj->m_iEndFrame;

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(fbxFilePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);

    std::string saveFilePath = "../../res/UserFile/Animation/";
    saveFilePath += FName;
    saveFilePath += ".bin";

    m_WriteFile.open(saveFilePath, std::ios::binary);
    m_WriteFile.write(reinterpret_cast<char*>(&m_DataHeader), sizeof(m_DataHeader));

    int count = 0;
    for (int i = 0; i < fbxObj->m_NameMatrixMap.size(); i++)
    {
        for (auto iter = fbxObj->m_NameMatrixMap[i].begin(); iter != fbxObj->m_NameMatrixMap[i].end(); iter++)
        {
            std::string convertStr = wtm(iter->first);
            WriteBoneNameData(convertStr.c_str(), convertStr.size());
            m_DataForm.matBoneList[count] = iter->second;
            count++;

            if (IsDataMax)
            {
                m_WriteFile.write(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));
                Reset();
                count = 0;
            }
        }
    }
    m_WriteFile.write(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));
    Reset();
    m_WriteFile.close();

    return true;
}

bool LAnimationIO::AnimationWrite(LFbxObj* fbxObj, std::wstring filePath)
{
    if (fbxObj == nullptr) return false;

    Reset();

    m_DataHeader.formSize = ComputeFormSize(fbxObj);
    m_DataHeader.mapSize = fbxObj->m_NameMatrixMap.size();
    m_DataHeader.matListSize = fbxObj->m_NameMatrixMap[0].size();
    m_DataHeader.startFrame = fbxObj->m_iStartFrame;
    m_DataHeader.endFrame = fbxObj->m_iEndFrame;

    m_WriteFile.open(filePath, std::ios::binary);
    m_WriteFile.write(reinterpret_cast<char*>(&m_DataHeader), sizeof(m_DataHeader));

    int count = 0;
    for (int i = 0; i < fbxObj->m_NameMatrixMap.size(); i++)
    {
        for (auto iter = fbxObj->m_NameMatrixMap[i].begin(); iter != fbxObj->m_NameMatrixMap[i].end(); iter++)
        {
            std::string convertStr = wtm(iter->first);
            WriteBoneNameData(convertStr.c_str(), convertStr.size());
            m_DataForm.matBoneList[count] = iter->second;
            count++;

            if (IsDataMax)
            {
                m_WriteFile.write(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));
                Reset();
                count = 0;
            }
        }
    }
    m_WriteFile.write(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));
    Reset();
    m_WriteFile.close();

    return true;
}

void LAnimationIO::AnimationRead(std::wstring filePath, std::wstring fbxFilePath)
{
    m_ReadFile.open(filePath, std::ios::binary);
    LFbxObj* fbxObj = nullptr;

    if (!m_ReadFile)
    {
        fbxObj = LFbxMgr::GetInstance().Load(fbxFilePath, L"../../res/hlsl/CustomizeMap.hlsl");
        AnimationWrite(fbxObj, filePath);
        return;
    }

    std::size_t found = fbxFilePath.find_last_of(L"/");
    std::wstring path = fbxFilePath.substr(0, found + 1);
    std::wstring key = fbxFilePath.substr(found + 1);
    std::shared_ptr<LFbxObj> managerObj = std::make_shared<LFbxObj>();

    Reset();
    m_ReadFile.read(reinterpret_cast<char*>(&m_DataHeader), sizeof(m_DataHeader));

    int formSize = m_DataHeader.formSize;
    int mapSize = m_DataHeader.mapSize;
    int matListSize = m_DataHeader.matListSize;

    managerObj->m_AnimationTreeSize = matListSize;
    managerObj->m_NameMatrixMap.resize(mapSize);
    managerObj->m_iStartFrame = m_DataHeader.startFrame;
    managerObj->m_iEndFrame = m_DataHeader.endFrame;

    m_ReadFile.read(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));

    int count = 0;
    for (int i = 0; i < mapSize; i++)
    {
        for (int j = 0; j < matListSize; j++)
        {
            ReadBoneNameData();
            std::wstring boneName = mtw(pReadTextBuffer);
            managerObj->m_NameMatrixMap[i].insert(std::make_pair(boneName, m_DataForm.matBoneList[count]));
            count++;
            if (IsDataMax)
            {
                Reset();
                count = 0;
                m_ReadFile.read(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));
            }
        }
    }

    m_ReadFile.close();
    LFbxMgr::GetInstance().m_map.insert(std::make_pair(key, managerObj));
    return;
}

void LAnimationIO::AnimationRead(std::wstring filePath)
{
    m_ReadFile.open(filePath, std::ios::binary);
    LFbxObj* fbxObj = nullptr;

    if (!m_ReadFile)
    {
        return;
    }

    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char FName[MAX_PATH];
    char Ext[MAX_PATH];

    _splitpath_s(wtm(filePath).c_str(), Drive, MAX_PATH, Dir, MAX_PATH, FName, MAX_PATH, Ext, MAX_PATH);


    std::wstring key = mtw(FName);
    key += L".fbx";
    std::shared_ptr<LFbxObj> managerObj = std::make_shared<LFbxObj>();

    Reset();
    m_ReadFile.read(reinterpret_cast<char*>(&m_DataHeader), sizeof(m_DataHeader));

    int formSize = m_DataHeader.formSize;
    int mapSize = m_DataHeader.mapSize;
    int matListSize = m_DataHeader.matListSize;

    managerObj->m_AnimationTreeSize = matListSize;
    managerObj->m_NameMatrixMap.resize(mapSize);
    managerObj->m_iStartFrame = m_DataHeader.startFrame;
    managerObj->m_iEndFrame = m_DataHeader.endFrame;

    m_ReadFile.read(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));

    int count = 0;
    for (int i = 0; i < mapSize; i++)
    {
        for (int j = 0; j < matListSize; j++)
        {
            ReadBoneNameData();
            std::wstring boneName = mtw(pReadTextBuffer);
            managerObj->m_NameMatrixMap[i].insert(std::make_pair(boneName, m_DataForm.matBoneList[count]));
            count++;
            if (IsDataMax)
            {
                Reset();
                count = 0;
                m_ReadFile.read(reinterpret_cast<char*>(&m_DataForm), sizeof(m_DataForm));
            }
        }
    }

    m_ReadFile.close();
    LFbxMgr::GetInstance().m_map.insert(std::make_pair(key, managerObj));
    return;
}
