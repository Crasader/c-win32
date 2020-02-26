#include "include\cef_sandbox_win.h"

#include "MainWnd.h"
#include "SimpleApp.h"

//#pragma comment(lib, "cef_sandbox_d.lib")


int CALLBACK WinMain(
  _In_ HINSTANCE hInstance,
  _In_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow
)
{
	void* sandbox_info = NULL;
	CefMainArgs main_args(hInstance);
	CefRefPtr<CefApp> app(new SimpleApp);
	CefSettings settings;
	settings.no_sandbox = true;
	settings.multi_threaded_message_loop = true;
	CefInitialize(main_args, settings, app.get(), sandbox_info);

	CPaintManagerUI::SetInstance(hInstance);
	CMainWnd wnd;
	wnd.Create(NULL, L"", UI_WNDSTYLE_FRAME | WS_THICKFRAME, WS_EX_ACCEPTFILES);
	wnd.ShowWindow();
	wnd.CenterWindow();
	CPaintManagerUI::MessageLoop();
	CefShutdown();
	return 0;
}