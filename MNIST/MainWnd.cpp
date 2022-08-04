#include "pch.h"
#include "MainWnd.h"

CMainWnd::CMainWnd()
{

}

CMainWnd::~CMainWnd()
{

}

LRESULT CMainWnd::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
    const DWORD dwChildWndStyle = WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS ; 
    const int nSize = 280 ; 
    RECT rcPaintBoardWnd { 1, 1, nSize, nSize } ; 
    RECT rcResWnd { nSize, 1, nSize + 49 * 2 + nSize, 49 * 2 + nSize } ; 
    RECT rcClearButtonWnd { 1, nSize + 1, nSize + 1, nSize + 50 } ; 
    int nCleraButtonWndHeight = (nSize + 50) - (nSize + 1) ; 
    RECT rcRunButtonWnd { 1, nSize + 1 + nCleraButtonWndHeight, nSize + 1, nSize + 50 + nCleraButtonWndHeight } ; 
    // Create child windows.
    m_paintBoardWnd.Create(m_hWnd, &rcPaintBoardWnd, NULL, dwChildWndStyle | WS_BORDER) ; 
    m_resWnd.Create(m_hWnd, &rcResWnd, NULL, dwChildWndStyle | WS_BORDER) ; 
    // Create controls.
    m_ctlClearButton.Create(_T("button"), m_hWnd, &rcClearButtonWnd, _T("Clear"), dwChildWndStyle | BS_PUSHBUTTON, 
        NULL, IDC_CLEAR_BUTTON, NULL) ; 
    m_ctlRunButton.Create(_T("button"), m_hWnd, &rcRunButtonWnd, _T("Run"), dwChildWndStyle | BS_PUSHBUTTON, 
        NULL, IDC_RUN_BUTTON, NULL) ; 
    return 0 ;
}

LRESULT CMainWnd::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) 
{
    return 0 ; 
}

LRESULT CMainWnd::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
    PAINTSTRUCT ps ; 
    HDC hDC = BeginPaint(&ps) ; 

    UNREFERENCED_PARAMETER(hDC) ; 
    
    EndPaint(&ps) ; 
    return 0 ; 
}

LRESULT CMainWnd::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/) 
{
    return 0 ; 
}

LRESULT CMainWnd::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL &/*bHandled*/) 
{
    return 0 ; 
}

LRESULT CMainWnd::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL &/*bHandled*/)
{
    PostQuitMessage(0) ; 
    return 0 ; 
}

// Command message handler func.

LRESULT CMainWnd::OnNMClickClearButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL &/*bHandled*/)
{
    m_paintBoardWnd.DeleteBitmap() ; 
    ::InvalidateRect(m_paintBoardWnd, NULL, TRUE) ; 
    ::UpdateWindow(m_paintBoardWnd) ;
    return 0 ; 
}

LRESULT CMainWnd::OnNMClickCRunButton(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL &/*bHandled*/)
{   
    if(m_paintBoardWnd.SaveBitmap())
    {
        Classify() ; 
    }
    else 
    {
        ATLASSERT(0) ; 
    }
    return 0 ; 
}

//      protected

void CMainWnd::Classify() 
{
    std::string pred ;

    init_apartment() ;
    m_ml.LoadModel();
    m_ml.LoadImageFile() ; 
    m_ml.BindModel() ; 
    m_ml.EvaluateModel(pred) ;
    
    m_resWnd.SetPred(pred) ; 
}