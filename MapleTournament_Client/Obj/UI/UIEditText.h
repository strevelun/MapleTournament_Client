#pragma once
#include "UI.h"

#include <string>
#include <functional>

class UIButton;
class Bitmap;

class UIEditText :
    public UI
{
private:
    std::function<void(UIEditText*, const std::wstring&)> m_Callback;

    std::wstring m_strInput;
    D2D1_RECT_F             m_rectCursor;
    bool                    m_showCursor = false;
    float                   m_deltaTime = 0.0f;
    int                     m_inputMaxCount = 0;

    IDWriteTextFormat* m_pTextFormat = nullptr;
    ID2D1SolidColorBrush* m_pBrush = nullptr;

    Bitmap* m_pBitmap = nullptr;

public:
    UIEditText(const UIEditText& _uiEditText);
    UIEditText(UI* _pParentPanel, UINT _width, UINT _height, int m_inputMaxCount, float _xpos = 0, float _ypos = 0, FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f);
    ~UIEditText();

    UIEditText* Clone() const override { return new UIEditText(*this); }

    const std::wstring& GetText() const { return m_strInput; }

    void Update() override;
    void Render() override;

    template <typename T>
    void SetCallback(void (T::*pFunc)(UIEditText*, const std::wstring&), T* _pObj) { m_Callback = std::bind(pFunc, _pObj, std::placeholders::_1, std::placeholders::_2); }
    void SetBitmap(Bitmap* _pBitmap) { m_pBitmap = _pBitmap; }

    void ClearEditText() { m_strInput.clear(); }

    //void SetButton(UIButton* _pButton) { m_pButton = _pButton; }
};

