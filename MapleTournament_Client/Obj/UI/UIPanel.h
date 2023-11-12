#pragma once
#include "UI.h"

#include <vector>

class Bitmap;

class UIPanel :
    public UI
{
    std::vector<UI*> m_vecMemberUI;
    Bitmap* m_pBitmap = nullptr;

    //ID2D1SolidColorBrush* m_pBrush = nullptr; // for rect visualization

public:
    UIPanel(const UIPanel& _ui);
    UIPanel(UI* _pParentPanel, UINT _width, UINT _height, float _xpos = 0.f, float _ypos = 0.f, FLOAT _pivotX = 0.f, FLOAT _pivotY = 0.f);
    ~UIPanel();

    UIPanel* Clone() const override { return new UIPanel(*this); }

    void AddChildUI(UI* _pUI);
    UI* FindChildUI(const std::wstring& _strName);
    const std::vector<UI*>& GetMemberUI() const { return m_vecMemberUI; }

    void SetActive(bool _bActive) override;
    void SetBitmap(Bitmap* _pBitmap) { m_pBitmap = _pBitmap; }
    void SetPos(float _xpos, float _ypos) override;
    void SetClickable(bool _bClickable) override;

    void SetPopup(bool _isPopup) override;

    void Update() override;
    void Render() override;
};

