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
		HPEN hPen = ::CreatePen(PS_SOLID, 15, RGB(0, 0, 0)) ; 
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
	::DeleteDC(hMemDC) ; 
	ReleaseDC(hDC) ; 
	return 0 ; 
}

LRESULT CPaintBoardWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
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

LRESULT CPaintBoardWnd::OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
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
	RECT rc ; 
	GetClientRect(&rc) ; 
	const int nSize = 28 ; 
	HDC hDC = GetDC() ; 
	HDC hMemDCSrc = ::CreateCompatibleDC(hDC) ; 
	HDC hMemDCDst = ::CreateCompatibleDC(hDC) ; 
	HBITMAP hResizedBitmap = ::CreateCompatibleBitmap(hDC, nSize, nSize) ; 
	HBITMAP hPrevSrcBitmap = reinterpret_cast<HBITMAP>(::SelectObject(hMemDCSrc, m_hBitmap)) ; 
	HBITMAP hPrevDstBitmap = reinterpret_cast<HBITMAP>(::SelectObject(hMemDCDst, hResizedBitmap)) ; 
	int res = ::StretchBlt(hMemDCDst, 0, 0, nSize, nSize, hMemDCSrc, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SRCCOPY) ;
	if(!res)
	{
		ATLTRACE("StretchBlt function is failed.") ; 
		return false ; 
	}
	hResizedBitmap = reinterpret_cast<HBITMAP>(::SelectObject(hMemDCDst, hPrevDstBitmap)) ; 
	Gdiplus::Bitmap *pBitmap = Gdiplus::Bitmap::FromHBITMAP(hResizedBitmap, NULL) ; 
	::DeleteObject(hResizedBitmap) ; 
	InvertColor(pBitmap) ; // Convert black to white, white to black for model input.
	CLSID pngClsid ; 
	HRESULT hResult = ::CLSIDFromString(_T("{557cf406-1a04-11d3-9a73-0000f81ef32e}"), &pngClsid) ; 
	if(hResult == NOERROR)
	{
		Gdiplus::Status stat = pBitmap->Save(_T("C:\\Projects\\Handwritten-Digit-Classification\\MNIST\\input.png"), &pngClsid, NULL) ; 
		if(stat == Gdiplus::Ok)
		{
			ATLTRACE("The bitmap has been saved successfully!\n") ; 
		}
		else 
		{
			ATLTRACE("The bitmap has not been saved.\n") ; 
			return false ; 
		}
	}
	else
	{
		ATLTRACE("The CLSID was not obtained.\n") ; 
		return false ; 
	}
	::SelectObject(hMemDCSrc, hPrevSrcBitmap) ; 
	::DeleteDC(hMemDCDst) ; 
	::DeleteDC(hMemDCSrc) ; 
	ReleaseDC(hDC) ; 
	return true ; 
}

//		protected 

void CPaintBoardWnd::InvertColor(Gdiplus::Bitmap *pBitmap) 
{
	const int nSize = 28 ; 
	const Gdiplus::Color black { 0, 0, 0 } ; 
	const Gdiplus::Color white { 255, 255, 255 } ; 
	Gdiplus::Color color ; 
	for(int nX = 0 ; nX < nSize ; nX++)
	{
		for(int nY = 0 ; nY < nSize ; nY++)
		{
			pBitmap->GetPixel(nX, nY, &color) ; 
			if(color.GetValue() == color.White)
			{
				pBitmap->SetPixel(nX, nY, black) ; 
			}
			else 
			{
				pBitmap->SetPixel(nX, nY, white) ; 
			}
		}
	}
}