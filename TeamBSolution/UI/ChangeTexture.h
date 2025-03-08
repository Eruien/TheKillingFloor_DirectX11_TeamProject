#pragma once
#include "MonoBehaviour.h"
class ChangeTexture : public MonoBehaviour
{
public:
    ChangeTexture();
    virtual ~ChangeTexture();

    void Init() override;
    void Frame() override;
    void ChangeFromPath(wstring path);
    void ChangeFromManager(wstring key);
};

