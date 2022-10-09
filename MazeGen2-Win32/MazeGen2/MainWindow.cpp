#include "stdafx.h"
#include "MainWindow.h"

#include<GL/glew.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <exception>
#include <iostream>

MainWindow::MainWindow()
	:_hWnd(0), _hRc(0)
{

}

MainWindow::MainWindow(HINSTANCE hInstance, WCHAR* szTitle, WCHAR* szWindowClass)
{
	_hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!_hWnd)
		throw std::exception("HWIND failed to load");

	_InitOpenGL();

}


MainWindow::~MainWindow()
{
}

void MainWindow::Show(int nCmdShow)
{
	ShowWindow(_hWnd, nCmdShow);
}

void MainWindow::Update()
{
	UpdateWindow(_hWnd);
}

void MainWindow::Resize(int width, int height)
{
	int midDim = width < height ? width : height;
	glViewport((width - midDim) / 2, (height - midDim) / 2, midDim, midDim);
}

void MainWindow::Paint()
{
	//Do OpenGL drawing
	//cache any current targeted contexts
	HDC hOldGLDC = wglGetCurrentDC();
	HGLRC hOldGLRC = wglGetCurrentContext();

	HDC hdc = GetDC(_hWnd);

	//set our context as target for drawing; all OpenGL draw calls from this point
	//on will go to our context until it is changed.
	wglMakeCurrent(hdc, _hRc);

	//this is where draw code goes
	_DrawGL();

	//OpenGL should always be double buffered if possible. Swap buffers to update after drawing
	SwapBuffers(hdc);
	//release draw context, restore previous contexts
	ReleaseDC(_hWnd, hdc);
	wglMakeCurrent(hOldGLDC, hOldGLRC);
}

void MainWindow::_InitOpenGL()
{
	//grab context
	HDC hDc = GetDC(_hWnd);

	/*The following structure defines attributes to produce an OGL context
	The specific settings for this piece will be up to the VISTAS folk. We
	Should ask them what they need.
	*/
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1.0;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	//ask Windows to give us the closest pixelformat to what is requested above
	int iFormat = ChoosePixelFormat(hDc, &pfd);
	//set pixel format for draw context
	SetPixelFormat(hDc, iFormat, &pfd);
	//create OpenGL Rendering context
	_hRc = wglCreateContext(hDc);

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		std::cout << "Error loading GLEW: " << glewGetErrorString(err) << std::endl;
	}

	//initial flags
	wglMakeCurrent(hDc, _hRc);

	ReleaseDC(_hWnd, hDc);
}

void MainWindow::_DrawGL()
{
	glClearColor(0., 0., 0., 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
