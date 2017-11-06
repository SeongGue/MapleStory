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
	//COM �ʱ�ȭ
	CoInitialize(0); 
	//IWICImagingFactory��ü ����.
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
		if (!s.empty()) //���ڿ��� ������� ����
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
	//900�� �̻� ȣ��Ǹ� ���� �߻�
	HRESULT hr;
	ID2D1Bitmap* bitmap = NULL;
	//1. �ִϸ��̼� �ؽ�Ʈ ������ �м��Ѵ�.
	AnalysisAnimTextFile(textFileName, sprite);

	//2. ���Ϸκ��� ���ڴ��� �����.
	hr = m_pFactory->CreateDecoderFromFilename(imageFileName, NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, &m_pDecoder);

	//3. ���ڴ��κ��� �������� �����Ѵ�.
	if (SUCCEEDED(hr))
	{
		hr = m_pDecoder->GetFrame(0, &m_pSource);
	}
	//4. �����͸� ����� ���������� �ʱ�ȭ�Ѵ�.
	if (SUCCEEDED(hr))
	{
		hr = m_pFactory->CreateFormatConverter(&m_pConverter);
	}
	if (SUCCEEDED(hr))
	{
		hr = m_pConverter->Initialize(m_pSource, GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
	}
	//5. �����ͷκ��� D2D��Ʈ�� �̹����� �����
	if (SUCCEEDED(hr))
	{
		hr = pRT->CreateBitmapFromWicBitmap(m_pConverter, NULL, &bitmap);
	}
	//6. ����� �ڿ��� ���������ش�.
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
