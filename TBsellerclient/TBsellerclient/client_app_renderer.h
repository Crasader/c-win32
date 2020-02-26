#ifndef CEF_TESTS_SHARED_RENDERER_CLIENT_APP_RENDERER_H_
#define CEF_TESTS_SHARED_RENDERER_CLIENT_APP_RENDERER_H_
#pragma once

#include <set>
#include "include/cef_render_process_handler.h"
#include "include/cef_app.h"
#include "WebClient.h"
#include "include/wrapper/cef_helpers.h"



namespace TBClient{

	// Client app implementation for the renderer process.
	class ClientAppRenderer :
		  public CefApp
		, public CefBrowserProcessHandler
		, public CefRenderProcessHandler
		{
		public:
		
	public:
		// Interface for renderer delegates. All Delegates must be returned via
		// CreateDelegates. Do not perform work in the Delegate
		// constructor. See CefRenderProcessHandler for documentation.
		class Delegate : public virtual CefBaseRefCounted {
		public:
			virtual void OnRenderThreadCreated(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefListValue> extra_info) {}

			virtual void OnWebKitInitialized(CefRefPtr<ClientAppRenderer> app) {}

			virtual void OnBrowserCreated(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser) {}

			virtual void OnBrowserDestroyed(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser) {}

			virtual CefRefPtr<CefLoadHandler> GetLoadHandler(
				CefRefPtr<ClientAppRenderer> app) {
				return NULL;
			}

			virtual bool OnBeforeNavigation(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefRequest> request,
				cef_navigation_type_t navigation_type,
				bool is_redirect) {
				return false;
			}

			virtual void OnContextCreated(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefV8Context> context) {}

			virtual void OnContextReleased(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefV8Context> context) {}

			virtual void OnUncaughtException(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefV8Context> context,
				CefRefPtr<CefV8Exception> exception,
				CefRefPtr<CefV8StackTrace> stackTrace) {}

			virtual void OnFocusedNodeChanged(CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser,
				CefRefPtr<CefFrame> frame,
				CefRefPtr<CefDOMNode> node) {}

			// Called when a process message is received. Return true if the message was
			// handled and should not be passed on to other handlers. Delegates
			// should check for unique message names to avoid interfering with each
			// other.
			virtual bool OnProcessMessageReceived(
				CefRefPtr<ClientAppRenderer> app,
				CefRefPtr<CefBrowser> browser,
				CefProcessId source_process,
				CefRefPtr<CefProcessMessage> message) {
				return false;
			}
		};

		typedef std::set<CefRefPtr<Delegate> > DelegateSet;

		ClientAppRenderer();

	private:
		// Creates all of the Delegate objects. Implemented by cefclient in
		// client_app_delegates_renderer.cc
		static void CreateDelegates(DelegateSet& delegates);

		// CefApp methods.
		CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() OVERRIDE {
			return this;
		}

		// CefRenderProcessHandler methods.
		virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;
		virtual void OnWebKitInitialized() OVERRIDE;
		virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
		virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) OVERRIDE;
		CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE;
		virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefRequest> request,
			NavigationType navigation_type,
			bool is_redirect) OVERRIDE;
		virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) OVERRIDE;
		virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context) OVERRIDE;
		virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefV8Context> context,
			CefRefPtr<CefV8Exception> exception,
			CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE;
		virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
			CefRefPtr<CefFrame> frame,
			CefRefPtr<CefDOMNode> node) OVERRIDE;
		virtual bool OnProcessMessageReceived(
			CefRefPtr<CefBrowser> browser,
			CefProcessId source_process,
			CefRefPtr<CefProcessMessage> message) OVERRIDE;



		virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE
		{
			return this;
		}


	private:
		// Set of supported Delegates.
		DelegateSet delegates_;

		IMPLEMENT_REFCOUNTING(ClientAppRenderer);
		DISALLOW_COPY_AND_ASSIGN(ClientAppRenderer);
	};

  // namespace client

}
#endif  // CEF_TESTS_SHARED_RENDERER_CLIENT_APP_RENDERER_H_

