#include "SimpleApp.h"

#include "include\cef_browser.h"
#include "include\wrapper\cef_helpers.h"

SimpleApp::SimpleApp(void)
{
}

SimpleApp::~SimpleApp(void)
{
}

void SimpleApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
							  CefRefPtr<CefFrame> frame,
							  CefRefPtr<CefV8Context> context)
{
	CEF_REQUIRE_RENDERER_THREAD();
	m_js_callback_context = NULL;
	m_js_callback_func = NULL;
	m_hWndMain = FindWindow(L"main_wnd_class", NULL);
	CefRefPtr<CefV8Value> object = context->GetGlobal();
	CefRefPtr<CefV8Handler> myhandler = this;
	CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(L"testfunc", myhandler);
	CefRefPtr<CefV8Value> func2 = CefV8Value::CreateFunction(L"testfunc2", myhandler);
	object->SetValue(L"myfunc", func, V8_PROPERTY_ATTRIBUTE_NONE);
	object->SetValue(L"test2", func2, V8_PROPERTY_ATTRIBUTE_NONE);
	object->SetValue(L"registercb", CefV8Value::CreateFunction(L"registercb", myhandler), V8_PROPERTY_ATTRIBUTE_NONE);
}

bool SimpleApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                        CefProcessId source_process,
                                        CefRefPtr<CefProcessMessage> message)
{
	if(message->GetName().compare(L"callback_func") == 0)
	{
		if(m_js_callback_func)
		{
			CefV8ValueList args;
			CefString str = message->GetArgumentList()->GetString(0);
			args.push_back(CefV8Value::CreateString(str));
			args.push_back(CefV8Value::CreateInt(message->GetArgumentList()->GetInt(1)));
			m_js_callback_func->ExecuteFunctionWithContext(m_js_callback_context, NULL, args);
		}
		return true;
	}
	return false;
}

bool SimpleApp::Execute(const CefString& name,
                       CefRefPtr<CefV8Value> object,
                       const CefV8ValueList& arguments,
                       CefRefPtr<CefV8Value>& retval,
                       CefString& exception)
{
	if(name.compare(L"testfunc") ==0)
	{
		//::MessageBox(NULL, L"myfunc", L"js call", MB_OK);
		COPYDATASTRUCT cds;
		const wchar_t* lpData = name.c_str();
		cds.lpData = (LPVOID)lpData;
		cds.cbData = (wcslen(lpData) + 1)*sizeof(WCHAR);
		if(m_hWndMain && IsWindow(m_hWndMain))
			::SendMessage(m_hWndMain, WM_COPYDATA, NULL, (LPARAM)&cds);
		return true;
	}
	else if(name.compare(L"testfunc2") == 0)
	{
		if(arguments.size() == 1 && arguments[0]->IsString())
		{
			/*CefRefPtr<CefV8Value> str = arguments[0];
			::MessageBox(NULL, str->GetStringValue().c_str(), L"test2", MB_OK);*/
			COPYDATASTRUCT cds;
			std::wstring str;
			str = L"function name: ";
			str += name;
			str += L"   arg: ";
			str += arguments[0]->GetStringValue();
			LPCWSTR lpData = str.c_str();
			cds.lpData = (LPVOID)lpData;
			cds.cbData = (wcslen(lpData) + 1)*sizeof(WCHAR);
			if(m_hWndMain && IsWindow(m_hWndMain))
				::SendMessage(m_hWndMain, WM_COPYDATA, NULL, (LPARAM)&cds);

			return true;
		}
	}
	else if(name.compare(L"registercb") == 0)
	{
		if(arguments.size() == 1 && arguments[0]->IsFunction())
		{
			m_js_callback_func = arguments[0].get();
			m_js_callback_context = CefV8Context::GetCurrentContext();
			CefV8ValueList args;
			args.push_back(CefV8Value::CreateString(L"register call back function"));
			args.push_back(CefV8Value::CreateInt(0));
			m_js_callback_func->ExecuteFunctionWithContext(m_js_callback_context, NULL, args);
		}
		return true;
	}
	return false;
}
