#pragma once
#include "Sprite.h"

class CImageLoad
{
private:
	IWICImagingFactory* m_pFactory;
	IWICBitmapDecoder* m_pDecoder;
	IWICBitmapFrameDecode* m_pSource;
	IWICFormatConverter* m_pConverter;

public:
	CImageLoad();
	~CImageLoad();

	bool Initialize();
	bool AnalysisAnimTextFile(std::string textFileName, CSprite* sprite);
	bool LoadGameImages(ID2D1HwndRenderTarget* pRT, std::string textFileName, TCHAR* imageFileName, CSprite* sprite);
	void Destroy();
};

