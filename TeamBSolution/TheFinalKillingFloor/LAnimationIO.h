#pragma once
#include <fstream>
#include <vector>
#include "LModel.h"
#include "LFbxMgr.h"

// 최대 250개의 데이터

struct AnimationHeader
{
    int formSize = 0;
    int mapSize = 0;
    int matListSize = 0;
    int startFrame = 0;
    int endFrame = 0;
};

struct AnimationForm
{
    char textBoneList[6000];
    TMatrix matBoneList[250];
};

class LAnimationIO
{
public:
    static LAnimationIO& GetInstance()
    {
        static LAnimationIO input;
        return input;
    }
public:
    std::ofstream m_WriteFile;
    std::ifstream m_ReadFile;
public:
    AnimationHeader m_DataHeader;
    AnimationForm m_DataForm;
public:
    int m_TextMaxSize = 24;
    int m_DataMaxSize = 250;
    void* pStrWritePos;
    int iStrWritePos;
    char pReadTextBuffer[24];
    void* pStrReadPos;
    int iStrReadPos;
    bool IsDataMax = false;
public:
    void Reset();
    void WriteBoneNameData(const char* data, int dataSize);
    void ReadBoneNameData();
    int ComputeFormSize(LFbxObj* fbxObj);
    bool AnimationWrite(std::wstring fbxFilePath);
    bool AnimationWrite(LFbxObj* fbxObj, std::wstring filePath);
    void AnimationRead(std::wstring filePath, std::wstring fbxFilePath);
    void AnimationRead(std::wstring filePath);
};

