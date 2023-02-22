#include "RFCipher.h"

bool RFCipher::RegisterWndClass() {
	HRESULT hr = CoInitialize(NULL);
	if (!SUCCEEDED(hr))
		return false;

	WNDCLASSEX wcex{ sizeof(WNDCLASSEX) };

	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = nullptr;
	wcex.lpszClassName = this->m_appTitle.c_str();
	wcex.hbrBackground = nullptr;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.lpfnWndProc = RFCipher::AppWndProc;
	wcex.lpszMenuName = nullptr;
	wcex.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassEx(&wcex)) {
		return false;
	}

	this->hMainWindow = CreateWindowEx(0, this->m_appTitle.c_str(), this->m_appTitle.c_str(), WS_POPUP,
		(GetSystemMetrics(SM_CXSCREEN) - this->m_appWidth) / 2, (GetSystemMetrics(SM_CYSCREEN) - this->m_appHeight) / 2,
		this->m_appWidth, this->m_appHeight, nullptr, nullptr, nullptr, this);

	if (!this->hMainWindow)
		return false;

	this->renderer = new UIRenderer{ this };

	ShowWindow(this->hMainWindow, SW_SHOW);
	UpdateWindow(this->hMainWindow);

	return true;
}

LRESULT RFCipher::AppWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	RFCipher* pApp;

	if (uMsg == WM_NCCREATE) {
		pApp = static_cast<RFCipher*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
		SetLastError(0);
		if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pApp))) {
			if (GetLastError() != 0) {
				return false;
			}
		}
	}
	else {
		pApp = reinterpret_cast<RFCipher*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	}

	if (pApp) {
		pApp->hMainWindow = hwnd;
		return pApp->WindowProc(hwnd, uMsg, wParam, lParam);
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT RFCipher::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
		case WM_CREATE:
		{
			this->CreateControls();
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		case WM_PAINT:
		case WM_DISPLAYCHANGE:
		{
			PAINTSTRUCT ps{};
			BeginPaint(hwnd, &ps);

			if (hwnd == this->hMainWindow) {
				this->m_windowHDC = ps.hdc;
				this->m_windowRect = ps.rcPaint;

				this->renderer->OnPaintMainWindow();
			}

			EndPaint(hwnd, &ps);
			return 0;
		}

		case WM_DRAWITEM:
		{
			this->renderer->DrawUI(wParam, reinterpret_cast<LPDRAWITEMSTRUCT>(lParam));
			break;
		}

		case WM_COMMAND:
		{
			this->OnClick(LOWORD(wParam));
			break;
		}

		case WM_LBUTTONDOWN:
		{
			this->OnLButtonClick();
			break;
		}

		case WM_SETCURSOR:
		{
			if (reinterpret_cast<HWND>(wParam) == this->hEncodeButton) {
				SetFocus(this->hEncodeButton);
			}
			else if (reinterpret_cast<HWND>(wParam) == this->hDecodeButton){
				SetFocus(this->hDecodeButton);
			}
			else {
				SetFocus(this->hEdit);
			}
			break;
		}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

RFCipher::RFCipher() {
	if (!this->RegisterWndClass()) {
		ExitProcess(EXIT_FAILURE);
	}
}

int RFCipher::Run() {
	MSG msg{};

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int>(msg.wParam);
}

void RFCipher::CreateControls() {
	this->hEncodeButton = CreateWindow(L"BUTTON", L"ADD", 
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, 
										50, 160, 185, 40,
										this->hMainWindow, reinterpret_cast<HMENU>(RFCipher::CONTROLS_ID::ENCODE_BTN), nullptr, nullptr
	);

	this->hDecodeButton = CreateWindow(L"BUTTON", L"ADD",
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		250, 160, 180, 40,
		this->hMainWindow, reinterpret_cast<HMENU>(RFCipher::CONTROLS_ID::DECODE_BTN), nullptr, nullptr
	);

	this->hCloseButton = CreateWindow(L"BUTTON", L"CLOSE",
										WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
										478, 5, 20, 20,
										this->hMainWindow, reinterpret_cast<HMENU>(RFCipher::CONTROLS_ID::CLOSE_BTN), nullptr, nullptr
	);

	this->hRollButton = CreateWindow(L"BUTTON", L"ROLL",
		WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
		450, 5, 20, 20,
		this->hMainWindow, reinterpret_cast<HMENU>(RFCipher::CONTROLS_ID::ROLL_BTN), nullptr, nullptr
	);

	this->hEdit = CreateWindow(L"EDIT", L"",
		WS_VISIBLE | WS_CHILD | ES_MULTILINE | ES_AUTOHSCROLL,
		50, 120, 380, 30,
		this->hMainWindow, reinterpret_cast<HMENU>(RFCipher::CONTROLS_ID::TEXT_EDIT), nullptr, nullptr
	);

	this->hRailsCountBox = CreateWindow(L"COMBOBOX", nullptr,
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		435, 120, 40, 30,
		this->hMainWindow, reinterpret_cast<HMENU>(RFCipher::CONTROLS_ID::RAILSCOUNT_BOX), nullptr, nullptr
		);

	SendMessage(this->hRailsCountBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("2"));
	SendMessage(this->hRailsCountBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("3"));
	SendMessage(this->hRailsCountBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("4"));
	SendMessage(this->hRailsCountBox, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>("5"));
	SendMessage(this->hRailsCountBox, CB_SETCURSEL, 1, 0);
	this->choosenCountOfRails = SendMessage(this->hRailsCountBox, CB_GETCURSEL, 0, 0) + 2;

}

void RFCipher::OnClick(const int& btnId) {
	switch (btnId) {
		case static_cast<int>(RFCipher::CONTROLS_ID::CLOSE_BTN):
		{
			PostQuitMessage(0);
			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::ROLL_BTN):
		{
			CloseWindow(this->hMainWindow);
			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::RAILSCOUNT_BOX):
		{
			this->choosenCountOfRails = SendMessage(this->hRailsCountBox, CB_GETCURSEL, 0, 0) + 2;
			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::ENCODE_BTN):
		{
			int length = GetWindowTextLength(this->hEdit) + 1;

			if (length - 1) {
				char* buffer = new char[length];

				GetWindowTextA(this->hEdit, buffer, length);
				SetWindowTextA(this->hEdit, this->RailFenceEncode(buffer, length).c_str());

				delete[] buffer;
			}
			break;
		}

		case static_cast<int>(RFCipher::CONTROLS_ID::DECODE_BTN):
		{
			int length = GetWindowTextLength(this->hEdit) + 1;

			if (length - 1) {
				char* buffer = new char[length];

				GetWindowTextA(this->hEdit, buffer, length);
				SetWindowTextA(this->hEdit, this->RailFenceDecode(buffer, length).c_str());

				delete[] buffer;
			}
			break;
		}
	}
}

void RFCipher::OnLButtonClick() {
	ReleaseCapture();
	SendMessage(this->hMainWindow, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
}

RFCipher::~RFCipher() {
	delete this->renderer;
}