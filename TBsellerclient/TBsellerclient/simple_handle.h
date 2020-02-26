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

	/*�ڴ����µ������֮�����;*/
	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/*��������յ�����ر�ʱ����;*/
	virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/*���������֮ǰ����;*/
	virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

	/*�ڴ����µĵ���ʽ�����IO�߳�֮ǰ����;*/
	/*��������Ҫ��ֹ�������ڱ�������д�;*/
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

	/*��������ش���ʱ����;*/
	virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		ErrorCode errorCode,
		const CefString& errorText,
		const CefString& failedUrl) OVERRIDE;
	/*�������ɼ���ʱ����;*/
	virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) OVERRIDE;


	// CefDownloadHandler methods:

	/*�����ؿ�ʼ֮ǰ����;*/
	virtual void OnBeforeDownload(CefRefPtr<CefBrowser>browser,
		CefRefPtr<CefDownloadItem>download_item,
		const CefString&suggested_name,
		CefRefPtr<CefBeforeDownloadCallback>callback) OVERRIDE;
	/*������״̬�������Ϣ����ʱ����;*/
	virtual void OnDownloadUpdated(CefRefPtr<CefBrowser>browser,
		CefRefPtr<CefDownloadItem>download_item,
		CefRefPtr<CefDownloadItemCallback>callback) OVERRIDE;

	// CefContextMenuHandler methods:

	/*����ʾ�����Ĳ˵�֮ǰ����;*/
	virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefContextMenuParams> params,
		CefRefPtr<CefMenuModel> model) OVERRIDE;
	/*������ִ�д������Ĳ˵���ѡ�����;*/
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
	//����CEF���ö��̼ܹ߳����б�Ҫʹ�����ͱհ�����֤�ڶ಻ͬ�̰߳�ȫ�Ĵ������ݡ�IMPLEMENT_LOCKING�����ṩ��Lock()��Unlock()�����Լ�AutoLock��������֤��ͬ�����ͬ������
	IMPLEMENT_LOCKING(SimpleHandler);


	//My members. 
public:
	//My variable
	RECT m_ClientRc;//��������ڴ�С;
	int m_BrowserId;//�����IDΨһ��ʶ��;
	CString m_name;
	bool m_end;
	CefWindowHandle m_main_wnd;//������ĸ����ھ��;

							   //My method
	void InvokeDefaultBrowser(std::wstring url);//ʹ��Ĭ���������url;
	int JugeIsAdvertisementWindow(CefString strUrl);//�ж�url����;
	void SetMainHwnd(CefWindowHandle hwnd);//��ȡ������ĸ����ھ��;
	void CloseHostBrowser(CefRefPtr<CefBrowser>browser, bool force_close);
	//void SetDlg(AccountDlg *Dlg);

};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
