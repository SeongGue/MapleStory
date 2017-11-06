// DirectX112DProject.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
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

// ���� ����:
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
HWND hWnd;
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

ID2D1Factory* pD2DFactory = NULL;
ID2D1HwndRenderTarget* pRT = NULL;
ID2D1SolidColorBrush* pWhiteBrush = NULL;
ID2D1Bitmap* bitmap = NULL;

CTimer timer;

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
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

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECTX112DPROJECT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
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

	//�⺻ �޽��� �����Դϴ�.
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
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
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
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

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

   //1. IWICImagingFactory��ü�� �����.
   CoInitialize(0); //COM �ʱ�ȭ
   hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
	   IID_PPV_ARGS(&pFactory));
   //2. ���Ϸκ��� ���ڴ��� �����.
   if (SUCCEEDED(hr))
   {
	   hr = pFactory->CreateDecoderFromFilename(L"background.png", NULL, GENERIC_READ,
		   WICDecodeMetadataCacheOnDemand, &pDecoder);
   }
   //3. ���ڴ��κ��� �������� �����Ѵ�.
   if (SUCCEEDED(hr))
   {
	   hr = pDecoder->GetFrame(0, &pSource);
   }
   //4. �����͸� ����� ���������� �ʱ�ȭ�Ѵ�.
   if (SUCCEEDED(hr))
   {
	   hr = pFactory->CreateFormatConverter(&pConverter);
   }
   if (SUCCEEDED(hr))
   {
	   hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA,
		   WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
   }
   //5.�����ͷκ��� D2D��Ʈ�� �̹����� �����
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
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
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
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
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

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
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
