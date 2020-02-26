//// TBsellerclient.cpp : 定义应用程序的入口点。
////
#include "stdafx.h"
//#include "include/cef_app.h"
//#include "include/cef_browser.h"
//#include "include/cef_client.h"
//#include "include/wrapper/cef_closure_task.h"
//#include "include/wrapper/cef_helpers.h"
//#include "include/cef_dom.h"
//
//class DomVisitTestor : public CefDOMVisitor
//{
//public:
//	DomVisitTestor();
//	void TestAccess(CefRefPtr<CefDOMDocument> document);
//	void TestModify(CefRefPtr<CefDOMDocument> document);
//
//	void Visit(CefRefPtr<CefDOMDocument> document) OVERRIDE;
//
//	IMPLEMENT_REFCOUNTING(DomVisitTestor);
//};
//
//void DomVisitTestor::TestAccess(CefRefPtr<CefDOMDocument> document)
//{
//	OutputDebugStringW(L"DomVisitTestor::TestAccess\r\n");
//	OutputDebugStringW(document->GetTitle().ToWString().c_str());
//	OutputDebugStringW(document->GetBaseURL().ToWString().c_str());
//
//	CefRefPtr<CefDOMNode> headNode = document->GetHead();
//	OutputDebugStringW(headNode->GetName().ToWString().c_str());
//	OutputDebugStringW(headNode->GetAsMarkup().ToWString().c_str());
//	wchar_t szLog[512] = { 0 };
//	if (headNode->HasChildren())
//	{
//		CefRefPtr<CefDOMNode> childNode = headNode->GetFirstChild();
//
//		do
//		{
//			swprintf_s(szLog, 256, L"node name -%s, type-%d, value-%s\r\n",
//				childNode->GetName().ToWString().c_str(), childNode->GetType(), childNode->GetValue());
//			OutputDebugStringW(szLog);
//		} while ((childNode = childNode->GetNextSibling()).get());
//	}
//
//	CefRefPtr<CefDOMNode> bodyNode = document->GetBody();
//	OutputDebugStringW(bodyNode->GetAsMarkup().ToWString().c_str());
//	if (bodyNode->HasChildren())
//	{
//		CefRefPtr<CefDOMNode> childNode = bodyNode->GetFirstChild();
//		do
//		{
//			swprintf_s(szLog, 256, L"node name -%s, type-%d, value-%s\r\n",
//				childNode->GetName().ToWString().c_str(), childNode->GetType(), childNode->GetValue());
//			OutputDebugStringW(szLog);
//		} while ((childNode = childNode->GetNextSibling()).get());
//	}
//}
//
//void DomVisitTestor::TestModify(CefRefPtr<CefDOMDocument> document)
//{
//	OutputDebugStringW(L"DomVisitTestor::TestModify\r\n");
//	CefRefPtr<CefDOMNode> bodyNode = document->GetBody();
//	if (bodyNode->HasChildren())
//	{
//		CefRefPtr<CefDOMNode> childNode = bodyNode->GetFirstChild();
//		wchar_t szLog[512] = { 0 };
//		do {
//			swprintf_s(szLog, 256, L"node name -%s,tagName-%s type-%d, value-%s\r\n",
//				childNode->GetName().ToWString().c_str(),
//				childNode->GetElementTagName().ToWString().c_str(),
//				childNode->GetType(), childNode->GetValue());
//			OutputDebugStringW(szLog);
//			if (childNode->IsElement() && childNode->GetElementTagName() == "H1"
//				&& childNode->GetElementAttribute("id") == "hello")
//			{
//				CefRefPtr<CefDOMNode> textNode = childNode->GetFirstChild();
//				swprintf_s(szLog, 512, L"found hello, text - %s\r\n", textNode->GetValue().ToWString().c_str());
//				OutputDebugStringW(szLog);
//				textNode->SetValue("Hello World Modified!");
//				break;
//			}
//		} while ((childNode = childNode->GetNextSibling()).get());
//	}
//
//	CefRefPtr<CefDOMNode> hello = document->GetElementById("hello");
//	if (hello.get())
//	{
//		hello->SetElementAttribute("align", "center");
//		OutputDebugStringW(L"Change hello align\r\n");
//	}
//}
//
//void DomVisitTestor::Visit(CefRefPtr<CefDOMDocument> document)
//{
//	TestAccess(document);
//	TestModify(document);
//}
//
//class MyClient : public CefClient, public CefLifeSpanHandler
//{
//	// Constructor & Destructor
//public:
//	virtual ~MyClient() {}
//
//	// CefClient methods:
//public:
//	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override
//	{
//		return this;
//	}
//
//	// CefLifeSpanHandler methods:
//public:
//	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override
//	{
//		CefQuitMessageLoop();
//	}
//
//private:
//	// Include the default reference counting implementation.
//	IMPLEMENT_REFCOUNTING(MyClient);
//};
////
////// Implement application-level callbacks for the browser process.
//class MyApp : public CefApp, public CefBrowserProcessHandler
//{
//public:
//	virtual ~MyApp() {}
//
//	// CefApp methods:
//	virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
//
//	// CefBrowserProcessHandler methods:
//	virtual void OnContextInitialized() override
//	{
//		CEF_REQUIRE_UI_THREAD();
//
//		// Information used when creating the native window.
//		CefWindowInfo window_info;
//
//		// SimpleHandler implements browser-level callbacks.
//		CefRefPtr<MyClient> client(new MyClient());
//
//		// On Windows we need to specify certain flags that will be passed to
//		// CreateWindowEx().
//		window_info.SetAsPopup(NULL, "cefsimple");
//
//		// Specify CEF browser settings here.
//		CefBrowserSettings browser_settings;
//
//		// Create the first browser window.
//		CefString url = "http://www.baidu.com";
//		CefBrowserHost::CreateBrowser(window_info, client, url, browser_settings, NULL);
//	}
//
//private:
//	// Include the default reference counting implementation.
//	IMPLEMENT_REFCOUNTING(MyApp);
//};
////
//int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
//{
//	// Provide CEF with command-line arguments.
//	CefMainArgs main_args(hInstance);
//
//	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
//	// that share the same executable. This function checks the command-line and,
//	// if this is a sub-process, executes the appropriate logic.
//	int exit_code = CefExecuteProcess(main_args, NULL, NULL);
//	if (exit_code >= 0)
//	{
//		// The sub-process has completed so return here.
//		return exit_code;
//	}
//
//	// Specify CEF global settings here.
//	CefSettings settings;
//	settings.no_sandbox = true;
//
//	// SimpleApp implements application-level callbacks for the browser process.
//	// It will create the first browser instance in OnContextInitialized() after
//	// CEF has initialized.
//	auto myApp = CefRefPtr<MyApp>(new MyApp());
//
//	// Initialize CEF.
//	CefInitialize(main_args, settings, myApp.get(), NULL);
//
//	// Run the CEF message loop. This will block until CefQuitMessageLoop() is
//	// called.
//	CefRunMessageLoop();
//
//	// Shut down CEF.
//	CefShutdown();
//
//	return 0;
//}
