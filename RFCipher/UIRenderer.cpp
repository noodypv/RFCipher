#include "UIRenderer.h"

void UIRenderer::InitResources() {
	HRESULT hr;

	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pD2DFactory);

	if (!SUCCEEDED(hr))
		ExitProcess(EXIT_FAILURE);

	D2D1_SIZE_U size = D2D1::SizeU(this->pApp->m_appWidth, this->pApp->m_appHeight);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
		0, 0, D2D1_RENDER_TARGET_USAGE_NONE, D2D1_FEATURE_LEVEL_DEFAULT
	);

	hr = this->pD2DFactory->CreateDCRenderTarget(&props, &this->pRT);

	if (!SUCCEEDED(hr))
		ExitProcess(EXIT_FAILURE);

	hr = pRT->CreateSolidColorBrush(
		this->backgroundColor,
		&this->pBackgroundBrush
	);

	if (!SUCCEEDED(hr))
		ExitProcess(EXIT_FAILURE);

	hr = pRT->CreateSolidColorBrush(
		this->defControlColor,
		&this->pControlBrush
	);

	if (!SUCCEEDED(hr))
		ExitProcess(EXIT_FAILURE);

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&pDWriteFactory));

	if (!SUCCEEDED(hr))
		ExitProcess(EXIT_FAILURE);
}

void UIRenderer::DrawUI(const int& wParam, LPDRAWITEMSTRUCT lpds) {

	this->pRT->BindDC(lpds->hDC, &lpds->rcItem);

	this->pRT->BeginDraw();

	switch (wParam) {
		case static_cast<int>(RFCipher::CONTROLS_ID::ENCODE_BTN):
		{

			// Drawing button's background

			this->OnHover(lpds);


			// Drawing the text
			this->pDWriteFactory->CreateTextFormat(
				L"Roboto",
				nullptr,
				DWRITE_FONT_WEIGHT_MEDIUM,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				15.0f,
				L"",
				&this->pTextFormat
			);

			this->pRT->DrawText(L"ENCODE", strlen("ENCODE"), this->pTextFormat, D2D1::RectF(58.0f, 9.0f, 170.0f, 40.0f), this->pControlBrush);
			this->pControlBrush->SetColor(this->defControlColor);

			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::DECODE_BTN):
		{
			// Drawing button's background

			this->OnHover(lpds);

			// Drawing the text
			this->pDWriteFactory->CreateTextFormat(
				L"Roboto",
				nullptr,
				DWRITE_FONT_WEIGHT_MEDIUM,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				15.0f,
				L"",
				&this->pTextFormat
			);

			this->pRT->DrawText(L"DECODE", strlen("DECODE"), this->pTextFormat, D2D1::RectF(58.0f, 9.0f, 170.0f, 40.0f), this->pControlBrush);
			this->pControlBrush->SetColor(this->defControlColor);

			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::CLOSE_BTN):
		{
			this->pControlBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));

			this->pRT->FillRectangle(D2D1::RectF(static_cast<float>(lpds->rcItem.left), static_cast<float>(lpds->rcItem.top), static_cast<float>(lpds->rcItem.right), static_cast<float>(lpds->rcItem.bottom)), this->pBackgroundBrush);


			this->pRT->DrawLine(D2D1::Point2F(lpds->rcItem.left + 3.0f, lpds->rcItem.top + 3.0f), D2D1::Point2F(lpds->rcItem.right - 4.0f, lpds->rcItem.bottom - 4.0f),
								this->pControlBrush, 1.0f);

			this->pRT->DrawLine(D2D1::Point2F(lpds->rcItem.left + 3.0f, lpds->rcItem.bottom - 4.0f), D2D1::Point2F(lpds->rcItem.right - 4.0f, lpds->rcItem.top + 3.0f),
				this->pControlBrush, 1.0f);

			this->pControlBrush->SetColor(this->defControlColor);

			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::ROLL_BTN):
		{
			this->pControlBrush->SetColor({1.0f, 1.0f, 1.0f, 1.0f});

			this->pRT->FillRectangle(D2D1::RectF(static_cast<float>(lpds->rcItem.left), static_cast<float>(lpds->rcItem.top), static_cast<float>(lpds->rcItem.right), static_cast<float>(lpds->rcItem.bottom)), this->pBackgroundBrush);

			this->pRT->DrawLine(D2D1::Point2F(static_cast<float>(lpds->rcItem.left) + 3.0f, static_cast<float>(lpds->rcItem.bottom) - 4.0f), D2D1::Point2F(static_cast<float>(lpds->rcItem.right) - 3.0f, static_cast<float>(lpds->rcItem.bottom) - 4.0f),
				this->pControlBrush, 1.5f);

			this->pControlBrush->SetColor(this->defControlColor);
			break;
		}
	}
	this->pRT->EndDraw();
}

void UIRenderer::OnHover(LPDRAWITEMSTRUCT lpds) {
	// Drawing button's background

	switch (lpds->itemAction) {
	case ODA_FOCUS:
	{
		if (lpds->itemState & ODS_FOCUS) {
			this->pControlBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
			this->pRT->FillRectangle(D2D1::RectF(static_cast<float>(this->pApp->m_windowRect.left) + 1.0f, static_cast<float>(this->pApp->m_windowRect.top) + 1.0f, static_cast<float>(this->pApp->m_windowRect.right) - 1.0f, static_cast<float>(this->pApp->m_windowRect.bottom) - 1.0f),
				this->pControlBrush);
			this->pControlBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
		}
		break;
	}
	case ODA_DRAWENTIRE:
	{
		this->pRT->FillRectangle(D2D1::RectF(static_cast<float>(lpds->rcItem.left) + 2.0f, static_cast<float>(lpds->rcItem.top) + 2.0f, static_cast<float>(lpds->rcItem.right) - 2.0f, static_cast<float>(lpds->rcItem.bottom) - 2.0f),
			this->pBackgroundBrush);
		this->pRT->DrawRectangle(D2D1::RectF(static_cast<float>(lpds->rcItem.left), static_cast<float>(lpds->rcItem.top), static_cast<float>(lpds->rcItem.right), static_cast<float>(lpds->rcItem.bottom)), this->pControlBrush);
		this->pControlBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	}
	}
}

void UIRenderer::OnPaintMainWindow() {

	this->pRT->BindDC(this->pApp->m_windowHDC, &this->pApp->m_windowRect);
	this->pRT->BeginDraw();

	// Painting the background
	this->pRT->FillRectangle(
		D2D1::RectF(this->pApp->m_windowRect.left, this->pApp->m_windowRect.top,
			this->pApp->m_windowRect.right, this->pApp->m_windowRect.bottom),
		this->pBackgroundBrush
	);

	// Drawing the text
	this->pDWriteFactory->CreateTextFormat(
		L"Roboto",
		nullptr,
		DWRITE_FONT_WEIGHT_BOLD,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		30.0f,
		L"",
		&this->pTextFormat
	);

	this->pControlBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	this->pRT->DrawText(L"RAIL FENCE CIPHER", strlen("RAIL FENCE CIPHER"), this->pTextFormat, D2D1::RectF(110.0f, 50.0f, 475.0f, 45.0f), this->pControlBrush);
	this->pControlBrush->SetColor(this->defControlColor);

	this->pRT->EndDraw();
}

UIRenderer::UIRenderer(RFCipher* pApp) : pApp(pApp) {
	InitResources();
}

UIRenderer::~UIRenderer() {
	this->ReleaseObj(this->pBackgroundBrush);
	this->ReleaseObj(this->pControlBrush);
	this->ReleaseObj(this->pRT);
	this->ReleaseObj(this->pD2DFactory);

	this->ReleaseObj(this->pDWriteFactory);
	this->ReleaseObj(this->pTextFormat);
}