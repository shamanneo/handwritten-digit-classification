#include "pch.h"
#include "PaintBoardWnd.h"

CPaintBoardWnd::CPaintBoardWnd()
{
	m_pt.x = m_pt.y = 0 ; 
	m_hBitmap = NULL ; 
}

CPaintBoardWnd::~CPaintBoardWnd()
{
	DeleteBitmap() ; 
}

LRESULT CPaintBoardWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
{
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnCommand(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/)
{
	m_pt.x = GET_X_LPARAM(lParam) ; 
	m_pt.y = GET_Y_LPARAM(lParam) ; 
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnMouseMove(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL &/*bHandled*/)
{
	if(wParam & VK_LBUTTON)
	{
		HDC hDC = GetDC() ; 
		HPEN hPen = ::CreatePen(PS_SOLID, 10, RGB(0, 0, 0)) ; 
		HGDIOBJ hPrevPen = ::SelectObject(hDC, hPen) ; 
		::MoveToEx(hDC, m_pt.x, m_pt.y, NULL) ; 
		m_pt.x = GET_X_LPARAM(lParam) ; 
		m_pt.y = GET_Y_LPARAM(lParam) ; 
		::LineTo(hDC, m_pt.x, m_pt.y) ; 
		::SelectObject(hDC, hPrevPen) ; 
		::DeleteObject(hPen) ; 
		ReleaseDC(hDC) ; 
	}
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
	HDC hDC = GetDC() ; 
	HDC hMemDC = ::CreateCompatibleDC(hDC) ; 
	RECT rc ; 
	GetClientRect(&rc) ; 
	// If previous bitmap is still valid, delete this bitmap. If bitmap is NULL, just return.
	DeleteBitmap() ; 
	m_hBitmap = ::CreateCompatibleBitmap(hDC, rc.right - rc.left, rc.bottom - rc.top) ; 
	HBITMAP hPrevBitmap = reinterpret_cast<HBITMAP>((::SelectObject(hMemDC,  m_hBitmap))) ;
	::BitBlt(hMemDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hDC, 0, 0, SRCCOPY) ; 
	::SelectObject(hMemDC, hPrevBitmap) ; 
	ReleaseDC(hDC) ; 
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	PAINTSTRUCT pt ; 
	HDC hDC = BeginPaint(&pt) ; 
	if(m_hBitmap != NULL)
	{
		HDC hMemDC = ::CreateCompatibleDC(hDC) ; 
		RECT rc ; 
		GetClientRect(&rc) ; 
		HBITMAP hPrevBitmap = reinterpret_cast<HBITMAP>(::SelectObject(hMemDC,  m_hBitmap)) ; 
		::BitBlt(hDC, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hMemDC, 0, 0, SRCCOPY) ; 
		::SelectObject(hMemDC, hPrevBitmap) ; 
		::DeleteDC(hMemDC) ; 
	}
	EndPaint(&pt) ; 
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
	PostQuitMessage(0) ; 
	return 0 ; 
}

void CPaintBoardWnd::DeleteBitmap()
{
	if(m_hBitmap != NULL)
	{
		::DeleteObject(m_hBitmap) ; 
		m_hBitmap = NULL ; 
	}
	else 
	{
		return ; 
	}
}

bool CPaintBoardWnd::SaveBitmap()
{
	return true ; 
}