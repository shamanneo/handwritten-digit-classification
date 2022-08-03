#include "pch.h"
#include "ResWnd.h"

CResWnd::CResWnd()
{

}

CResWnd::~CResWnd()
{

}

LRESULT CResWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
{
	return 0 ; 
}

LRESULT CResWnd::OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
	return 0 ; 
}

LRESULT CResWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
{
	PAINTSTRUCT pt ; 
	HDC hDC = BeginPaint(&pt) ; 

	UNREFERENCED_PARAMETER(hDC) ; 

	EndPaint(&pt) ; 
	return 0 ; 
}

LRESULT CResWnd::OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
	PostQuitMessage(0) ; 
	return 0 ; 
}
