#pragma once

#include "Object.h"
#include <time.h>

#define MAP_X     32
#define MAP_Z     32
#define HEIGHT    50.0f


#pragma comment(lib,"glaux.lib")
#pragma comment(lib,"D:\\open-source\\soil\\Simple OpenGL Image Library\\lib\\SOIL.lib")

Object                g_obj;
HDC                   g_hdc    =  NULL;
HINSTANCE             g_hInst  =  NULL;
HGLRC                 g_hGlrc  =  NULL;
HWND                  g_hMainWnd=  NULL;
BOOL                  g_bKeys[256];
BOOL                  g_bActive = TRUE;
BOOL                  g_bFullscreen = FALSE;
BOOL                  g_bDone = FALSE;
LPCTSTR               g_strClassName= TEXT("OpenGL Window");
GLfloat               g_imageData[MAP_X][MAP_Z] ;
GLfloat               g_texcoord[MAP_Z*MAP_X][2];
GLdouble              g_eye[3],g_center[3];
GLdouble              g_jumpHeight=0.0,g_jumpSpeed = 0.0;
BOOL                  g_bJump = FALSE;
   
INT       g_wndCenterX,g_wndCenterY;
GLuint    g_envList;
GLuint    texname=0;
GLuint    texcross=0;
GLuint    texLeft,texRight,texTop,texFront,texBack;
GLfloat   transX = 0.0,transY=-1.5,transZ=-3.0;
GLfloat   angleY = 0.0,angleX=0.0;
GLfloat   g_angleX = 0.0,g_angleY = 0.0;
GLfloat   tran = 0.0;
GLboolean g_bRun = false;
GLint     g_frameCount;

GLfloat GetHeight(GLfloat x,GLfloat z);
GLvoid  Wander();
GLvoid  LoadTerrain(char * filename);
GLvoid  LoadTexture(char * filename);
GLvoid  ResizeFunc(GLint width,GLint height);
BOOL    InitOpenGL();
GLvoid  DrawScene();
GLvoid  KillWindow();
BOOL    CreateGLWindow(LPCTSTR title, int width, int height, int bits, BOOL fullscreenflag);
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	MSG  msg;
	BOOL bFullScreen = TRUE;

	if (::MessageBox(NULL,TEXT("运行在全屏下？？"),TEXT("OpenGL"),MB_YESNO) == IDNO)
	{
		bFullScreen = FALSE;
	}

	if(!CreateGLWindow(TEXT("OpenGL Window"),640,480,32,bFullScreen))
	{
		return 0;
	}

	while (! g_bDone)
	{
		//SetForegroundWindow(g_hMainWnd);
		if (::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				g_bDone = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (g_bActive)
			{
				if (g_bKeys[VK_ESCAPE])
				{
					g_bDone = TRUE;
				}
				else
				{
					DrawScene();
					SwapBuffers(g_hdc);
				}
			}
			if (g_bKeys[VK_UP])
			{
				g_obj.Run();
				if (!g_bRun)
				{
					g_bRun = true;
			    	g_obj.SetState(RUN);
				}
				//transZ += 0.1;
			}
			else
			{
				if (g_bRun)
				{
					g_bRun = false;
					g_obj.SetState(CROUCH_STAND);
				}
			}
			if (g_bKeys[VK_DOWN])
			{
				g_obj.Catch(g_center[0]-g_eye[0],g_center[1]-g_eye[1],g_center[2]-g_eye[2]);
				//transZ -= 0.1;
			}
			if (g_bKeys[VK_LEFT])
			{
				g_obj.Rotate(LEFT,1.0);
				//angleY += 0.5;
			}
			if (g_bKeys[VK_RIGHT])
			{
				g_obj.Rotate(RIGHT,1.0);
				//angleY -= 0.5;
			}
		}
	}
	return msg.wParam;
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT  wndRect;
    INT  x,y;

    switch (message)
    {
	case WM_MOUSEMOVE:
		{
			GetWindowRect(hwnd,&wndRect);
			g_wndCenterX = (wndRect.right-wndRect.left)/2;
			g_wndCenterY = (wndRect.bottom - wndRect.top)/2;
			x = LOWORD(lParam);
			y = HIWORD(lParam);
			//angleX += (x-g_wndCenterX)/100.0;
			//angleY += (y-g_wndCenterY)/100.0;
			g_angleX -= (x-g_wndCenterX)/1000.0;
			g_angleY -= (y-g_wndCenterY)/900.0;
			if (g_angleY >= 360.0)
			{
				g_angleY = 360.0;
			}
			if (g_angleY <= -360.0)
			{
				g_angleY = -360.0;
			}
            if (g_angleX >= 360.0)
            {
				g_angleX = 360.0;
            }
			if (g_angleX <= -360.0)
			{
				g_angleX = -360.0;
			}
			//if (angleY >= 50.0 )
			//{
			//	angleY = 50.0;
			//}
			//if (angleY <= -20.0)
			//{
			//	angleY = -20.0;
			//}
			if (abs(x-g_wndCenterX)>150 || abs(y-g_wndCenterY)>150)
			{
				x = g_wndCenterX;
				y = g_wndCenterY;
				//SetCursorPos(x,y);
			}
			return (0);
		}
	case WM_ACTIVATE:
		{
			if (!HIWORD(lParam))
			{
				g_bActive = TRUE;
			}
			else
			{
				g_bActive = FALSE;
			}
			return (0);
		}
	case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
			case SC_MONITORPOWER:
			case SC_SCREENSAVE:
				break;
			}
			break;
		}
	case WM_KEYDOWN:
		{
			g_bKeys[wParam] = TRUE;
			return (0);
		}
	case WM_KEYUP:
		{
			g_bKeys[wParam] = FALSE;
			return (0);
		}
	case WM_SIZE:
		{
			ResizeFunc(LOWORD(lParam),HIWORD(lParam));
			return (0);
		}
    case WM_DESTROY:
		{
			PostQuitMessage (0);
			return (0);
		}
    }
    return DefWindowProc (hwnd, message, wParam, lParam);
}

GLvoid ResizeFunc( GLint width,GLint height )
{
	if (height <= 0)
	{
		height = 1;
	}
	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,((GLfloat)width/(GLfloat)height),0.1f,3000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

BOOL InitOpenGL()
{
	GLfloat pos[]={1.0,1.0,-6.0};
    g_eye[0]=0.0;
	g_eye[1]=0.0;
	g_eye[2]=-100.0;
	g_center[0]= 0.0;
	g_center[1]= 0.0;
	g_center[2]= 0.0;
	glClearColor(0.0,0.0,0.0,1.0);
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	LoadTexture("floor.bmp");	
	g_obj.InitObject("tris.md2","tris.tga");
	g_obj.SetState(CROUCH_STAND);
	//g_loader.LoadModel("tris.md2");
	//g_loader.LoadSkin("tris.tga");
	//g_loader.SetAnim(POINT_A);
 //   g_loader.ScaleModel(1.5);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_REPLACE);

	glLightfv(GL_LIGHT0,GL_POSITION,pos);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_TEXTURE);
	glEnable(GL_TEXTURE_2D);

	LoadTerrain("Terrain.bmp");

	g_envList=glGenLists(1);
	glNewList(g_envList,GL_COMPILE);
	glPushMatrix();
	glEnable(GL_TEXTURE);
	glBindTexture(GL_TEXTURE_2D,texname);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);glVertex3f(-500.0,0.0,500.0);
	glTexCoord2d(4.0,0.0);glVertex3f(500.0,0.0,500.0);
	glTexCoord2d(4.0,4.0);glVertex3f(500.0,0.0,-500.0);
	glTexCoord2d(0.0,4.0);glVertex3f(-500.0,0.0,-500.0);
	glEnd();	
#define SCALE_X            (1100.0f/32)
#define SCALE_Z            (1100.0f/32)
//    GLfloat x= -550.0;
//    GLfloat z= -550.0;
//	for (int i=0;i<MAP_X-1;x+=SCALE_X,++i)
//	{
//		z = -550.0;
//		for (int e=0;e < MAP_Z-1;z+=SCALE_Z,++e)
//		{
//	        glBegin(GL_QUADS);
//			glTexCoord2f(x,z);    glVertex3f(x,g_imageData[i][e],z);
//			glTexCoord2f(x+1,z);  glVertex3f(x+SCALE_X,g_imageData[i+1][e],z);
//			glTexCoord2f(x+1,z+1);glVertex3f(x+SCALE_X,g_imageData[i+1][e+1],z+SCALE_Z);
//			glTexCoord2f(x,z+1);  glVertex3f(x,g_imageData[i][e+1],z+SCALE_Z);
//            glEnd();
//		}
//	}
	

	glBindTexture(GL_TEXTURE_2D,texTop);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);glVertex3f(-500.0,500.0,500.0);
	glTexCoord2d(1.0,0.0);glVertex3f(500.0,500.0,500.0);
	glTexCoord2d(1.0,1.0);glVertex3f(500.0,500.0,-500.0);
	glTexCoord2d(0.0,1.0);glVertex3f(-500.0,500.0,-500.0);
	glEnd();

	glMatrixMode(GL_MODELVIEW);

	glBindTexture(GL_TEXTURE_2D,texFront);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);glVertex3f(-500.0,500.0,-500.0);
	glTexCoord2d(1.0,0.0);glVertex3f(500.0,500.0,-500.0);
	glTexCoord2d(1.0,1.0);glVertex3f(500.0,0.0,-500.0);
	glTexCoord2d(0.0,1.0);glVertex3f(-500.0,0.0,-500.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texRight);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);glVertex3f(-500.0,500.0,500.0);
	glTexCoord2d(1.0,0.0);glVertex3f(-500.0,500.0,-500.0);
	glTexCoord2d(1.0,1.0);glVertex3f(-500.0,0.0,-500.0);
	glTexCoord2d(0.0,1.0);glVertex3f(-500.0,0.0,500.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texLeft);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);glVertex3f(500.0,500.0,-500.0);
	glTexCoord2d(1.0,0.0);glVertex3f(500.0,500.0,500.0);
	glTexCoord2d(1.0,1.0);glVertex3f(500.0,0.0,500.0);
	glTexCoord2d(0.0,1.0);glVertex3f(500.0,0.0,-500.0);
	glEnd();

	glBindTexture(GL_TEXTURE_2D,texBack);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glBegin(GL_POLYGON);
	glTexCoord2d(0.0,0.0);glVertex3f(500.0,500.0,500.0);
	glTexCoord2d(1.0,0.0);glVertex3f(-500.0,500.0,500.0);
	glTexCoord2d(1.0,1.0);glVertex3f(-500.0,0.0,500.0);
	glTexCoord2d(0.0,1.0);glVertex3f(500.0,0.0,500.0);
	glEnd();

	glDisable(GL_TEXTURE);
	glPopMatrix();
	glEndList();
	return TRUE;
}

GLvoid DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLoadIdentity();
	//glTranslatef(g_center[0],g_center[1],g_center[2]);
	//glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	//glColor4f(1.0,1.0,1.0,1.0);
	//glBindTexture(GL_TEXTURE_2D,texcross);
	//glBegin(GL_QUADS);
	//glTexCoord2d(1,1); glVertex2f(1,1);
	//glTexCoord2d(1,0); glVertex2f(1,-1);
	//glTexCoord2d(0,0); glVertex2f(-1,-1);
	//glTexCoord2d(0,1); glVertex2f(-1,1);
	//glEnd();

	//glDisable(GL_BLEND);
	//glEnable(GL_DEPTH_TEST);     

	//if (g_bRun)
	//{
	//	if (g_frameCount==0)
	//	{
	//		g_obj.SetState(RUN);
	//	}
	//	g_frameCount++;
	//	if (g_frameCount>=5)
	//	{
	//		g_bRun = false;
	//		g_frameCount=0;
	//	}
	//}
	//else
	//{
	//	g_obj.SetState(STAND);
	//}

	g_obj.Render();

	Wander();
	glColor3f(1.0,1.0,1.0);
	glCallList(g_envList);


	double eye[3];
	float v[3];
	g_obj.GetPos(v);
    eye[0]=g_eye[0];eye[1]=0;eye[2]=g_eye[2];
	glLineWidth(5.0f);
	glPushMatrix();
	glTranslatef(0.0,0.5,0.0);
	glBegin(GL_LINES);
	glVertex3fv(v);
	glVertex3dv(eye);
	glEnd();
	glPopMatrix();

	glFlush();
}

GLvoid KillWindow()
{
	glDeleteTextures(1,&texname);

	if (g_bFullscreen)
	{
		ChangeDisplaySettings(NULL,0);
		ShowCursor(TRUE);
	}

	if (g_hGlrc)
	{
		if (! ::wglMakeCurrent(NULL,NULL))
		{
			::MessageBox(NULL,TEXT("释放DC或者RC失败"),
				TEXT("关闭错误"),MB_OK|MB_ICONERROR);
		}
		if (! ::wglDeleteContext(g_hGlrc))
		{
			::MessageBox(NULL,TEXT("释放DC或者RC失败"),
				TEXT("关闭错误"),MB_OK|MB_ICONERROR);
		}
		g_hGlrc = NULL;
	}

	if (g_hdc && ! ::ReleaseDC(g_hMainWnd,g_hdc))
	{
		::MessageBox(NULL,TEXT("释放HDC失败"),
			TEXT("关闭错误"),MB_OK|MB_ICONERROR);
	}

	if (g_hMainWnd && ! ::DestroyWindow(g_hMainWnd))
	{
		::MessageBox(NULL,TEXT("关闭窗口失败"),
			TEXT("关闭错误"),MB_OK|MB_ICONERROR);
	}
	if(! ::UnregisterClass(g_strClassName,g_hInst))
	{
		::MessageBox(NULL,TEXT("销毁窗口失败"),
			TEXT("关闭错误"),MB_OK|MB_ICONERROR);
		g_hInst = NULL;
	}
}

BOOL CreateGLWindow(LPCTSTR title, int width, int height,
					  int bits, BOOL fullscreenflag)
{
	GLuint  PixelFormat;
	DWORD   dwExStyle;
	DWORD   dwStyle;
    RECT    wndRect;
	WNDCLASSEX   wndclassex = {0};

	g_hInst = GetModuleHandle(NULL);
	wndclassex.cbSize        = sizeof(WNDCLASSEX);
	wndclassex.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndclassex.lpfnWndProc   = WndProc;
	wndclassex.cbClsExtra    = 0;
	wndclassex.cbWndExtra    = 0;
	wndclassex.hInstance     = g_hInst;
	wndclassex.hIcon         = LoadIcon (NULL, IDI_APPLICATION);
	wndclassex.hCursor       = LoadCursor (NULL, IDC_ARROW);
	wndclassex.hbrBackground = NULL;
	wndclassex.lpszMenuName  = NULL;
	wndclassex.lpszClassName = g_strClassName;
	wndclassex.hIconSm       = wndclassex.hIcon;

	wndRect.left = 0;
	wndRect.top = 0;
	wndRect.right = width;
	wndRect.bottom = height;

	g_bFullscreen = fullscreenflag;


	if (!RegisterClassEx (&wndclassex))
	{
		MessageBox (NULL, TEXT ("RegisterClassEx failed!"), title, MB_ICONERROR);
		return FALSE;
	}
	if (g_bFullscreen)
	{
		DEVMODE  dv;
		ZeroMemory(&dv,sizeof(dv));
		dv.dmSize = sizeof(DEVMODE);
		dv.dmPelsWidth = width;
		dv.dmPelsHeight = height;
		dv.dmBitsPerPel = bits;
		dv.dmDisplayFrequency = 60;
		dv.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;
		if (ChangeDisplaySettings(&dv,CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			if (::MessageBox(NULL,TEXT("全屏设置失败,是否使用窗口模式"),
				TEXT("OpenGL"),MB_YESNO | MB_ICONEXCLAMATION)==IDYES)
			{
				g_bFullscreen = FALSE;
			}
			else
			{
				::MessageBox(NULL,TEXT("程序关闭"),TEXT("Error"),MB_OK);
				return FALSE;
			}
		}
	}

	if (g_bFullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_POPUP;
		ShowCursor(FALSE);
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle   = WS_OVERLAPPEDWINDOW; 
	}

	AdjustWindowRectEx(&wndRect,dwStyle,FALSE,dwExStyle);

	g_hMainWnd = CreateWindowEx (dwExStyle, 
		g_strClassName, 
		title,
		WS_CLIPCHILDREN|
		WS_CLIPSIBLINGS|
		dwStyle,
		0, 0, 
		wndRect.right - wndRect.left, 
		wndRect.bottom- wndRect.top, 
		NULL, 
		NULL, 
		g_hInst,
		NULL); 
	if (!g_hMainWnd)
	{
		MessageBox(NULL,TEXT("创建窗口失败"),TEXT("Error"),MB_OK | MB_ICONERROR);
		KillWindow();
		return FALSE;
	}

	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),					// 上述格式描述符的大小
		1,								                // 版本号
		PFD_DRAW_TO_WINDOW |				     		// 格式支持窗口
		PFD_SUPPORT_OPENGL |			     			// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,				        		// 必须支持双缓冲
		PFD_TYPE_RGBA,						        	// 申请 RGBA 格式
		bits,						            		// 选定色彩深度
		0, 0, 0, 0, 0, 0,						        // 忽略的色彩位
		0,							                 	// 无Alpha缓存
		0,							                	// 忽略Shift Bit
		0,							                	// 无累加缓存
		0, 0, 0, 0,					             		// 忽略聚集位
		16,							                 	// 16位 Z-缓存 (深度缓存)
		0,						                		// 无蒙板缓存
		0,						                		// 无辅助缓存
		PFD_MAIN_PLANE,			          				// 主绘图层
		0,							                	// Reserved
		0, 0, 0					             			// 忽略层遮罩

	};

	if ((g_hdc = GetDC(g_hMainWnd)) == NULL)
	{
		MessageBox(NULL,TEXT("GetDC 错误"),TEXT("Error"),MB_OK | MB_ICONERROR);
		KillWindow();
		return FALSE;
	}
	if (!(PixelFormat =ChoosePixelFormat(g_hdc,&pfd)))
	{
		MessageBox(NULL,TEXT("ChoosePixelFormat 错误"),TEXT("Error"),MB_OK | MB_ICONERROR	);
		KillWindow();
		return FALSE;
	}
	if (! SetPixelFormat(g_hdc,PixelFormat,&pfd))
	{
		MessageBox(NULL,TEXT("SetPixelFormat 错误"),TEXT("Error"),MB_OK | MB_ICONERROR	);
		KillWindow();
		return FALSE;
	}
	if (!(g_hGlrc = wglCreateContext(g_hdc)))
	{
		MessageBox(NULL,TEXT("创建OpenGL上下文错误"),TEXT("Error"),MB_OK | MB_ICONERROR	);
		KillWindow();
		return FALSE;
	}
	if(! wglMakeCurrent(g_hdc,g_hGlrc))
	{
		MessageBox(NULL,TEXT("wglMakeCurrent错误"),TEXT("Error"),MB_OK | MB_ICONERROR	);
		KillWindow();
		return FALSE;
	}

	SetForegroundWindow(g_hMainWnd);
	SetFocus(g_hMainWnd);
	ShowWindow (g_hMainWnd, SW_SHOW);
	ResizeFunc(width,height);

	return InitOpenGL();
	//UpdateWindow (g_hMainWnd);
}

GLvoid LoadTexture( char * filename )
{
	if ((texname=SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,
		texname,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | 
		SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT))==0)
	{
		MessageBox(NULL,TEXT("Load Texture Error"),TEXT("Error"),MB_OK);
	}

	texTop = SOIL_load_OGL_texture("ceiling.bmp"/*"2TOP.bmp"*/,SOIL_LOAD_AUTO,
		texTop,SOIL_FLAG_DDS_LOAD_DIRECT);
	texBack = SOIL_load_OGL_texture("wall.bmp"/*"2RBACK.bmp"*/,SOIL_LOAD_AUTO,
		texBack,SOIL_FLAG_DDS_LOAD_DIRECT);
	texLeft = SOIL_load_OGL_texture("wall.bmp"/*"2LEFT.bmp"*/,SOIL_LOAD_AUTO,
		texLeft,SOIL_FLAG_DDS_LOAD_DIRECT);
	texRight = SOIL_load_OGL_texture("wall.bmp"/*"2RIGHT.bmp"*/,SOIL_LOAD_AUTO,
		texRight,SOIL_FLAG_DDS_LOAD_DIRECT);
	texFront = SOIL_load_OGL_texture("wall.bmp"/*"2FRONT.bmp"*/,SOIL_LOAD_AUTO,
		texFront,SOIL_FLAG_DDS_LOAD_DIRECT);
	texcross = SOIL_load_OGL_texture("crosshair.bmp",SOIL_LOAD_AUTO,
		texcross,SOIL_FLAG_DDS_LOAD_DIRECT);

}

GLvoid LoadTerrain( char * filename )
{ 
	BITMAPFILEHEADER   bfh;
	BITMAPINFOHEADER   bih;
	FILE*              bmpFile;
	RGBTRIPLE          rgb;

	if (fopen_s(&bmpFile,filename,"rb")!=0)
	{
		printf("Open File Error");
		return ;
	}

	fread_s(&bfh,sizeof(BITMAPFILEHEADER),sizeof(BITMAPFILEHEADER),1,bmpFile);
	printf("File Size %f",bfh.bfSize/1024.0);
	fseek(bmpFile,sizeof(bfh),SEEK_SET);
	fread_s(&bih,sizeof(BITMAPINFOHEADER),sizeof(BITMAPINFOHEADER),1,bmpFile);

	//fseek(bmpFile,sizeof(bfh),SEEK_SET);
	fseek(bmpFile,bfh.bfOffBits,SEEK_SET);
	//fseek(bmpFile,bfh.bfOffBits,SEEK_SET);


	for (int x=0;x<bih.biWidth;++x)
	{
		for (int z=0;z<bih.biHeight;++z)
		{
			fread_s(&rgb,sizeof(rgb),sizeof(rgb),1,bmpFile);
			g_imageData[x][z]=rgb.rgbtGreen - 50;
		}
	}



	srand((unsigned)time(NULL));
	for (int i = 0;i<MAP_X;i++)
	{
		for (int e= 0;e<MAP_Z;e++)
		{
			g_imageData[i][e]=-(((GLfloat)rand())/RAND_MAX)+0.2;
		}
	}

	for (int cnt =0;cnt<4;cnt++)
	{
		for (int i = 1;i<MAP_X-1;i++)
		{
			for (int e= 1;e<MAP_Z-1;e++)
			{
				g_imageData[i][e]=(g_imageData[i-1][e]+g_imageData[i+1][e]+
					g_imageData[i][e-1]+g_imageData[i][e+1])/4.0;
				if ((rand()%20==1)&&(g_imageData[i][e]>0)&&cnt==0)
				{
					g_imageData[i][e]*=2;
					g_imageData[i-1][e]=g_imageData[i][e]/2;
					g_imageData[i+1][e]=g_imageData[i][e]/2;
					g_imageData[i][e-1]=g_imageData[i][e]/2;
					g_imageData[i][e+1]=g_imageData[i][e]/2;
				}
			}
		}
	}
}

GLvoid Wander()
{
	static BOOL bADown = false,bBDown=false;
	static int countA = 0,countB = 0;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	float  ang = (3.14159*(180.0-g_angleX) / 180.0f);
	if (g_bKeys['w'] || g_bKeys['W'])
	{
		//UP
		g_eye[0] -= sin(ang);
		g_eye[2] += cos(ang);
	}
	if (g_bKeys['s'] || g_bKeys['S'])
	{
		//down
		g_eye[0] += sin(ang);
		g_eye[2] -= cos(ang);
	}
	if (g_bKeys['a'] || g_bKeys['A'])
	{
		//bADown = true;
		//countA = 40;
		g_angleX += 0.5f;
	}
	//if (bADown)
	//{
	//	if (countA>=0)
	//	{
	//		g_angleX +=0.5;
	//		countA--;
	//	}
	//	else
	//	{
	//		bADown=false;
	//	}
	//}

	if (g_bKeys['d'] || g_bKeys['D'])
	{
		g_angleX -= 0.5f;
		//bBDown = true;
		//countB = 40;
	}
	//if (bBDown)
	//{
	//	if (countB>=0)
	//	{
	//		g_angleX -=0.5;
	//		countB--;
	//	}
	//	else
	//	{
	//		bBDown=false;
	//	}
	//}

	if (g_eye[0]>= 498.0f)
	{
		g_eye[0] = 498.0f;
	}
	if (g_eye[0] <= -498.0f)
	{
		g_eye[0] = -498.0;
	}
	if (g_eye[2]>= 498.0f)
	{
		g_eye[2] = 498.0f;
	}
	if (g_eye[2] <= -498.0f)
	{
		g_eye[2] = -498.0;
	}
	if (g_bJump)
	{
		if (g_jumpHeight >= 0.0)
		{
			g_jumpHeight += g_jumpSpeed + 0.5*0.098;
		}
		else
		{
			g_jumpSpeed = 0.0;
			g_jumpHeight = 0.0;
			g_bJump = FALSE;
		}
		g_jumpSpeed -= 0.098;
	}
	if (g_bKeys[' '])
	{
		g_bJump = TRUE;
		g_jumpSpeed = 2.0;
		g_jumpHeight = 0.0;
	}
	g_eye[1] = GetHeight(g_eye[0],g_eye[2])+HEIGHT+g_jumpHeight;
	g_center[0] = g_eye[0] + 100*sin(-ang);
	g_center[2] = g_eye[2] + 100*cos(ang);
	g_center[1] = g_eye[1] + g_angleY;
	gluLookAt(g_eye[0],g_eye[1],g_eye[2],
		g_center[0],g_center[1],g_center[2],0.0,1.0,0.0);
}

GLfloat GetHeight( GLfloat x,GLfloat z )
{
	if (x <=-100.0 || x>=100.0)
	{
		x = 100.0*(x/abs(x));
	}
	if (z <=-100.0 || z>=100.0)
	{
		z = 100.0*(z/abs(z));
	}
	x+=100.0;
	z+=100.0;

	GLint  X,Z;
	GLfloat sx = SCALE_X,sy = SCALE_Z;
	X = (int)(x / SCALE_X);
	Z = (int)(z / SCALE_Z);

	if (X > MAP_X)
	{
		X = MAP_X -1;
	}
	if (Z > MAP_Z)
	{
		Z = MAP_Z -1;
	}
	return g_imageData[X][Z] + 0.2;
}
