#pragma once

#include "RFCipher.h"

class RFCipher;

class UIRenderer {
private:

	RFCipher* pApp{};

	ID2D1Factory* pD2DFactory{};
	ID2D1DCRenderTarget* pRT{};

	IDWriteFactory* pDWriteFactory{};
	IDWriteTextFormat* pTextFormat{};

	ID2D1SolidColorBrush* pBackgroundBrush{};
	ID2D1SolidColorBrush* pControlBrush{};

	const D2D1_COLOR_F backgroundColor{ 0.086f, 0.086f, 0.086f, 1.0f };
	const D2D1_COLOR_F defControlColor{ 0.2f, 0.2f, 0.2f, 1.0f };

	friend class RFCipher;

public:

	void DrawUI(const int&, LPDRAWITEMSTRUCT);

	void OnPaintMainWindow();

	void InitResources();

	UIRenderer(RFCipher*);

	~UIRenderer();

private:

	void OnHover(LPDRAWITEMSTRUCT);

	template<typename T>
	static void ReleaseObj(T& obj) {
		if (obj != nullptr) {
			obj->Release();
		}
		obj = nullptr;
	}
};
