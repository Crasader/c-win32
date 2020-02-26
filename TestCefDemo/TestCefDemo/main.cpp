#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include <Windows.h>

class MyClient : public CefClient, public CefLifeSpanHandler
{
	// Constructor & Destructor
public:
	virtual ~MyClient() {}

	// CefClient methods:
public:
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
	{
		return this;
	}

	// CefLifeSpanHandler methods:
public:
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override
	{
		CefQuitMessageLoop();
	}

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(MyClient);
};

// Implement application-level callbacks for the browser process.
class MyApp : public CefApp, public CefBrowserProcessHandler
{
public:
	virtual ~MyApp() {}

	// CefApp methods:
	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }

	// CefBrowserProcessHandler methods:
	virtual void OnContextInitialized() override
	{
		CEF_REQUIRE_UI_THREAD();

		// Information used when creating the native window.
		CefWindowInfo window_info;

		// SimpleHandler implements browser-level callbacks.
		CefRefPtr<MyClient> client(new MyClient());

		// On Windows we need to specify certain flags that will be passed to
		// CreateWindowEx().
		window_info.SetAsPopup(NULL, "cefsimple");

		// Specify CEF browser settings here.
		CefBrowserSettings browser_settings;

		// Create the first browser window.
		CefString url = "http://www.baidu.com";
		CefBrowserHost::CreateBrowser(window_info, client, url, browser_settings, NULL);
	}

private:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(MyApp);
};

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	// Provide CEF with command-line arguments.
	CefMainArgs main_args(hInstance);

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, NULL, NULL);
	if (exit_code >= 0)
	{
		// The sub-process has completed so return here.
		return exit_code;
	}

	// Specify CEF global settings here.
	CefSettings settings;
	settings.no_sandbox = true;

	// SimpleApp implements application-level callbacks for the browser process.
	// It will create the first browser instance in OnContextInitialized() after
	// CEF has initialized.
	auto myApp = CefRefPtr<MyApp>(new MyApp());

	// Initialize CEF.
	CefInitialize(main_args, settings, myApp.get(), NULL);

	// Run the CEF message loop. This will block until CefQuitMessageLoop() is
	// called.
	CefRunMessageLoop();

	// Shut down CEF.
	CefShutdown();

	return 0;
}
