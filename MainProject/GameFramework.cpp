#include "stdafx.h"
#include "GameFramework.h"


CGameFramework::CGameFramework()
{
	m_pD2DFactory = NULL;
	m_pRT = NULL;
	m_pPlayer = NULL;
	m_objects.reserve(1);
}


CGameFramework::~CGameFramework()
{
}

bool CGameFramework::MultipleInstancePrevention()
{
	HANDLE mutex;
	mutex = CreateMutex(NULL, true, _T("You ran the program multiple times."));
	
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return false;
	
	return true;
}

bool CGameFramework::Initialize(HWND hWnd)
{
	if (!MultipleInstancePrevention())
		return false;

	m_timer.Reset();

	//D2DFactory 초기화
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&m_pD2DFactory);

	//RederTarget초기화
	RECT rc;
	GetClientRect(hWnd, &rc);
	hr = m_pD2DFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hWnd,
			D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
		&m_pRT);

	//imageLoad클래스 초기화
	if (!m_imageFactory.Initialize())
		return false;
	//이미지 파일 로드
	CSprite sprite;
	
	if (!m_imageFactory.LoadGameImages(m_pRT, std::string("SpriteData/Character.txt"), _T("SpriteData/Character.png"), &sprite))
		return false;
	m_pPlayer = new CPlayer(&sprite);
	//m_objects.push_back(player);

	//imageLoad클래스 소멸
	m_imageFactory.Destroy();
	
	return true;
}

void CGameFramework::Update()
{
}

void CGameFramework::Draw()
{
	m_pRT->BeginDraw();
	m_pRT->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
	m_pPlayer->Draw(m_pRT);
	for(auto obj : m_objects)
		obj->Draw(m_pRT);
	m_pRT->EndDraw();
}

void CGameFramework::KeyboardMessage(UINT message, WPARAM wParam)
{
	m_pPlayer->KeyboardMessage(message, wParam);
}

void CGameFramework::Destroy()
{
	SafeRelease(&m_pD2DFactory);
	SafeRelease(&m_pRT);

	SafeDelete(m_pPlayer);
	for (auto obj : m_objects)
		SafeDelete(obj);
}