#pragma once
#include "UIMarkup.h"

namespace DuiLib
{
#include "langdef.h"

	class UILIB_API CDuiLangManager
	{
	public:
		CDuiLangManager(void);
		~CDuiLangManager(void);

		static ELangAreaID GetLangAreaID(LPCWSTR lpwsLang);
		static LPCWSTR GetLangAreaName(ELangAreaID langID);
		static LPCWSTR GetLangNameString(ELangAreaID langID);

	public:
		bool SetDefaultLang(LPCTSTR lpsDefLang);
		bool SetLangPath(LPCTSTR lpsLangPath);
		bool SetLangXml(LPCTSTR lpsLangXml, bool bAppend = true);
		bool SwitchLang(LPCTSTR lpsLang);
		bool GetText(LPCTSTR lpsTextID, CDuiString& strText);

	private:
		bool LoadXML(LPCTSTR lpsXMLFile, LPBYTE* lppBuffer, DWORD* lpdwSize);

	private:
		class CLangXML
		{
		public:
			CDuiString m_strFileName;
			CDuiString m_strFullXmlFileName;
			CMarkup m_xml;

		public:
			inline CMarkupNode GetCurrentLangNode(LPCTSTR lpsLang);
		};

	private:
		CDuiString m_strLangPath;
		CDuiString m_strCurLangXml;
		CDuiString m_strDefLang;
		CStdStringPtrMap m_mapLangXml;//XmlFileName,CLangXML
	};
}
