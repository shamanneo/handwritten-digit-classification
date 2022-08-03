#pragma once

class CPaintBoardWnd : public CWindowImpl<CPaintBoardWnd>
{ 
	private :
		POINT m_pt ; 
		HBITMAP m_hBitmap ; 
	public :
		CPaintBoardWnd() ; 
		~CPaintBoardWnd() ; 
	public :
		DECLARE_WND_CLASS(_T("PaintBoardWnd"))

		BEGIN_MSG_MAP(CPaintBoardWnd)
			MESSAGE_HANDLER(WM_CREATE, OnCreate) 
			MESSAGE_HANDLER(WM_COMMAND, OnCommand) 
			MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown) 
			MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove) 
			MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp) 
			MESSAGE_HANDLER(WM_PAINT, OnPaint) 
			MESSAGE_HANDLER(WM_DESTROY, OnDestory) 
		END_MSG_MAP()

	public :
		LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ; 
		LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
		LRESULT OnDestory(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled) ;
	public :
		bool SaveBitmap() ; // Save bitmap to local storage.
		void DeleteBitmap() ; 
} ;



