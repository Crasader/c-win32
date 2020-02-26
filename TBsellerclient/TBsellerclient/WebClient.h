#pragma once
#include "include/cef_client.h"
#include "include/cef_base.h"
#include "include/cef_app.h"
#include "include/cef_browser_process_handler.h"


	enum DOMTestType {
		DOM_TEST_STRUCTURE = 0,
		DOM_TEST_MODIFY
	};

	class CWebClient
		: public CefClient
		, public CefLoadHandler
		, public CefLifeSpanHandler
		, public CefRequestHandler
		, public CefDisplayHandler
		,public CefApp
		
	{
		typedef cef_window_open_disposition_t WindowOpenDisposition;
	protected:
		CefRefPtr<CefBrowser> m_Browser;
		CefRefPtr<CefDOMDocument> m_Document;
	public:
		std::wstring m_url;//加载当前页的url
		HWND m_hWnd; //接收消息的句柄
		CefString m_TitleStr; //网址标题
		bool m_force_close;
		bool m_Isrenderer;
	public:
		CWebClient(void);
		virtual ~CWebClient(void);
		CefRefPtr<CefBrowser> GetBrowser()
		{
			return m_Browser;
		}

		void CWebClient::CloseBrowser(bool force_close);
		virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
		{
			return this;
		}
		virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE
		{
			return this;
		}
		virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE
		{
			return this;
		}
		virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE
		{
			return this;
		}
		virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

		virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			int httpStatusCode) OVERRIDE;
		virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			TransitionType transition_type) OVERRIDE;
		virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			ErrorCode errorCode,
			const CefString& errorText,
			const CefString& failedUrl) OVERRIDE;
		virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
			const CefString& title)OVERRIDE;
		virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			const CefString& target_url,
			const CefString& target_frame_name,
			WindowOpenDisposition target_disposition,
			bool user_gesture,
			const CefPopupFeatures& popupFeatures,
			CefWindowInfo& windowInfo,
			CefRefPtr<CefClient>& client,
			CefBrowserSettings& settings,
			bool* no_javascript_access)OVERRIDE;
		void TurnRenderMode() {
			m_Isrenderer = true;
		}

	
		virtual void OnBeforeCommandLineProcessing(const CefString& process_type,
			CefRefPtr<CefCommandLine> command_line) OVERRIDE;
	
		virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

		virtual void OnWebKitInitialized()
		{
			/*	std::string extensionCode =
					"var test;"
					"if (!test)"
					"  test = {};"
					"(function() {"
					"  test.myval = 'My Value!';"
					"})();";


				CefRegisterExtension("v8/test", extensionCode, NULL);
		*/
		}
		// CefBrowserProcessHandler methods:
		//virtual void OnContextInitialized() override;
		// 添加CEF的SP虚函数
	private:
		IMPLEMENT_REFCOUNTING(CWebClient);

		//IMPLEMENT_LOCKING(CWebClient);
	};

