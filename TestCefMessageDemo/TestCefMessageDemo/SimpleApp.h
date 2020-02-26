#pragma once
#include "include\cef_app.h"

class SimpleApp :
	public CefApp,
	public CefBrowserProcessHandler,
	public CefRenderProcessHandler,
	public CefV8Handler
{
public:
	SimpleApp(void);
	~SimpleApp(void);

	public:
	// CefApp methods:
  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler()
      OVERRIDE { return this; }

  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler()
	  OVERRIDE { return this; }

  // CefBrowserProcessHandler methods:
  virtual void OnContextInitialized() OVERRIDE{}

  //CefRenderProcessHandler methods
   virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefV8Context> context) OVERRIDE;

   virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE {
   }

   virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message) OVERRIDE;

   //CefV8Handler
   virtual bool Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception) OVERRIDE;

private:
	CefRefPtr<CefV8Value> m_js_callback_func;
	CefRefPtr<CefV8Context> m_js_callback_context;

	HWND m_hWndMain;

private:
	IMPLEMENT_REFCOUNTING(SimpleApp)
};

