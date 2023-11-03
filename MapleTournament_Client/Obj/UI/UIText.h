#pragma once
#include "UI.h"

#include <d2d1.h>
#include <dwrite.h>

class UIText :
    public UI
{
private:
    std::wstring m_text;


    FLOAT           m_size = 0.f;

    IDWriteTextFormat* m_pTextFormat = nullptr;
    IDWriteTextLayout* m_pLayout = nullptr;
    ID2D1SolidColorBrush* m_pBrush = nullptr;
    DWRITE_TEXT_METRICS         m_metrics;

public:
    UIText(const UIText& _uiText);
    UIText(UI* _pParentPanel, std::wstring _text, FLOAT _size, float _xpos = 0, float _ypos = 0, FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f);
    ~UIText();

    UIText* Clone() const override { return new UIText(*this); }

    void ReassignText(std::wstring _text);
    void SetTextColor(D2D1::ColorF _color);

    const std::wstring& GetText() const { return m_text; }

    void Render();

private:
    bool Init();
};

