#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_


#include "include/cef_client.h"

#include "include/cef_base.h"
#include "include/cef_browser.h"
#include "include\base\cef_lock.h"
#include "include/cef_web_plugin.h"


#include "cef_cookie.h"
#include <list>



class SimpleHandler : public CefClient,
	public CefDisplayHandler,
	public CefDownloadHandler,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefContextMenuHandler,
	public CefCookieVisitor,
	public CefWebPluginInfoVisitor
{
public:
	SimpleHandler();
	~SimpleHandler();


	// Provide access to the single global instance of this object.
	static SimpleHandler* GetInstance();

	// CefClient methods:
	virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefLoadHandler>		GetLoadHandler() OVERRIDE {
		return this;
	}
	virtual CefRefPtr<CefContextMenuHandler>GetContextMenuHandler() {
		return this;
	}
	virtual CefRefPtr<CefDownloadHandler>	GetDownloadHandler() {
		return this;
	}



	// CefDisplayHandler methods:
	virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
		const CefString& title) OVERRIDE;


	// CefLifeSpanHandler methods:

	/*在创建新的浏览器之后调用;*/
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/*当浏览器收到请求关闭时调用;*/
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/*浏览器销毁之前调用;*/
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/*在创建新的弹出式浏览器IO线程之前调用;*/
	/*本程序主要阻止弹出，在本浏览器中打开;*/
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
		bool* no_javascript_access) OVERRIDE;

	// CefLoadHandler methods:

	/*浏览器加载错误时调用;*/
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;
	/*浏览器完成加载时调用;*/
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) OVERRIDE;


	// CefDownloadHandler methods:

	/*在下载开始之前调用;*/
	virtual void OnBeforeDownload(CefRefPtr<CefBrowser>browser,
		CefRefPtr<CefDownloadItem>download_item,
		const CefString&suggested_name,
		CefRefPtr<CefBeforeDownloadCallback>callback) OVERRIDE;
	/*在下载状态或进度信息更新时调用;*/
	virtual void OnDownloadUpdated(CefRefPtr<CefBrowser>browser,
		CefRefPtr<CefDownloadItem>download_item,
		CefRefPtr<CefDownloadItemCallback>callback) OVERRIDE;

	// CefContextMenuHandler methods:

	/*在显示上下文菜单之前调用;*/
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;
	/*调用来执行从上下文菜单中选择的命;*/
	virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		int command_id,
		EventFlags event_flags) OVERRIDE;

	// CefCookieVisitor methods:
	virtual bool Visit(const CefCookie& cookie, int count, int total,
		bool& deleteCookie)OVERRIDE
	{
		return false;
	};


	// Request that all existing browser windows close.
	// void CloseAllBrowsers(bool force_close);

	bool IsClosing() const { return is_closing_; }

	CefRefPtr<CefBrowser> GetBrowser() { return m_browser; }



public:


	CefRefPtr<CefBrowser> m_browser;

	bool is_closing_;
	// Include the default reference counting implementation.

protected:
	// Include the default reference counting implementation.
	IMPLEMENT_REFCOUNTING(SimpleHandler);
	//由于CEF采用多线程架构，有必要使用锁和闭包来保证在多不同线程安全的传递数据。IMPLEMENT_LOCKING定义提供了Lock()和Unlock()方法以及AutoLock对象来保证不同代码块同步访问
	IMPLEMENT_LOCKING(SimpleHandler);


	//My members. 
public:
	//My variable
	RECT m_ClientRc;//浏览器窗口大小;
	int m_BrowserId;//浏览器ID唯一标识符;
	CString m_name;
	bool m_end;
	CefWindowHandle m_main_wnd;//浏览器的父窗口句柄;

							   //My method
	void InvokeDefaultBrowser(std::wstring url);//使用默认浏览器打开url;
	int JugeIsAdvertisementWindow(CefString strUrl);//判断url类型;
	void SetMainHwnd(CefWindowHandle hwnd);//获取浏览器的父窗口句柄;
	void CloseHostBrowser(CefRefPtr<CefBrowser>browser, bool force_close);
	//void SetDlg(AccountDlg *Dlg);

};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
