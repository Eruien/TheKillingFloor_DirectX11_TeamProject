#pragma once
#include <fstream>
#include <vector>
#include "LModel.h"
#include "LFbxMgr.h"

struct HierarchyHeader
{
    int treeSize;
};

struct HierarchyForm
{
    char textBoneList[2400];
    TMatrix matBoneList[100];
};

struct CharacterHeader
{
    int fbxNodeNameMapSize = 0;
    int fbxNodeNameListSize = 0;
    int texFileNameListSize = 0;
    int bindPoseMapSize = 0;
    char defaultPath[60];
};

struct CharacterForm
{
    char fbxNodeNameMapwStr[3600];
    int fbxNodeNameMapInt[600];
    char fbxNodeNameList[3600];
    char textFileNameList[320];
    char bindPoseMapStr[3600];
    TMatrix bindPoseMapMat[150];
};

struct VertexHeader
{
    int vertexListSize = 0;
    int vertexFormSize = 0;
};

struct VertexForm
{
    SimpleVertex pnct[100];
    LVertexIW iw[100];
};

struct MeshHeader
{
    int subMtrlCount = 0;
    int offsetListSize = 0;
};

struct MeshForm
{
    int subMtrlVertexSize[20];
    int OffsetList[20];
};

class LCharacterIO
{
public:
    static LCharacterIO& GetInstance()
    {
        static LCharacterIO input;
        return input;
    }
public:
    std::ofstream m_WriteFile;
    std::ifstream m_ReadFile;
public:
    HierarchyHeader m_HierarchyHeaader;
    HierarchyForm m_HierarchyForm;
    CharacterHeader m_CharacterHeader;
    CharacterForm m_CharacterForm;
    VertexHeader m_VertexHeader;
    VertexForm m_VertexForm;
    MeshHeader m_MeshHeader;
    MeshForm m_MeshForm;
public:
    char pReadTextBuffer[24];
    char pReadTextureBuffer[40];
    int m_TextMaxSize = 24;
    void* pWritePos;
    int iWritePos;
    void* pReadPos;
    int iReadPos;
public:
    void CharacterReset();
    void WriteCharacterData(const char* data, int dataSize, void* pPos, int& iPos, char* changePtr);
    void ReadCharacterData(char* buffer, int bufferSize, void* pReadPos, int& iReadPos, char* changePtr);
    int ComputeVertexFormSize(int vertexListSize, int formSize);
public:
    bool ItemWrite(std::wstring fbxFilePath);
    bool ItemRead(std::wstring filePath);
    bool CharacterWrite(std::wstring fbxFilePath);
    bool CharacterRead(std::wstring filePath);
    bool CharacterRead(std::wstring filePath, std::wstring shaderPath);
};

