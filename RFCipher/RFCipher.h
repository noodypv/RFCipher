/*

	RFCipher - Application for encode/decode text using Rail Fence approach

	Author: NDY, 2023

*/

#pragma once

#include "APILibs.h"
#include "UIRenderer.h"

class UIRenderer;

class RFCipher {
private:

	HWND hMainWindow{};
	HWND hEncodeButton{};
	HWND hDecodeButton{};
	HWND hCloseButton{};
	HWND hRollButton{};
	HWND hEdit{};
	HWND hRailsCountBox{};

	RECT m_windowRect{};
	HDC m_windowHDC{};

	int m_appWidth{500};
	int m_appHeight{300};

	int choosenCountOfRails{};

	const std::wstring m_appTitle = L"NCore Application";

	UIRenderer* renderer{};

	friend class UIRenderer;

	enum class CONTROLS_ID {
		ENCODE_BTN,
		DECODE_BTN,
		CLOSE_BTN,
		ROLL_BTN,
		TEXT_EDIT,
		RAILSCOUNT_BOX
	};

private:

	bool RegisterWndClass();

	void CreateControls();

	static LRESULT CALLBACK AppWndProc(HWND, UINT, WPARAM, LPARAM);

	LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

	void OnClick(const int&);

	void OnLButtonClick();

public:
	explicit RFCipher();

	~RFCipher();

	int Run();

private:

	std::string RailFenceEncode(const char*, const int);

	std::string RailFenceDecode(const char*, const int);

};