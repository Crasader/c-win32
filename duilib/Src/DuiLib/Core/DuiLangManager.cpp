#include "stdafx.h"
#include "DuiLangManager.h"
#include <shlwapi.h>
#include <Strsafe.h>

namespace DuiLib
{
	CDuiLangManager::CDuiLangManager(void)
	{

	}

	CDuiLangManager::~CDuiLangManager(void)
	{
		for(int i = 0; i < m_mapLangXml.GetSize(); ++i)
		{
			LPCTSTR psKey = m_mapLangXml.GetAt(i);
			if(psKey)
			{
				CLangXML* pXML = (CLangXML*)m_mapLangXml.Set(psKey, NULL);
				if(pXML)
					delete pXML;
			}
		}
		m_mapLangXml.RemoveAll();
	}

	bool CDuiLangManager::SetDefaultLang(LPCTSTR lpsDefLang)
	{
		if(NULL == lpsDefLang)
			return false;

		CLangXML* pXML = (CLangXML*)m_mapLangXml.Find(m_strCurLangXml);
		if(pXML)
		{
			CMarkupNode curNode = pXML->GetCurrentLangNode(lpsDefLang);
			if(!curNode.IsValid())
				return false;
		}				
		m_strDefLang = lpsDefLang;
		return true;
	}

	bool CDuiLangManager::SetLangPath(LPCTSTR lpsLangPath)
	{
		if(NULL == lpsLangPath || !PathFileExists(lpsLangPath))
			return false;

		m_strLangPath = lpsLangPath;
		return true;
	}

	bool CDuiLangManager::SetLangXml(LPCTSTR lpsLangXml, bool bAppend/* = true*/)
	{
		if(NULL == lpsLangXml)
			return false;

		LPCTSTR psKey = PathFindFileName(lpsLangXml);
		if(!m_mapLangXml.Find(psKey))
		{
			WCHAR wszBuf[MAX_PATH] = {0};
			StringCchCopy(wszBuf, MAX_PATH, m_strLangPath);
			PathAppend(wszBuf, lpsLangXml);
			if(!PathFileExists(wszBuf))
				return false;
			
			LPBYTE pbyBuf = NULL;
			DWORD dwBufSize = 0;
			if(!LoadXML(wszBuf, &pbyBuf, &dwBufSize))
				return false;

			CLangXML* pXML = new CLangXML;
			if(pXML->m_xml.LoadFromMem(pbyBuf, dwBufSize) 
				&& pXML->m_xml.IsValid() 
				&& pXML->m_xml.GetRoot().IsValid() 
				&& StrCmpIW(pXML->m_xml.GetRoot().GetName(), L"language") == 0)
			{
				if(!bAppend)
				{
					for(int i = 0; i < m_mapLangXml.GetSize(); ++i)
					{
						LPCTSTR psKey = m_mapLangXml.GetAt(i);
						if(psKey)
						{
							CLangXML* pXML = (CLangXML*)m_mapLangXml.Set(psKey, NULL);
							if(pXML)
								delete pXML;
						}
					}
					m_mapLangXml.RemoveAll();
				}

				if(m_mapLangXml.Insert(psKey, (LPVOID)pXML))
				{
					pXML->m_strFullXmlFileName = wszBuf;
					pXML->m_strFileName = psKey;
					pXML = NULL;
				}
			}
			if(pXML)
			{
				delete pXML;
				return false;
			}
			if(pbyBuf)
				delete [] pbyBuf;
		}
		m_strCurLangXml = psKey;
		CLangXML* pXML = (CLangXML*)m_mapLangXml.Find(m_strCurLangXml);
		return (NULL == pXML) ? false : true;
	}

	bool CDuiLangManager::SwitchLang(LPCTSTR lpsLang)
	{
		if(NULL == lpsLang)
			return false;

		CLangXML* pXML = (CLangXML*)m_mapLangXml.Find(m_strCurLangXml);
		if(NULL == pXML)
			return false;

		CMarkupNode curNode = pXML->GetCurrentLangNode(lpsLang);
		if(curNode.IsValid())
		{
			m_strDefLang = lpsLang;
			return true;
		}
		return false;
	}

	bool CDuiLangManager::GetText(LPCTSTR lpsTextID, CDuiString& strText)
	{
		if(NULL == lpsTextID)
			return false;

		if(lpsTextID[0] != L'#')
			return false;

		++lpsTextID;//Skip '#'

		CLangXML* pXML = (CLangXML*)m_mapLangXml.Find(m_strCurLangXml);
		if(NULL == pXML)
			return false;

		CMarkupNode nodeLang = pXML->GetCurrentLangNode(m_strDefLang);
		if(!nodeLang.IsValid())
			return false; 

		CMarkupNode node = nodeLang.GetChild(L"item");
		while (node.IsValid())
		{
			LPCTSTR psID = node.GetAttributeValue(L"id");
			if(psID && lstrcmpiW(psID, lpsTextID) == 0)
			{
				strText = node.GetValue();
				return true;
			}
			if(!node.HasSiblings())
				break;
			node = node.GetSibling();
		}
		return false;
	}

	CMarkupNode CDuiLangManager::CLangXML::GetCurrentLangNode(LPCTSTR lpsLang)
	{
		CMarkupNode root = m_xml.GetRoot();
		return root.GetChild(lpsLang);
	}

	bool CDuiLangManager::LoadXML(LPCTSTR lpsXMLFile, LPBYTE* lppBuffer, DWORD* lpdwSize)
	{
		if(NULL == lpsXMLFile || !PathFileExists(lpsXMLFile) || NULL == lppBuffer || NULL == lpdwSize)
			return false;

		HANDLE hFile = ::CreateFile(lpsXMLFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(INVALID_HANDLE_VALUE == hFile)
			return false;

		bool bRtn = false;
		DWORD dwSizeHigh = 0;
		DWORD dwSize = GetFileSize(hFile, &dwSizeHigh);
		if(dwSizeHigh == 0 && dwSize > 0 && dwSize!=INVALID_FILE_SIZE)
		{
			LPBYTE pbyBuf = new BYTE[dwSize+4];
			if(ReadFile(hFile, pbyBuf, dwSize, &dwSizeHigh, NULL) && dwSize == dwSizeHigh)
			{
				*(DWORD*)&pbyBuf[dwSize] = 0;
				
				*lppBuffer = pbyBuf;
				*lpdwSize = dwSize;
				bRtn = true;
			}
		}
		CloseHandle(hFile);
		return bRtn;
	}

	ELangAreaID CDuiLangManager::GetLangAreaID(LPCWSTR lpwsLang)
	{
		for(int i = 0; i < gc_LangAreaDicSize; i++)
		{
			if(0 == StrCmpIW(lpwsLang, gc_LangAreaDic[i].langSysName))
			{
				return gc_LangAreaDic[i].langID;
			}
		}
		return LA_EN_US;
	}

	LPCWSTR CDuiLangManager::GetLangAreaName(ELangAreaID langID)
	{
		WCHAR *cSysName = (WCHAR*)gc_LangAreaDic[0].langSysName;
		LANGID nSysLangID = LANGIDFROMLCID(GetSystemDefaultLCID());
		for ( int i = 0; i < gc_LangAreaDicSize; i++ )
		{
			if (gc_LangAreaDic[i].langID == nSysLangID)
			{
				cSysName = (WCHAR*)gc_LangAreaDic[i].langSysName;
			}
			if ( gc_LangAreaDic[i].langID == langID )
			{
				return gc_LangAreaDic[i].langSysName;
			}
		}
		return cSysName;
	}

	LPCWSTR CDuiLangManager::GetLangNameString(ELangAreaID langID)
	{
		WCHAR *cSysName = (WCHAR*)gc_LangAreaDic[0].langSysName;
		LANGID nSysLangID = LANGIDFROMLCID(GetSystemDefaultLCID());
		for ( int i = 0; i < gc_LangAreaDicSize; i++ )
		{
			if (gc_LangAreaDic[i].langID == nSysLangID)
			{
				cSysName = (WCHAR*)gc_LangAreaDic[i].langNameShow;
			}
			if ( gc_LangAreaDic[i].langID == langID )
			{
				return gc_LangAreaDic[i].langNameShow;
			}
		}
		return cSysName;
	}
}//namespace DuiLib