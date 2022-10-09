#pragma once
#include "stdafx.h"

class MainWindow
{
public:
	MainWindow();
	MainWindow(HINSTANCE hInstance, WCHAR* szTitle, WCHAR* szWindowClass);
	~MainWindow();

	void Show(int nCmdShow);
	void Update();
	void Resize(int width, int height);
	void Paint();

private:
	HWND _hWnd;
	//OpenGL rendering context
	HGLRC _hRc;

	void _InitOpenGL();
	void _DrawGL();
};

