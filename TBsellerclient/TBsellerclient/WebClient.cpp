#include "stdafx.h"
#include "WebClient.h"
#include "include/base/cef_bind.h"
#include "include/cef_web_plugin.h"
#include "include/cef_app.h"

#include "include/wrapper/cef_helpers.h"
#include "client_app_renderer.h"
#include "include/capi/cef_frame_capi.h"
//#include "libcef_dll/ctocpp/frame_ctocpp.h"
//#include "libcef_dll/ctocpp/browser_ctocpp.h"

	const char* cTestMessage = "DOMTest.Message";

	using TBClient::ClientAppRenderer;

	class DomVisitTestor : public CefDOMVisitor
	{
	public:
		explicit DomVisitTestor(CefRefPtr<CefBrowser> browser, DOMTestType test_type)
			: browser_(browser),
			test_type_(test_type) {
		}
	protected:
		CefRefPtr<CefBrowser> browser_;
		DOMTestType test_type_;

	public:
		DomVisitTestor();
		void TestAccess(CefRefPtr<CefDOMDocument> document);
		void TestModify(CefRefPtr<CefDOMDocument> document);

		void Visit(CefRefPtr<CefDOMDocument> document) OVERRIDE;

		IMPLEMENT_REFCOUNTING(DomVisitTestor);
	};



CWebClient::CWebClient()
{
	m_hWnd = NULL;
	m_Isrenderer = true;
}
CWebClient::~CWebClient()
{

}
void CWebClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	
	m_Browser = browser;
}
void CWebClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (m_Browser == NULL)
	{
		return;
	}
	if (m_Browser->IsSame(browser))
		m_Browser = NULL;

	//CefQuitMessageLoop();
	//CefShutdown();
}
void CWebClient::OnLoadStart(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	TransitionType transition_type)
{

	m_url = browser->GetMainFrame()->GetURL();
	
	::PostMessageA(m_hWnd, WM_LOADSTART, NULL, (LPARAM)&m_url);
	return __super::OnLoadStart(browser, frame,transition_type);
}
void CWebClient::OnLoadEnd(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode)
{
	if (m_Isrenderer)
	{
		m_url = browser->GetMainFrame()->GetURL();
		::PostMessageA(m_hWnd, WM_LOADEND, NULL, (LPARAM)&m_url);
	}
	//else
	//{
	//	if (frame->IsMain())
	//	{
	//		CefRefPtr<CefProcessMessage> message(
	//			CefProcessMessage::Create(cTestMessage));
	//		message->GetArgumentList()->SetInt(0, DOM_TEST_MODIFY);
	//		browser->SendProcessMessage(PID_RENDERER, message);
	//		//browser->GetMainFrame()->VisitDOM(new DomVisitTestor(browser, DOM_TEST_MODIFY));
	//		
	//	}
	//}
}

void CWebClient::OnLoadError(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	ErrorCode errorCode,
	const CefString& errorText,
	const CefString& failedUrl)
{

}
void CWebClient::OnTitleChange(CefRefPtr<CefBrowser> browser,
	const CefString& title)
{
	m_TitleStr = title;
	::PostMessageA(m_hWnd, WM_TITLECHANGE, NULL, (LPARAM)&m_TitleStr);
}
bool CWebClient::OnBeforePopup(CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	const CefString& target_url,
	const CefString& target_frame_name,
	WindowOpenDisposition target_disposition,
	bool user_gesture,
	const CefPopupFeatures& popupFeatures,
	CefWindowInfo& windowInfo,
	CefRefPtr<CefClient>& client,
	CefBrowserSettings& settings,
	bool* no_javascript_access)
{
	m_url = target_url;
	::PostMessageA(m_hWnd, WM_LOADPOPUP, NULL, (LPARAM)&m_url);
	return true;
}

void CWebClient::CloseBrowser(bool force_close)
{

}

void CWebClient::OnBeforeCommandLineProcessing(const CefString & process_type, CefRefPtr<CefCommandLine> command_line)
{
	command_line->AppendSwitch("process-per-site");
	command_line->AppendSwitch("ppapi-out-of-process");
	command_line->AppendSwitchWithValue("--ppapi-flash-path", "Plugins\\pepflashplayer.dll");
	//manifest.jsonÖÐµÄversion
	command_line->AppendSwitchWithValue("--ppapi-flash-version", "30.0.0.134");

}


	

	
	void DomVisitTestor::TestAccess(CefRefPtr<CefDOMDocument> document)
	{
		OutputDebugStringW(L"DomVisitTestor::TestAccess\r\n");
		OutputDebugStringW(document->GetTitle().ToWString().c_str());
		OutputDebugStringW(document->GetBaseURL().ToWString().c_str());

		CefRefPtr<CefDOMNode> headNode = document->GetHead();
		OutputDebugStringW(headNode->GetName().ToWString().c_str());
		OutputDebugStringW(headNode->GetAsMarkup().ToWString().c_str());
		wchar_t szLog[512] = { 0 };
		if (headNode->HasChildren())
		{
			CefRefPtr<CefDOMNode> childNode = headNode->GetFirstChild();

			do
			{
				swprintf_s(szLog, 256, L"node name -%s, type-%d, value-%s\r\n",
					childNode->GetName().ToWString().c_str(), childNode->GetType(), childNode->GetValue());
				OutputDebugStringW(szLog);
			} while ((childNode = childNode->GetNextSibling()).get());
		}

		CefRefPtr<CefDOMNode> bodyNode = document->GetBody();
		OutputDebugStringW(bodyNode->GetAsMarkup().ToWString().c_str());
		if (bodyNode->HasChildren())
		{
			CefRefPtr<CefDOMNode> childNode = bodyNode->GetFirstChild();
			do
			{
				swprintf_s(szLog, 256, L"node name -%s, type-%d, value-%s\r\n",
					childNode->GetName().ToWString().c_str(), childNode->GetType(), childNode->GetValue());
				OutputDebugStringW(szLog);
			} while ((childNode = childNode->GetNextSibling()).get());
		}
	}

	void DomVisitTestor::TestModify(CefRefPtr<CefDOMDocument> document)
	{
		OutputDebugStringW(L"DomVisitTestor::TestModify\r\n");
		CefRefPtr<CefDOMNode> bodyNode = document->GetBody();
		if (bodyNode->HasChildren())
		{
			CefRefPtr<CefDOMNode> childNode = bodyNode->GetFirstChild();
			wchar_t szLog[512] = { 0 };
			do {
				swprintf_s(szLog, 256, L"node name -%s,tagName-%s type-%d, value-%s\r\n",
					childNode->GetName().ToWString().c_str(),
					childNode->GetElementTagName().ToWString().c_str(),
					childNode->GetType(), childNode->GetValue());
				OutputDebugStringW(szLog);
				if (childNode->IsElement() && childNode->GetElementTagName() == "H1"
					&& childNode->GetElementAttribute("id") == "hello")
				{
					CefRefPtr<CefDOMNode> textNode = childNode->GetFirstChild();
					swprintf_s(szLog, 512, L"found hello, text - %s\r\n", textNode->GetValue().ToWString().c_str());
					OutputDebugStringW(szLog);
					textNode->SetValue("Hello World Modified!");
					break;
				}
			} while ((childNode = childNode->GetNextSibling()).get());
		}

		CefRefPtr<CefDOMNode> hello = document->GetElementById("hello");
		if (hello.get())
		{
			hello->SetElementAttribute("align", "center");
			OutputDebugStringW(L"Change hello align\r\n");
		}
	}

	void DomVisitTestor::Visit(CefRefPtr<CefDOMDocument> document)
	{
		if (test_type_ == DOM_TEST_STRUCTURE)
			TestAccess(document);
		if (test_type_ == DOM_TEST_STRUCTURE)
			TestModify(document);
	}

	class DOMRendererTest : public TBClient::ClientAppRenderer::Delegate {
	public:
		DOMRendererTest() {
		}

		bool OnProcessMessageReceived(
			CefRefPtr<TBClient::ClientAppRenderer> app,
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) override {
			CEF_REQUIRE_RENDERER_THREAD();
			if (message->GetName() == cTestMessage) {
				//EXPECT_EQ(message->GetArgumentList()->GetSize(), (size_t)1);
				int test_type = message->GetArgumentList()->GetInt(0);

				browser->GetMainFrame()->VisitDOM(
					new DomVisitTestor(browser, static_cast<DOMTestType>(test_type)));
				return true;
			}
			return false;
		}

	private:
		IMPLEMENT_REFCOUNTING(DOMRendererTest);
	};


void CreateDOMRendererTests(TBClient::ClientAppRenderer::DelegateSet& delegates) {
	delegates.insert(new DOMRendererTest);
}

bool CWebClient::DoClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	if (m_Browser)
		m_force_close = true;
	return false;
}
