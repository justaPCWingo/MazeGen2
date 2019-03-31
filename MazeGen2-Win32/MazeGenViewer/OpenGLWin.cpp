#include "OpenGLWin.h"
//#include<afxwin.h>
#include<random>
#include<chrono>
#include "VisMgr.h"
#include <glm/glm.hpp>

inline glm::vec3 Color2GLM(Color c)
{
	return glm::vec3(c.R / 255., c.G / 255., c.B / 255.);
}

inline Color GLM2Color(glm::vec3 c)
{
	return Color::FromArgb(c.r * 255, c.g * 255, c.b * 255);
}

OpenGLForm::OpenGLWin::OpenGLWin(System::Windows::Forms::Control ^ parentCtrl)
{
	
	m_bldr = new MazeBuilder(10, 10);

	CreateParams^ cp = gcnew CreateParams;

	// Set the position on the form
	cp->X = 0;
	cp->Y = 0;
	cp->Height = parentCtrl->Height;
	cp->Width = parentCtrl->Width;

	// Specify the form as the parent.
	cp->Parent = parentCtrl->Handle;

	// Create as a child of the specified parent and make OpenGL compliant (no clipping)
	cp->Style = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	// Create the actual window
	this->CreateHandle(cp);

	m_hDC = GetDC((HWND)this->Handle.ToPointer());

	if (m_hDC)
	{
		m_visMgr = new VisMgr("Shaders", cp->X,cp->Y, cp->Width,cp->Height);
		MySetPixelFormat(m_hDC);
		m_visMgr->InitForOpenGL();
		ReSizeGLScene(cp->Width, cp->Height);


		InitGL();

		m_bldr->GenerateMaze();
		std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<float> distribution(-0.1, 0.1);
		m_visMgr->NewMaze(m_bldr);
		m_visMgr->SetPathTime(0.0);
		m_visMgr->SetRotAxis(distribution(generator), distribution(generator), distribution(generator));
		
	}

}

GLvoid OpenGLForm::OpenGLWin::ReSizeGLScene(GLsizei width, GLsizei height)		// Resize and initialise the gl window
{
	if (this)
	{
		if (height == 0)										// Prevent A Divide By Zero By
		{
			height = 1;										// Making Height Equal One
		}
		SetWindowPos((HWND)this->Handle.ToPointer(), HWND_TOPMOST, 0, 0, width, height, SWP_NOZORDER);
		glViewport(0, 0, width, height);						// Reset The Current Viewport

																//TODO: add glm matrix stuff here
		m_visMgr->SetViewport(0, 0, width, height);

	}
}

System::Void OpenGLForm::OpenGLWin::Render(System::Void)
{
	if (m_visMgr->ReadyToDraw())
		m_visMgr->Draw();
	else
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

System::Void OpenGLForm::OpenGLWin::SwapOpenGLBuffers(System::Void)
{
	SwapBuffers(m_hDC);
}

System::Void OpenGLForm::OpenGLWin::RefreshWithBuilder()
{
	m_bldr->GenerateMaze();
	//random stuff
	std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<float> distribution(-0.1, 0.1);
	m_visMgr->RefreshWithMaze(m_bldr);
	m_visMgr->SetPathTime(0.0);
	m_visMgr->SetRotAxis(distribution(generator), distribution(generator), distribution(generator));
	Invalidate();
}

System::Void OpenGLForm::OpenGLWin::UpdateAnimation()
{
	if (m_visMgr->GetPathTime() < m_visMgr->GetPathLength() + 8)
	{
		m_visMgr->SetPathTime(m_visMgr->GetPathTime() + 0.017);
		Invalidate();
	}
	else
	{
		RefreshWithBuilder();
	}
}

System::Void OpenGLForm::OpenGLWin::Invalidate()
{
	//invalidate does not work correctly; however, with timer set to 1/60 sec, should get similar effect
	//HWND winHandle = (HWND)this->Handle.ToPointer();
	//InvalidateRect(winHandle, NULL, TRUE);

	Render();
	
}

OpenGLForm::OpenGLWin::~OpenGLWin()
{
	this->DestroyHandle();
}

void OpenGLForm::OpenGLWin::WndProc(System::Windows::Forms::Message m)
{
	//Not working for some reason...
	switch (m.Msg)
	{
	case WM_ACTIVATEAPP:
		Render();
	case WM_PAINT:
		Render();
		break;
	}
	System::Windows::Forms::NativeWindow::WndProc(m);
}

GLint OpenGLForm::OpenGLWin::MySetPixelFormat(HDC hdc)
{
	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		16,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	GLint  iPixelFormat;

	// get the device context's best, available pixel format match 
	if ((iPixelFormat = ChoosePixelFormat(hdc, &pfd)) == 0)
	{
		MessageBox::Show("ChoosePixelFormat Failed");
		return 0;
	}

	// make that match the device context's current pixel format 
	if (SetPixelFormat(hdc, iPixelFormat, &pfd) == FALSE)
	{
		MessageBox::Show("SetPixelFormat Failed");
		return 0;
	}

	if ((m_hglrc = wglCreateContext(m_hDC)) == NULL)
	{
		MessageBox::Show("wglCreateContext Failed");
		return 0;
	}

	if ((wglMakeCurrent(m_hDC, m_hglrc)) == NULL)
	{
		MessageBox::Show("wglMakeCurrent Failed");
		return 0;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		MessageBox::Show("GLEW Error: " + gcnew System::String((const char*)glewGetErrorString(err)));
		return 0;
	}


	return 1;
}

bool OpenGLForm::OpenGLWin::InitGL(GLvoid)				// All setup for opengl goes here
{
	glEnable(GL_LINE_SMOOTH);
	glShadeModel(GL_SMOOTH);							// Enable smooth shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black background
	glClearDepth(1.0f);									// Depth buffer setup
	glEnable(GL_DEPTH_TEST);							// Enables depth testing
	glDepthFunc(GL_LEQUAL);								// The type of depth testing to do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really nice perspective calculations
	return TRUE;										// Initialisation went ok
}

// Properties
System::Boolean OpenGLForm::OpenGLWin::drawPath::get()
{
	return m_visMgr->GetShowFullPath();
}

System::Void OpenGLForm::OpenGLWin::drawPath::set(System::Boolean show)
{
	m_visMgr->SetShowFullPath(show);
}

Color OpenGLForm::OpenGLWin::gridColor::get()
{
	return GLM2Color(m_visMgr->GetGridColor());
}

System::Void OpenGLForm::OpenGLWin::gridColor::set(Color c)
{
	m_visMgr->SetGridColor(Color2GLM(c));
}

Color OpenGLForm::OpenGLWin::pathColor::get()
{
	return GLM2Color(m_visMgr->GetPathColor());
}

System::Void OpenGLForm::OpenGLWin::pathColor::set(Color c)
{
	m_visMgr->SetPathColor(Color2GLM(c));
}

Color OpenGLForm::OpenGLWin::wallColor::get()
{
	return GLM2Color(m_visMgr->GetWallColor());
}

System::Void OpenGLForm::OpenGLWin::wallColor::set(Color c)
{
	m_visMgr->SetWallColor(Color2GLM(c));
}

System::Boolean OpenGLForm::OpenGLWin::showDecayColor::get()
{
	return m_visMgr->GetShowPathDecay();
}

System::Void OpenGLForm::OpenGLWin::showDecayColor::set(System::Boolean show)
{
	m_visMgr->SetShowPathDecay(show);
}

Color OpenGLForm::OpenGLWin::decayColor::get()
{
	return GLM2Color(m_visMgr->GetDecayedPathColor());
}

System::Void OpenGLForm::OpenGLWin::decayColor::set(Color c)
{
	m_visMgr->SetDecayedPathColor(Color2GLM(c));
}

int OpenGLForm::OpenGLWin::decayDelay::get()
{
	return m_visMgr->GetDecayDelay();
}

System::Void OpenGLForm::OpenGLWin::decayDelay::set(int delay)
{
	m_visMgr->SetDecayDelay(delay);
}

System::Boolean OpenGLForm::OpenGLWin::doRotate::get()
{
	return m_visMgr->GetApplyRot();
}

System::Void OpenGLForm::OpenGLWin::doRotate::set(System::Boolean doRot)
{
	m_visMgr->SetApplyRot(doRot);
}

