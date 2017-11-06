#include "stdafx.h"
#include "ImageLoad.h"


CImageLoad::CImageLoad()
{
	m_pFactory = NULL;
	m_pDecoder = NULL;
	m_pSource = NULL;
	m_pConverter = NULL;
}

CImageLoad::~CImageLoad()
{
}

bool CImageLoad::Initialize()
{
	//COM 초기화
	CoInitialize(0); 
	//IWICImagingFactory객체 생성.
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL,
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_pFactory));

	if (FAILED(hr))
		return false;
	return true;
}

bool CImageLoad::AnalysisAnimTextFile(std::string textFileName, CSprite* sprite)
{
	std::ifstream f;
	f.open(textFileName.c_str(), std::ios::in);

	int index = 0;
	while (!f.eof())
	{
		std::string s;
		f >> s;
		if (!s.empty()) //문자열이 비었으면 리턴
		{
			std::string analysisString = s.substr(0, s.find("="));
			int num = atoi(s.substr(s.find("=") + 1).c_str());

			if (analysisString == "animType") {
				sprite->SetAnimType(num);
				sprite->AllocAnimCnt();
			}
			else if (analysisString == "animCnt") {
				sprite->SetAnimCnt(index - 1, num);
			}
			else if (analysisString == "width") {
				sprite->SetWidth(num);
			}
			else if (analysisString == "height") {
				sprite->SetHeight(num);
			}
			else
				return false;
		}
		index++;
	}
	sprite->CalcAnimSize();
	return true;
}

bool CImageLoad::LoadGameImages(ID2D1HwndRenderTarget* pRT, std::string textFileName, TCHAR* imageFileName, CSprite* sprite)
{
	//900번 이상 호출되면 오류 발생
	HRESULT hr;
	ID2D1Bitmap* bitmap = NULL;
	//1. 애니메이션 텍스트 파일을 분석한다.
	AnalysisAnimTextFile(textFileName, sprite);

	//2. 파일로부터 디코더를 만든다.
	hr = m_pFactory->CreateDecoderFromFilename(imageFileName, NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, &m_pDecoder);

	//3. 디코더로부터 프레임을 추출한다.
	if (SUCCEEDED(hr))
	{
		hr = m_pDecoder->GetFrame(0, &m_pSource);
	}
	//4. 컨버터를 만들어 프레임으로 초기화한다.
	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreateFormatConverter(&m_pConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pConverter->Initialize(m_pSource, GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
	}
	//5. 컨버터로부터 D2D비트맵 이미지를 만든다
	if (SUCCEEDED(hr))
	{
		hr = pRT->CreateBitmapFromWicBitmap(m_pConverter, NULL, &bitmap);
	}
	//6. 사용한 자원을 해제시켜준다.
	SafeRelease(&m_pDecoder);
	SafeRelease(&m_pSource);
	SafeRelease(&m_pConverter);

	if (FAILED(hr))
		return false;

	sprite->SetSprite(bitmap);

	return true;
}

void CImageLoad::Destroy()
{
	SafeRelease(&m_pFactory);
}
