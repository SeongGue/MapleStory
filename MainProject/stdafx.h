// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <tchar.h>
#include <iostream>
#include <d2d1.h>
#include <wincodec.h>
#include <string>
#include <fstream>
#include <vector>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
template <class Interface>
inline void SafeRelease(Interface** ppInterfaceToRelease)
{
	if (*ppInterfaceToRelease != NULL)
	{
		(*ppInterfaceToRelease)->Release();

		(*ppInterfaceToRelease) = NULL;
	}
}

template <class Pointer>
inline void SafeDelete(Pointer* p)
{
	if (p != NULL)
	{
		delete p;
		p = NULL;
	}
}