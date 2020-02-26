//#pragma once
//#include <list>
//#include <map>
//#include <string>
//#include <utility>
//
//#include "include/base/cef_bind.h"
//#include "include/base/cef_scoped_ptr.h"
//#include "include/cef_browser.h"
//#include "include/cef_client.h"
//#include "include/cef_frame.h"
//#include "include/cef_task.h"
//#include "include/cef_waitable_event.h"
////#include "tests/ceftests/thread_helper.h"
//
//class TestHandler : public CefClient,
//	public CefDialogHandler,
//	public CefDisplayHandler,
//	public CefDownloadHandler,
//	public CefDragHandler,
//	public CefGeolocationHandler,
//	public CefJSDialogHandler,
//	public CefLifeSpanHandler,
//	public CefLoadHandler,
//	public CefRequestHandler
//{
//public:
//	// Tracks the completion state of related test runs.
//	class CompletionState {
//	public:
//		// |total| is the number of times that TestComplete() must be called before
//		// WaitForTests() will return.
//		explicit CompletionState(int total);
//
//		// Call this method to indicate that a test has completed.
//		void TestComplete();
//
//		// This method blocks until TestComplete() has been called the required
//		// number of times.
//		void WaitForTests();
//
//		int total() const { return total_; }
//		int count() const { return count_; }
//
//	private:
//		int total_;
//		int count_;
//
//		// Handle used to notify when the test is complete
//		CefRefPtr<CefWaitableEvent> event_;
//	};
//
//
//	explicit TestHandler(CompletionState* completion_state = NULL);
//	~TestHandler() override;
//
//	// Implement this method to set up the test. Only used in combination with a
//	// Collection. Call SetupComplete() once the setup is complete.
//	virtual void SetupTest() {}
//
//	// Implement this method to run the test. Call DestroyTest() once the test is
//	// complete.
//	virtual void RunTest() = 0;
//
//	// CefClient methods. Add new methods as needed by test cases.
//	CefRefPtr<CefDialogHandler> GetDialogHandler() override {
//		return this;
//	}
//	CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
//		return this;
//	}
//	CefRefPtr<CefDownloadHandler> GetDownloadHandler() override {
//		return this;
//	}
//	CefRefPtr<CefDragHandler> GetDragHandler() override {
//		return this;
//	}
//	CefRefPtr<CefGeolocationHandler> GetGeolocationHandler() override {
//		return this;
//	}
//	CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() override {
//		return this;
//	}
//	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
//		return this;
//	}
//	CefRefPtr<CefLoadHandler> GetLoadHandler() override {
//		return this;
//	}
//	CefRefPtr<CefRequestHandler> GetRequestHandler() override {
//		return this;
//	}
//
//	// CefDownloadHandler methods
//	void OnBeforeDownload(
//		CefRefPtr<CefBrowser> browser,
//		CefRefPtr<CefDownloadItem> download_item,
//		const CefString& suggested_name,
//		CefRefPtr<CefBeforeDownloadCallback> callback) override {}
//
//	// CefDragHandler methods
//	void OnDraggableRegionsChanged(
//		CefRefPtr<CefBrowser> browser,
//		const std::vector<CefDraggableRegion>& regions) override {}
//
//	// CefLifeSpanHandler methods
//	void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
//	void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
//
//	// CefRequestHandler methods
//	CefRefPtr<CefResourceHandler> GetResourceHandler(
//		CefRefPtr<CefBrowser> browser,
//		CefRefPtr<CefFrame> frame,
//		CefRefPtr<CefRequest> request) override;
//
//	// These methods should only be used if at most one non-popup browser exists.
//	CefRefPtr<CefBrowser> GetBrowser();
//	int GetBrowserId();
//
//	// Copies the map of all the currently existing browsers into |map|. Must be
//	// called on the UI thread.
//	void GetAllBrowsers(BrowserMap* map);
//
//	// Called by the test function to execute the test. This method blocks until
//	// the test is complete. Do not reference the object after this method
//	// returns. Do not use this method if the CompletionState object is shared by
//	// multiple handlers or when using a Collection object.
//	void ExecuteTest();
//
//	// Event that will be signaled from the TestHandler destructor.
//	// Used by ReleaseAndWaitForDestructor.
//	void SetDestroyEvent(CefRefPtr<CefWaitableEvent> event) {
//		destroy_event_ = event;
//	}
//
//	// If a test will not call DestroyTest() indicate so using this method.
//	void SetDestroyTestExpected(bool expected) {
//		destroy_test_expected_ = expected;
//	}
//
//	// Returns true if a browser currently exists.
//	static bool HasBrowser() { return browser_count_ > 0; }
//
//protected:
//	// Indicate that test setup is complete. Only used in combination with a
//	// Collection.
//	virtual void SetupComplete();
//
//	// Close any existing non-popup browsers. Test completion will be signaled
//	// once all the browsers have closed if
//	// |signal_completion_when_all_browsers_close_| is true (default value).
//	// If no browsers exist then this method will do nothing and
//	// TestComplete() must be called manually.
//	virtual void DestroyTest();
//
//	// Called on the UI thread if the test times out as a result of calling
//	// SetTestTimeout(). Calls DestroyTest() by default.
//	virtual void OnTestTimeout(int timeout_ms, bool treat_as_error);
//
//	// Called from CreateBrowser() to optionally set per-browser settings.
//	virtual void PopulateBrowserSettings(CefBrowserSettings* settings) {}
//
//	void CreateBrowser(const CefString& url,
//		CefRefPtr<CefRequestContext> request_context = NULL);
//	static void CloseBrowser(CefRefPtr<CefBrowser> browser,
//		bool force_close);
//
//	void AddResource(const std::string& url,
//		const std::string& content,
//		const std::string& mime_type);
//
//	void AddResource(const std::string& url,
//		const std::string& content,
//		const std::string& mime_type,
//		const ResourceContent::HeaderMap& header_map);
//
//	void AddResourceEx(const std::string& url, const ResourceContent& content);
//
//	void ClearResources();
//
//	void SetSignalCompletionWhenAllBrowsersClose(bool val) {
//		signal_completion_when_all_browsers_close_ = val;
//	}
//	bool SignalCompletionWhenAllBrowsersClose() const {
//		return signal_completion_when_all_browsers_close_;
//	}
//
//	// Call OnTestTimeout() after the specified amount of time.
//	void SetTestTimeout(int timeout_ms = 5000, bool treat_as_error = true);
//
//	// Signal that the test is complete. This will be called automatically when
//	// all existing non-popup browsers are closed if
//	// |signal_completion_when_all_browsers_close_| is true (default value). It
//	// is an error to call this method before all browsers have closed.
//	void TestComplete();
//
//	// Returns the single UIThreadHelper instance, creating it if necessary. Must
//	// be called on the UI thread.
//	UIThreadHelper* GetUIThreadHelper();
//
//private:
//	// Used to notify when the test is complete. Can be accessed on any thread.
//	CompletionState* completion_state_;
//	bool completion_state_owned_;
//
//	// Map browser ID to browser object for non-popup browsers. Only accessed on
//	// the UI thread.
//	BrowserMap browser_map_;
//
//	// Values for the first created browser. Modified on the UI thread but can be
//	// accessed on any thread.
//	int first_browser_id_;
//	CefRefPtr<CefBrowser> first_browser_;
//
//	// Map of resources that can be automatically loaded. Only accessed on the
//	// IO thread.
//	typedef std::map<std::string, ResourceContent > ResourceMap;
//	ResourceMap resource_map_;
//
//	// If true test completion will be signaled when all browsers have closed.
//	bool signal_completion_when_all_browsers_close_;
//
//	CefRefPtr<CefWaitableEvent> destroy_event_;
//
//	// Tracks whether DestroyTest() is expected or has been called.
//	bool destroy_test_expected_;
//	bool destroy_test_called_;
//
//	scoped_ptr<UIThreadHelper> ui_thread_helper_;
//
//	// Used to track the number of currently existing browser windows.
//	static int browser_count_;
//
//	DISALLOW_COPY_AND_ASSIGN(TestHandler);
//};
//
//
//template<typename T>
//void ReleaseAndWaitForDestructor(CefRefPtr<T>& handler, int delay_ms = 2000) {
//	CefRefPtr<CefWaitableEvent> event =
//		CefWaitableEvent::CreateWaitableEvent(true, false);
//	handler->SetDestroyEvent(event);
//	T* _handler_ptr = handler.get();
//	handler = NULL;
//	bool handler_destructed = event->TimedWait(delay_ms);
//	EXPECT_TRUE(handler_destructed);
//	if (!handler_destructed) {
//		// |event| is a stack variable so clear the reference before returning.
//		_handler_ptr->SetDestroyEvent(NULL);
//	}
//}