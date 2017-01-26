#pragma warning (disable : 4530) // complaint about exception handler
#pragma warning (disable : 4273)
#pragma warning (disable : 4311) // pointer truncation from HANDLE to long

#include "system.h"

#ifdef USE_GLEE
#include <GL/glee.c>
#endif

//namespace Tmpl8 {
	void Tmpl8::NotifyUser(char* s)
	{
		HWND hApp = FindWindow(NULL, "Template");
		MessageBox(hApp, s, "ERROR", MB_OK);
		exit(0);
	}
//}

static bool running = true;
static HINSTANCE hInstance;
static HWND hwnd;
static Tmpl8::Game* game = 0;

static int SCRPITCH = 0;
int ACTWIDTH, ACTHEIGHT;
static bool FULLSCREEN = false, firstframe = true;

Tmpl8::Surface* surface = 0;
float lastftime = 0;
LARGE_INTEGER lasttime, ticksPS;

// timer stuff
double timer::inv_freq = 1;

// forward declarations
void ReshapeWindow( unsigned int w, unsigned int h );

// Math Stuff
// ----------------------------------------------------------------------------
float vec3EuclidianDistance(const vec3& a, const vec3& b){ return a.vec3EuclidianDistance(b); };
vec3 normalize(const vec3& v) { return v.normalized(); }
vec3 cross(const vec3& a, const vec3& b) { return a.cross(b); }
float dot(const vec3& a, const vec3& b) { return a.dot(b); }
vec3 operator * (const float& s, const vec3& v) { return vec3(v.x * s, v.y * s, v.z * s); }
vec3 operator * (const vec3& v, const float& s) { return vec3(v.x * s, v.y * s, v.z * s); }
vec4 operator * (const float& s, const vec4& v) { return vec4(v.x * s, v.y * s, v.z * s, v.w * s); }
vec4 operator * (const vec4& v, const float& s) { return vec4(v.x * s, v.y * s, v.z * s, v.w * s); }
vec4 operator * (const vec4& v, const mat4& M)
{
	vec4 mx(M.cell[0], M.cell[4], M.cell[8], M.cell[12]);
	vec4 my(M.cell[1], M.cell[5], M.cell[9], M.cell[13]);
	vec4 mz(M.cell[2], M.cell[6], M.cell[10], M.cell[14]);
	vec4 mw(M.cell[3], M.cell[7], M.cell[11], M.cell[15]);
	return v.x * mx + v.y * my + v.z * mz + v.w * mw;
}
mat4 mat4::rotate(const vec3 l, const float a)
{
	// http://inside.mines.edu/fs_home/gmurray/ArbitraryAxisRotation
	mat4 M;
	const float u = l.x, v = l.y, w = l.z, ca = cosf(a), sa = sinf(a);
	M.cell[0] = u * u + (v * v + w * w) * ca, M.cell[1] = u * v * (1 - ca) - w * sa;
	M.cell[2] = u * w * (1 - ca) + v * sa, M.cell[4] = u * v * (1 - ca) + w * sa;
	M.cell[5] = v * v + (u * u + w * w) * ca, M.cell[6] = v * w * (1 - ca) - u * sa;
	M.cell[8] = u * w * (1 - ca) - v * sa, M.cell[9] = v * w * (1 - ca) + u * sa;
	M.cell[10] = w * w + (u * u + v * v) * ca;
	M.cell[3] = M.cell[7] = M.cell[11] = M.cell[12] = M.cell[13] = M.cell[14] = 0, M.cell[15] = 1;
	return M;
}
mat4 mat4::rotatex(const float a)
{
	mat4 M;
	const float ca = cosf(a), sa = sinf(a);
	M.cell[5] = ca, M.cell[6] = -sa;
	M.cell[9] = sa, M.cell[10] = ca;
	return M;
}
mat4 mat4::rotatey(const float a)
{
	mat4 M;
	const float ca = cosf(a), sa = sinf(a);
	M.cell[0] = ca, M.cell[2] = sa;
	M.cell[8] = -sa, M.cell[10] = ca;
	return M;
}
mat4 mat4::rotatez(const float a)
{
	mat4 M;
	const float ca = cosf(a), sa = sinf(a);
	M.cell[0] = ca, M.cell[1] = -sa;
	M.cell[4] = sa, M.cell[5] = ca;
	return M;
}

mat4 mat4::translate(const vec3 pos)
{
	mat4 M;
	vec4 position = vec4(pos, 1.0f);
	vec3 newPos = (position*M).xyz;
	M.cell[3] = newPos.x;
	M.cell[7] = newPos.y;
	M.cell[11] = newPos.z;
	return M;
}



// FatalError
// Generic error handling; called by FATALERROR macro.
// ----------------------------------------------------------------------------
void FatalError( const char* file, int line, const char* message )
{
	char t[8192];
	sprintf( t, "%s, line %i:\n%s", file, line, message );
	MessageBox( NULL, t, "Error", MB_OK | MB_ICONEXCLAMATION );
	exit( 0 );
}
void FatalError( const char* file, int line, const char* message, const char* context )
{
	char t[3000];
	sprintf( t, "%s, line %i:\n%s", file, line, message );
	MessageBox( NULL, t, context, MB_OK | MB_ICONEXCLAMATION );
	exit( 0 );
}

// GetTime
// Reasonably accurate time in a float
// ----------------------------------------------------------------------------
float GetTime()
{
	LARGE_INTEGER freq, value;
	QueryPerformanceCounter( &value );
	QueryPerformanceFrequency( &freq );
	return (float)((double)value.QuadPart / (double)freq.QuadPart);
}

// StartTimer
// Mark the start of a time period to measure
// ----------------------------------------------------------------------------
static LONGLONG startTime = 0;
void StartTimer()
{
	LARGE_INTEGER value;
	QueryPerformanceCounter( &value );
	startTime = value.QuadPart;
}

// GetDuration
// Get the duration since calling StartTimer
// ----------------------------------------------------------------------------
float GetDuration()
{
	LARGE_INTEGER freq, value;
	LONGLONG elapsed;
	QueryPerformanceCounter( &value );
	QueryPerformanceFrequency( &freq );
	elapsed = value.QuadPart - startTime;
	return (float)((double)elapsed / (double)freq.QuadPart);
}

// OCL Window code

// GetWindowHandle
// Provides global access to the application window handle.
// ----------------------------------------------------------------------------
HWND GetWindowHandle()
{
	return hwnd;
}


// WndProc
// Handle windows messages.
// ----------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) 
{
	switch (message) 
	{
	case WM_SIZE:
		ReshapeWindow( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) running = false;
		if (game) game->KeyDown( wParam & 255 );
		break;
	case WM_MOUSEMOVE:
		if (game) game->MouseMove( LOWORD( lParam ), HIWORD( lParam ) );
		break;
	case WM_LBUTTONDOWN:
		//if (game) game->MouseClick();
		break;
	case WM_KEYUP:
		if (game) game->KeyUp( wParam & 255 );
		break;
	}
	return DefWindowProc( hWnd, message, wParam, lParam );
}

// RedirectIOToConsole
// Add a debugging console window and divert stdout, stdin and sterr to it.
// Adapted from http://dslweb.nwnexus.com/~ast/dload/guicon.htm .
// ----------------------------------------------------------------------------
#pragma warning( disable:4311 ) 
#pragma warning( disable:4302 )
void RedirectIOToConsole()
{
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	AllocConsole();
	GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &coninfo );
	coninfo.dwSize.Y = 500;
	SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coninfo.dwSize );
	long h1 = (long)GetStdHandle( STD_OUTPUT_HANDLE );
	int h2 = _open_osfhandle( h1, _O_TEXT );
	FILE* fp = _fdopen( h2, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );
	h1 = (long)GetStdHandle( STD_INPUT_HANDLE ), h2 = _open_osfhandle( h1, _O_TEXT );
	fp = _fdopen( h2, "r" ), *stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );
	h1 = (long)GetStdHandle( STD_ERROR_HANDLE ), h2 = _open_osfhandle( h1, _O_TEXT );
	fp = _fdopen( h2, "w" ), *stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );
	std::ios::sync_with_stdio();
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stderr );
}

// window_from_dc_replacement
// https://www.opengl.org/discussion_boards/showthread.php/173030 
// ----------------------------------------------------------------------------
HWND WINAPI window_from_dc_replacement( HDC dc )
{
	static HWND wnd = NULL;
	if (dc == NULL) return NULL;
	if (wnd == NULL) 
	{
		WNDCLASSA wc;
		memset( &wc, 0, sizeof( wc ) );
		wc.lpfnWndProc = DefWindowProc;
		wc.hInstance = GetModuleHandleA( NULL );
		wc.lpszClassName = "_dummy_window_class_";
		RegisterClassA( &wc );
		wnd = CreateWindowA( wc.lpszClassName, NULL, WS_POPUP, 0, 0, 32, 32, NULL, NULL, wc.hInstance, NULL );
	}
	return wnd;
}

// patch_window_from_dc
// https://www.opengl.org/discussion_boards/showthread.php/173030 
// ----------------------------------------------------------------------------
void patch_window_from_dc()
{
	DWORD old_prot;
	unsigned __int64 wfdc = (unsigned __int64)GetProcAddress( GetModuleHandleA( "user32.dll" ), "WindowFromDC" );
	VirtualProtect( (void*)wfdc, 14, PAGE_EXECUTE_WRITECOPY, &old_prot );
	// jmp [eip + 0]
	*(unsigned char*)(wfdc + 0) = 0xff;
	*(unsigned char*)(wfdc + 1) = 0x25;
	*(unsigned*)(wfdc + 2) = 0x00000000;
	*(unsigned __int64*)(wfdc + 6) = (unsigned __int64)&window_from_dc_replacement;
}

// OpenWindow
// Opens a window.
// ----------------------------------------------------------------------------
bool OpenWindow( const char* title, int width, int height ) 
{
	WNDCLASS windowClass;
	memset( &windowClass, 0, sizeof( windowClass ) );
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	hInstance = GetModuleHandle( NULL );
	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC)WndProc;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon( NULL, IDI_WINLOGO );
	windowClass.hCursor = LoadCursor( NULL, IDC_ARROW );
	windowClass.lpszClassName = title;
	if (!RegisterClass( &windowClass )) return false;
	hwnd = CreateWindowEx( dwExStyle, title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL );
	if (!Tmpl8::CreateGLContext()) FATALERROR("Failed to create OpenGL context");
	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
	return true;
}


// ReshapeWindow
// Any operations required after a window resize go here.
// ----------------------------------------------------------------------------
void ReshapeWindow( unsigned int w, unsigned int h )
{
	// TODO
}

// WinMain
// Application entry point and message pump.
// ----------------------------------------------------------------------------
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ) 
{
	MSG msg;
	RedirectIOToConsole();
	OpenWindow( "OpenCL Laboratory", SCRWIDTH, SCRHEIGHT );
	game = new Tmpl8::Game();
	game->Init();
	while (running)
	{
		if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE )) 
		{
			if (msg.message == WM_QUIT) running = false; else 
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else 
		{
			StartTimer();
			game->Tick(lastftime);
			lastftime = GetDuration();
			Tmpl8::Present();
		}
	}
	game->Shutdown();
	Tmpl8::ShutdownGL();
	return (int)msg.wParam;
}



// AG Tmpl window code
void redirectIO()
{
	static const WORD MAX_CONSOLE_LINES = 500;
	int hConHandle;
	long lStdHandle;
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	AllocConsole();
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
		&coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
		coninfo.dwSize);
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen(hConHandle, "w");
	*stderr = *fp;
	setvbuf(stderr, NULL, _IONBF, 0);
	std::ios::sync_with_stdio();
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
}

int main(int argc, char **argv)
{
	redirectIO();
	printf("application started.\n");
	SDL_Init(SDL_INIT_VIDEO);
	surface = new Tmpl8::Surface(SCRWIDTH, SCRHEIGHT);
	surface->Clear(0);
	surface->InitCharset();
	SDL_Window* window = SDL_CreateWindow("Template", 100, 100, SCRWIDTH, SCRHEIGHT, SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* frameBuffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCRWIDTH, SCRHEIGHT);
	int exitapp = 0;
	game = new Tmpl8::Game();
	game->SetTarget(surface);
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	while (!exitapp)
	{
		void* target = 0;
		int pitch;
		SDL_LockTexture(frameBuffer, NULL, &target, &pitch);
		if (pitch == (surface->GetWidth() * 4))
		{
			memcpy(target, surface->GetBuffer(), SCRWIDTH * SCRHEIGHT * 4);
		}
		else
		{
			unsigned char* t = (unsigned char*)target;
			for (int i = 0; i < SCRHEIGHT; i++)
			{
				memcpy(t, surface->GetBuffer() + i * SCRWIDTH, SCRWIDTH * 4);
				t += pitch;
			}
		}
		SDL_UnlockTexture(frameBuffer);
		SDL_RenderCopy(renderer, frameBuffer, NULL, NULL);
		SDL_RenderPresent(renderer);
		if (firstframe)
		{
			game->Init();
			firstframe = false;
		}
		// calculate frame time and pass it to game->Tick
		StartTimer();
		game->Tick(lastftime);
		lastftime = GetDuration();
		// event loop
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				exitapp = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
				{
					exitapp = 1;
					// find other keys here: http://sdl.beuc.net/sdl.wiki/SDLKey
				}
				game->KeyDown(event.key.keysym.scancode);
				break;
			case SDL_KEYUP:
				game->KeyUp(event.key.keysym.scancode);
				break;
			case SDL_MOUSEMOTION:
				game->MouseMove(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				game->MouseUp(event.button.button);
				break;
			case SDL_MOUSEBUTTONDOWN:
				game->MouseDown(event.button.button);
				break;
			default:
				break;
			}
		}
	}
	game->Shutdown();
	SDL_Quit();
	return 1;
}

// EOF