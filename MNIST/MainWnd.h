#pragma once
#include "PaintBoardWnd.h" 
#include "ResWnd.h" 

class CMainWnd : public CWindowImpl<CMainWnd> 
{
    private :
        // Child windows.
        CPaintBoardWnd m_paintBoardWnd ; 
        CResWnd m_resWnd ; 
    private :
        // Controls.
		CWindow m_ctlClearButton ; // ID : IDC_CLEAR_BUTTON.
        CWindow m_ctlRunButton ; // ID : IDC_RUN_BUTTON.
    public :
        CMainWnd() ; 
        ~CMainWnd() ; 
    public :
        DECLARE_WND_CLASS(_T("MainWnd"))

        BEGIN_MSG_MAP(CMainWnd)

            // IDC_CLEAR_BUTTON command messeage handler.
            COMMAND_HANDLER(IDC_CLEAR_BUTTON, BN_CLICKED, OnNMClickClearButton) 
            COMMAND_HANDLER(IDC_RUN_BUTTON, BN_CLICKED, OnNMClickCRunButton) 
            // Main window message handler.
            MESSAGE_HANDLER(WM_CREATE, OnCreate) 
            MESSAGE_HANDLER(WM_COMMAND, OnCommand) 
            MESSAGE_HANDLER(WM_PAINT, OnPaint) 
            MESSAGE_HANDLER(WM_SIZE, OnSize) 
            MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown) 
            MESSAGE_HANDLER(WM_DESTROY, OnDestroy) 

        END_MSG_MAP() 

    public :
        LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ; 
        LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
        LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
        LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
        LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
        LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
    public :
        LRESULT OnNMClickClearButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled) ;
        LRESULT OnNMClickCRunButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled) ;
        
} ;