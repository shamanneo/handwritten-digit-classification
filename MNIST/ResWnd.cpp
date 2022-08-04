#include "pch.h"
#include "ResWnd.h"

CResWnd::CResWnd()
{
	m_sentence = _T("The model prediction value is :") ; 
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

	TextOut(hDC, 0, 0, m_sentence, m_sentence.GetLength()) ; 

	EndPaint(&pt) ; 
	return 0 ; 
}

LRESULT CResWnd::OnDestory(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
	PostQuitMessage(0) ; 
	return 0 ; 
}

void CResWnd::SetPred(std::string &pred) 
{
	m_sentence = _T("The model prediction value is :") ; 
	m_pred = pred.c_str() ; 
	m_sentence.Append(m_pred) ; 
	InvalidateRect(NULL, TRUE) ; 
	UpdateWindow() ; 
}
