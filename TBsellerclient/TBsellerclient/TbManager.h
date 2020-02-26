#pragma once

struct WebInfo
{
	wchar_t URL[256];
	wchar_t Confirm[2];
	WebInfo() {
		memset(URL, 0, 256);
		memset(Confirm, 0, 2);
	}
};

class CTbDuilibManager : public WindowImplBase
{
public:
	CTbDuilibManager();
	virtual ~CTbDuilibManager();
private:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	//virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
	virtual void Notify(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);
	virtual void OnClick(TNotifyUI& msg);

private:
	void ShowAllWeb();
	bool LoadFile();
	void SaveFile();
	void AddItem();
	void  OnInitWnd();
	//LRESULT OnAddListItem(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

public:
	std::vector<wstring> m_domain;
	std::vector<wstring> m_desc;
	HANDLE m_hFile;

};

