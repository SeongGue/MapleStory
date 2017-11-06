// DirectX112DProject.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "DirectX112DProject.h"
#include "Timer.h"

#define MAX_LOADSTRING 100

template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease!= NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pWhiteBrush = NULL;
ID2D1Bitmap* bitmap = NULL;

CTimer timer;

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTX112DPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECTX112DPROJECT));

	MSG msg = { 0 };
	timer.Reset();
	RECT rc;
	GetClientRect(hWnd, &rc);
	HDC hdc = GetDC(hWnd);

	//기본 메시지 루프입니다.
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			if (timer.IsNextFrameTime())
			{
				pRT->BeginDraw();
				//pRT->SetTransform(D2D1::Matrix3x2F::Identity());
				//pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
				pRT->DrawBitmap(bitmap);
				pRT->EndDraw();
			}
		}
	}
	
    return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECTX112DPROJECT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECTX112DPROJECT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,
	   &pD2DFactory);
   RECT rc;
   GetClientRect(hWnd, &rc);
   hr = pD2DFactory->CreateHwndRenderTarget(
	   D2D1::RenderTargetProperties(),
	   D2D1::HwndRenderTargetProperties(hWnd,
		   D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),
	   &pRT);

   if (SUCCEEDED(hr))
   {
	   pRT->CreateSolidColorBrush(
		   D2D1::ColorF(D2D1::ColorF::White),
		   &pWhiteBrush
	   );
   }

   IWICImagingFactory* pFactory = NULL;
   IWICBitmapDecoder* pDecoder = NULL;
   IWICBitmapFrameDecode* pSource = NULL;
   IWICStream* pStream = NULL;
   IWICFormatConverter* pConverter = NULL;
   IWICBitmapScaler* pScalar = NULL;

   //1. IWICImagingFactory객체를 만든다.
   CoInitialize(0); //COM 초기화
   hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
	   IID_PPV_ARGS(&pFactory));
   //2. 파일로부터 디코더를 만든다.
   if (SUCCEEDED(hr))
   {
	   hr = pFactory->CreateDecoderFromFilename(L"background.png", NULL, GENERIC_READ,
		   WICDecodeMetadataCacheOnDemand, &pDecoder);
   }
   //3. 디코더로부터 프레임을 추출한다.
   if (SUCCEEDED(hr))
   {
	   hr = pDecoder->GetFrame(0, &pSource);
   }
   //4. 컨버터를 만들어 프레임으로 초기화한다.
   if (SUCCEEDED(hr))
   {
	   hr = pFactory->CreateFormatConverter(&pConverter);
   }
   if (SUCCEEDED(hr))
   {
	   hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA,
		   WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
   }
   //5.컨버터로부터 D2D비트맵 이미지를 만든다
   if (SUCCEEDED(hr))
   {
	   hr = pRT->CreateBitmapFromWicBitmap(pConverter, NULL, &bitmap);
   }

   pFactory->Release();
   pDecoder->Release();
   pSource->Release();
   pConverter->Release();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다.
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		pD2DFactory->Release();
		pRT->Release();
		pWhiteBrush->Release();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
