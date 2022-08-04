#pragma once
#include "MainWnd.h"

class CMainApp
{
    private :
        CMainWnd m_MainWnd ; 
        private : // GDI+
        Gdiplus::GdiplusStartupInput m_gdiplusStartUpInput ; 
        ULONG_PTR m_gdiplusToken ; 
    public :
        CMainApp() ; 
        ~CMainApp() ; 
    public :
        int Run(HINSTANCE hInstance, int nCmdShow) ; 
    public :    
        static CMainApp &GetInstance() ; 
        static void Release() ; 
    public :
        bool PreTranslateMessage(MSG &msg) ; 
} ;