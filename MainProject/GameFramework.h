#pragma once
#include "Timer.h"
#include "ImageLoad.h"
#include "Player.h"

class CGameFramework
{
private:
	ID2D1Factory* m_pD2DFactory;
	ID2D1HwndRenderTarget* m_pRT;
	CTimer m_timer;

	CImageLoad m_imageFactory;
	CPlayer* m_pPlayer;
	std::vector<CGameObject*> m_objects;
	
public:
	CGameFramework();
	~CGameFramework();

	bool MultipleInstancePrevention();
	bool Initialize(HWND hWnd);
	void Update();
	void Draw();
	void KeyboardMessage(UINT message, WPARAM wParam);
	void Destroy();
	CTimer* GetTimer() { return &m_timer; }
};

