#pragma once
#include "UIListCommonDefine.hpp"

using namespace DuiLib;

struct WordCardListItemInfo
{
	bool folder;
	bool empty;
	CDuiString id;
	CDuiString word;
	CDuiString wordmean;
};


class CWordCardUI : public CListUI
{
public:
	enum { SCROLL_TIMERID = 10 };

	CWordCardUI(CPaintManagerUI& paint_manager);

	~CWordCardUI();

	bool Add(CControlUI* pControl);

	bool AddAt(CControlUI* pControl, int iIndex);

	bool Remove(CControlUI* pControl, bool bDoNotDestroy = false);

	bool RemoveAt(int iIndex, bool bDoNotDestroy);

	void RemoveAll();

	void DoEvent(TEventUI& event);

	Node* GetRoot();

	Node* AddNode(const WordCardListItemInfo& item, Node* parent = NULL);

	bool RemoveNode(Node* node);

	void SetChildVisible(Node* node, bool visible);

	bool CanExpand(Node* node) const;

	bool SelectItem(int iIndex, bool bTakeFocus = false);


private:
	Node*	root_node_;
	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;
	CDuiRect	text_padding_;
	int level_text_start_pos_;
	CDuiString level_expand_image_;
	CDuiString level_collapse_image_;
	CPaintManagerUI& paint_manager_;

	CDialogBuilder m_dlgBuilder;

};


