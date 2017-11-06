// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <tchar.h>
#include <iostream>
#include <d2d1.h>
#include <wincodec.h>
#include <string>
#include <fstream>
#include <vector>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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