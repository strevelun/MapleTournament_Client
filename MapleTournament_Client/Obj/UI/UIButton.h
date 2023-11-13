#pragma once
#include "UI.h"

#include <functional>

class Mouse;
class Bitmap;

class UIButton :
    public UI
{
private:    
    Bitmap* m_pNormalBitmap = nullptr;
    Bitmap* m_pPressedBitmap = nullptr;
    Bitmap* m_pMouseHoverBitmap = nullptr;

    UIText* m_pUIText = nullptr;
    Bitmap* m_pBitmap = nullptr;
    std::function<void()> m_Callback;

public:
    UIButton(const UIButton& _uiButton);
    UIButton(UI* _pParentPanel, UINT _width, UINT _height, float _xpos = 0.f, float _ypos = 0.f,  FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f);
    ~UIButton();

    UIButton* Clone() const override { return new UIButton(*this); }

    void Update() override;
    void Render() override;
    
    template <typename T>
    void SetCallback(void (T::*pFunc)(), T* _pObj) { m_Callback = std::bind(pFunc, _pObj); }
    void SetCallback(const std::function<void()>& _func) {  m_Callback = _func; }
    void SetBitmap(Bitmap* _pBitmap);
    void SetMouseHoverBitmap(Bitmap* _pBitmap) { m_pMouseHoverBitmap = _pBitmap; }
    void SetUIText(UIText* _pUIText);
    void SetPressedBitmap(Bitmap* _pBitmap) { m_pPressedBitmap = _pBitmap; }

    UIText* GetUIText();

    void OnClick() override;
    void OnLButtonDown() override;
    void MousePressed() override;
    void MouseOn() override;
    void MouseOut() override;
    void MouseHover() override;
};

