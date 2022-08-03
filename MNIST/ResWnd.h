#pragma once

class CResWnd : public CWindowImpl<CResWnd>
{ 
	public :
		CResWnd() ; 
		~CResWnd() ; 
	public :
		DECLARE_WND_CLASS(_T("ResWnd"))

		BEGIN_MSG_MAP(CResWnd)
			MESSAGE_HANDLER(WM_CREATE, OnCreate) 
			MESSAGE_HANDLER(WM_COMMAND, OnCommand) 
			MESSAGE_HANDLER(WM_PAINT, OnPaint) 
			MESSAGE_HANDLER(WM_DESTROY, OnDestory) 
		END_MSG_MAP()

	public :
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
} ;

