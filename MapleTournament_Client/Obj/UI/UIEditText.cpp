#include "../../NetworkCore/NetworkManager.h"
#include "UIEditText.h"
#include "../../Managers/InputManager.h"
#include "../../GraphicCore/Graphics.h"
#include "../../Timer.h"
#include "UIButton.h"
#include "../../Bitmap.h"

UIEditText::UIEditText(const UIEditText& _uiEditText) 
	: UI(_uiEditText)
{
	m_Callback = _uiEditText.m_Callback;
	m_strInput = _uiEditText.m_strInput;
	m_rectCursor = _uiEditText.m_rectCursor;
	m_showCursor = _uiEditText.m_showCursor;
	m_deltaTime = _uiEditText.m_deltaTime;
	m_pBitmap = _uiEditText.m_pBitmap;

	// 모든 UIEditText의 텍스트 크기와 색, 폰트는 고정
	Graphics::GetInst()->CreateTextFormat(L"Arial", 25.f, &m_pTextFormat);
	Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Black, &m_pBrush);
}

UIEditText::UIEditText(UI* _pParentPanel, UINT _width, UINT _height, int _inputMaxCount, float _xpos, float _ypos, FLOAT _pivotX, FLOAT _pivotY) :
	UI(_pParentPanel, _width, _height, _xpos, _ypos, _pivotX, _pivotY), m_inputMaxCount(_inputMaxCount)
{
	m_rectCursor = { m_tPos.x + 0.f, m_tPos.y + 0.f, m_tPos.x + 1.f, m_tPos.y + (float)_height};

	Graphics::GetInst()->CreateTextFormat(L"Arial", 25.f, &m_pTextFormat);
	Graphics::GetInst()->CreateSolidColorBrush(D2D1::ColorF::Black, &m_pBrush);
}

UIEditText::~UIEditText()
{
	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pBrush) m_pBrush->Release();
}

void UIEditText::Update()
{
	UI::Update();

	if (!m_bFocus) return;

	const std::vector<WPARAM>& vecKeyInput = InputManager::GetInst()->GetVecKeyInput();

	size_t size = vecKeyInput.size();
	for (size_t i = 0; i < size; i++)
	{
		WPARAM key = vecKeyInput[i];
		size_t curStrInputSize = m_strInput.size();
		if (key == VK_SPACE && curStrInputSize < m_inputMaxCount) m_strInput.append(L" ");
		else if (key == VK_BACK)
		{
			if (curStrInputSize >= 1) m_strInput.pop_back();
		}

		else if (key == VK_RETURN)
		{
			m_Callback(this, m_strInput);
		}
		else if (char(33) <= key && key <= char(126))
		{
			if (curStrInputSize < m_inputMaxCount)
			{
				wchar_t wkey = static_cast<wchar_t>(key);
				m_strInput.append(1, wkey);
			}
		}
	}
	 // InputManager는 문자키만 받도록 설정
	if (size > 0)
	{
		IDWriteTextLayout* pLayout = nullptr;
		HRESULT hr = Graphics::GetInst()->GetDwriteFactory()->CreateTextLayout(m_strInput.c_str(), (int)m_strInput.length(), m_pTextFormat, (FLOAT)m_size.width, (FLOAT)m_size.height, &pLayout);

		if (SUCCEEDED(hr))
		{
			DWRITE_TEXT_METRICS metrics;
			hr = pLayout->GetMetrics(&metrics);

			if (SUCCEEDED(hr))
			{
				m_rectCursor.left = m_tPos.x + metrics.widthIncludingTrailingWhitespace;
				m_rectCursor.right = m_tPos.x + metrics.widthIncludingTrailingWhitespace;
			}
			pLayout->Release();
		}
	}

	m_deltaTime += Timer::GetInst()->GetDeltaTime();
	if (m_deltaTime >= 0.5f)
	{
		m_showCursor = !m_showCursor;
		m_deltaTime = 0.f;
	}
}

void UIEditText::Render()
{
	ID2D1HwndRenderTarget* pRenderTarget = Graphics::GetInst()->GetRenderTarget();

	if (m_pBitmap)		pRenderTarget->DrawBitmap(m_pBitmap->GetBitmap(), m_rect);

	if(m_strInput.length() > 0)
		pRenderTarget->DrawTextW(m_strInput.c_str(), (int)m_strInput.length(), m_pTextFormat, m_rect, m_pBrush);

	if (m_bFocus)
	{
		pRenderTarget->DrawRectangle(m_rect, m_pBrush, 5.f);
		if (m_showCursor)
		{
			pRenderTarget->DrawRectangle(m_rectCursor, m_pBrush, 3.f);
		}
	}
}

void UIEditText::ClearEditText()
{
	m_strInput.clear();
	m_rectCursor = { m_tPos.x + 0.f, m_tPos.y + 0.f, m_tPos.x + 1.f, m_tPos.y + (float)m_size.height};
}