#pragma once

#include <windows.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// 
// Adapted from: 
//     https://www.codeproject.com/Articles/16051/Creating-an-OpenGL-view-on-a-Windows-Form
//

using namespace System::Diagnostics;
using namespace System::Windows::Forms;
using namespace System::Drawing;

class VisMgr;
class MazeBuilder;

namespace OpenGLForm 
{
	public ref class OpenGLWin: public System::Windows::Forms::NativeWindow
	{
	public:
		OpenGLWin(System::Windows::Forms::Control ^ parentCtrl);

		GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

		System::Void Render(System::Void);

		System::Void SwapOpenGLBuffers(System::Void);

		System::Void RefreshWithBuilder();

		System::Void UpdateAnimation();

		System::Void Invalidate();

		property Color gridColor
		{
			Color get();
			System::Void set(Color color);
		};

		property Color wallColor
		{
			Color get();
			System::Void set(Color color);
		};

		property Color pathColor
		{
			Color get();
			System::Void set(Color color);
		};

		property  System::Boolean drawPath
		{
			System::Boolean get();
			System::Void set(System::Boolean show);
		};

		property  System::Boolean showDecayColor
		{
			System::Boolean get();
			System::Void set(System::Boolean show);
		};

		property Color decayColor
		{
			Color get();
			System::Void set(Color color);
		};

		property int decayDelay
		{
			int get();
			System::Void set(int delay);
		};

		property  System::Boolean doRotate
		{
			System::Boolean get();
			System::Void set(System::Boolean show);
		};

	private:
		HDC m_hDC;
		HGLRC m_hglrc;
		VisMgr* m_visMgr;
		MazeBuilder* m_bldr;

	protected:
		void WndProc(System::Windows::Forms::Message m);
		~OpenGLWin(System::Void);

		GLint MySetPixelFormat(HDC hdc);
		bool InitGL(GLvoid);

	};
}