#include "StdAfx.h"
#include ".\gameserviceparamfile.h"
#include "../MarkupSTL/MarkupSTL.h"

CGameServiceParamFile::CGameServiceParamFile(void)
{
}

CGameServiceParamFile::~CGameServiceParamFile(void)
{
}
bool CGameServiceParamFile::Load(const char* szFileName, GameServiceParamArray& array)
{
	CMarkupSTL xml;
	xml.Load(szFileName);
	while ( xml.FindElem("GameServiceParam") )
	{
		GameServiceParam param;
		memset(&param, 0, sizeof(GameServiceParam));

		xml.FindChildElem( "CenterServerAddr");
		_tcscpy(param.m_szCenterServerAddr, xml.GetChildData().c_str());
		xml.FindChildElem( "LoaderServerPass");
		_tcscpy(param.m_szLoaderServerPass, xml.GetChildData().c_str());
		xml.FindChildElem( "ServerID");
		param.m_wServerID = atoi(xml.GetChildData().c_str());
		xml.FindChildElem( "GameNetPort");
		param.m_wGameNetPort = atoi(xml.GetChildData().c_str());
		xml.FindChildElem( "GameNetAddr");
		_tcscpy(param.m_szGameNetAddr, xml.GetChildData().c_str());

		array.Add(param);
	}
	return true;
}
bool CGameServiceParamFile::Save(const char* szFileName, GameServiceParamArray& array)
{
	CMarkupSTL xml;
	for(int i = 0; i < array.GetCount(); i ++)
	{
		GameServiceParam param = array.GetAt(i);
		
		xml.AddElem("GameServiceParam");
		xml.IntoElem();
		xml.AddElem("CenterServerAddr", param.m_szCenterServerAddr);
		xml.AddElem("LoaderServerPass", param.m_szLoaderServerPass);
		xml.AddElem("ServerID", param.m_wServerID);
		xml.AddElem("GameNetPort", param.m_wGameNetPort);
		xml.AddElem("GameNetAddr", param.m_szGameNetAddr);
		xml.OutOfElem();
		
	}
	return xml.Save(szFileName);
}
