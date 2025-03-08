#pragma once
#include <fstream>
#include <vector>
#include "LStd.h"

struct ExportHeader
{
    int animationListSize = 0;
    int itemListSize = 0;
};

struct ExportData
{
    char characterFbxPath[60];
    TMatrix characterPos;
    char defaultposeAnimation[60];
    char animationList[600];
    char itemList[300];
    char itemMatParentName[300];
    TMatrix itemScale[5];
    TMatrix itemRotation[5];
    TMatrix itemTranslation[5];
    float matNumber[9];
};

class LExportIO
{
public:
    static LExportIO& GetInstance()
    {
        static LExportIO input;
        return input;
    }
public:
    std::ofstream m_WriteFile;
    std::ifstream m_ReadFile;
public:
    ExportHeader m_ExportHeader;
    ExportData m_ExportForm;
public:
    std::vector<std::wstring> m_AnimationList;
    std::vector<std::wstring> m_ItemList;
    std::vector<std::wstring> m_ItemParentName;
    std::vector<TMatrix> m_ItemScale;
    std::vector<TMatrix> m_ItemRotation;
    std::vector<TMatrix> m_ItemTranslation;
public:
    int m_TextMaxSize = 60;
    void* pWritePos = nullptr;
    int iWritePos = 0;
    int iWritePos2 = 0;
    void* pReadPos = nullptr;
    int iReadPos = 0;
    int iReadPos2 = 0;
    char pReadTextBuffer[60] = { 0, };
    int itemCount = 0;
public:
    void SetCharacterFbxPath(std::wstring characterFbxPath, TMatrix characterPos);
    void SetDefaultPoseAnimationPath(std::wstring defaultAnimationPath);
    void SetAnimationPath(std::wstring AnimationPath);
    void SetItem(std::wstring ItemPath, std::wstring ParentName, TMatrix matScale, TMatrix matRotation, TMatrix matTranslation);
public:
    void Reset();
    void ResetiPos();
    void WriteStringData(const char* data, int dataSize, void* pPos, int& iPos, char* changePtr);
    void ReadStringData(char* buffer, int bufferSize, void* pReadPos, int& iReadPos, char* changePtr);
    bool ExportWrite(std::wstring FormFileName, int animationListSize, int itemListSize);
    bool ExportRead(std::wstring FormFileName);
public:
    LExportIO();
};

