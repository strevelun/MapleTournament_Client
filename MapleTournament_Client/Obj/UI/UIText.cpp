#include "UIText.h"
#include "../../GraphicCore/Graphics.h"
#include "../../Debug.h"
#include "../../Constants.h"

UIText::UIText(const UIText& _uiText)
	: UI(_uiText)
{
	m_text = _uiText.m_text;
	m_size = _uiText.m_size;

	ReassignText(_uiText.m_text);
	D2D1_COLOR_F color = _uiText.m_pBrush->GetColor();
	SetTextColor(D2D1::ColorF(color.r, color.g, color.b, color.a));
}

UIText::UIText(UI* _pParentPanel, std::wstring _text, FLOAT _size, float _xpos, float _ypos, FLOAT _pivotX, FLOAT _pivotY) :
	UI(_pParentPanel, 0, 0, _xpos, _ypos, _pivotX, _pivotY), m_size(_size), m_text(_text)
{
	if (!Init()) m_bAlive = false;
}

UIText::~UIText()
{
	if(m_pLayout)	m_pLayout->Release();
	if(m_pBrush)	m_pBrush->Release();
	if(m_pTextFormat)		m_pTextFormat->Release();
}

void UIText::ReassignText(std::wstring _text)
{
	m_text = _text;

	IDWriteFactory* pDWriteFactory = Graphics::GetInst()->GetDwriteFactory();

	HRESULT hr = pDWriteFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		m_size, L"en-US", &m_pTextFormat);
	if (FAILED(hr))
	{
		Debug::Log(("UIText : CreateTextFormat returned : " + std::to_string(hr)));
		return;
	}

	float width = float(m_pParentUI ? m_pParentUI->GetWidth() : ScreenWidth);
	float height = float(m_pParentUI ? m_pParentUI->GetHeight() : ScreenHeight);

	hr = pDWriteFactory->CreateTextLayout(m_text.c_str(), (UINT32)m_text.length(), m_pTextFormat, width, height, &m_pLayout);
	if (FAILED(hr))
	{
		Debug::Log(("UIText : CreateTextLayout returned : " + std::to_string(hr)));
		return;
	}

	hr = m_pLayout->GetMetrics(&m_metrics);
	if (FAILED(hr))
	{
		Debug::Log(("UIText : GetMetrics returned : " + std::to_string(hr)));
		return;
	}

	SetSize((UINT)m_metrics.width, (UINT)m_metrics.height);
}

void UIText::SetTextColor(D2D1::ColorF _color)
{
	if (m_pBrush)	m_pBrush->Release();

	Graphics::GetInst()->CreateSolidColorBrush(_color, &m_pBrush);
}

void UIText::Render()
{
	//Graphics::GetInst()->GetRenderTarget()->DrawRectangle(m_rect, m_pBrush);
	Graphics::GetInst()->GetRenderTarget()->DrawTextLayout(D2D1::Point2F(m_rect.left, m_rect.top), m_pLayout, m_pBrush);
}

bool UIText::Init()
{
	IDWriteFactory* pDWriteFactory = Graphics::GetInst()->GetDwriteFactory();

	HRESULT hr = pDWriteFactory->CreateTextFormat(L"Arial", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		m_size, L"en-US", &m_pTextFormat);
	if (FAILED(hr))
	{
		Debug::Log(("UIText : CreateTextFormat returned : " + std::to_string(hr)));
		return false;
	}

	Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Black, &m_pBrush);

	if(m_pParentUI)
		hr = pDWriteFactory->CreateTextLayout(m_text.c_str(), (u_int)m_text.length(), m_pTextFormat, (float)m_pParentUI->GetWidth(), (float)m_pParentUI->GetHeight(), &m_pLayout);
	else
		hr = pDWriteFactory->CreateTextLayout(m_text.c_str(), (u_int)m_text.length(), m_pTextFormat, ScreenWidth, ScreenHeight, &m_pLayout);
	if (FAILED(hr))
	{
		Debug::Log(("UIText : CreateTextLayout returned : " + std::to_string(hr)));
		return false;
	}

	hr = m_pLayout->GetMetrics(&m_metrics); 
	if (FAILED(hr))
	{
		Debug::Log(("UIText : GetMetrics returned : " + std::to_string(hr)));
		return false;
	}

	SetSize((UINT)m_metrics.width, (UINT)m_metrics.height);

	if (m_metrics.lineCount > 1)
	{
		if (m_pParentUI)
			m_pParentUI->SetSize((UINT)m_metrics.width, m_metrics.lineCount * m_pParentUI->GetHeight());
	}
	return true;
}
