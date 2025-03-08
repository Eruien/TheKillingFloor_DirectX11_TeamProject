#include "LExportIO.h"

void LExportIO::Reset()
{
    ZeroMemory(&m_ExportHeader, sizeof(m_ExportHeader));
    ZeroMemory(&m_ExportForm, sizeof(m_ExportForm));
    iWritePos = 0;
    iWritePos2 = 0;
    iReadPos = 0;
    iReadPos2 = 0;
    itemCount = 0;
}

void LExportIO::ResetiPos()
{
    iWritePos = 0;
    iWritePos2 = 0;
    iReadPos = 0;
    iReadPos2 = 0;
    itemCount = 0;
}

void LExportIO::ReadStringData(char* buffer, int bufferSize, void* pReadPos, int& iReadPos, char* changePtr)
{
    pReadPos = &changePtr[iReadPos];
    CopyMemory(buffer, pReadPos, bufferSize);
    iReadPos += bufferSize;

    return;
}

bool LExportIO::ExportRead(std::wstring FormFileName)
{
    Reset();

    std::wstring saveFilePath = L"../../res/UserFile/Form/";
    saveFilePath += FormFileName;

    m_ReadFile.open(saveFilePath, std::ios::binary);
    m_ReadFile.read(reinterpret_cast<char*>(&m_ExportHeader), sizeof(m_ExportHeader));
    m_ReadFile.read(reinterpret_cast<char*>(&m_ExportForm), sizeof(m_ExportForm));

    int animationListSize = m_ExportHeader.animationListSize;
    int itemListSize = m_ExportHeader.itemListSize;

    for (int i = 0; i < animationListSize; i++)
    {
        ReadStringData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_ExportForm.animationList);
        std::wstring animationName = mtw(pReadTextBuffer);
        m_AnimationList.push_back(animationName);
    }

    ResetiPos();
    for (int i = 0; i < itemListSize; i++)
    {
        ReadStringData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos, m_ExportForm.itemList);
        std::wstring itemName = mtw(pReadTextBuffer);
        m_ItemList.push_back(itemName);

        ReadStringData(pReadTextBuffer, m_TextMaxSize, pReadPos, iReadPos2, m_ExportForm.itemMatParentName);
        std::wstring itemParentName = mtw(pReadTextBuffer);
        m_ItemParentName.push_back(itemParentName);

        m_ItemScale.push_back(m_ExportForm.itemScale[i]);
        m_ItemRotation.push_back(m_ExportForm.itemRotation[i]);
        m_ItemTranslation.push_back(m_ExportForm.itemTranslation[i]);
    }

    m_ReadFile.close();
    return true;
}

LExportIO::LExportIO()
{
    Reset();
}
