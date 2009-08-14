#pragma	  once
//×ª»»ÎÄ×ÖGBK±àÂëÎªUtf8±àÂë
CString inline ConvertGBKToUtf8(CString strGBK) 
{ 
	int len=MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL,0); 
	unsigned short * wszUtf8 = new unsigned short[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	char *szUtf8=new char[len + 1]; 
	memset(szUtf8, 0, len + 1); 
	WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
	CString strOut = szUtf8; 
	delete[] szUtf8; 
	delete[] wszUtf8;
	return strOut;
} 
//×ª»»ÎÄ×ÖUft8±àÂëÎªGBK±àÂë
CString inline ConvertUtf8ToGBK(CString strUtf8) 
{ 
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
	CString strOut = szGBK; 
	delete[] szGBK; 
	delete[] wszGBK; 
	return strOut;
} 
//×ª»»ÎÄ×ÖACP±àÂëÎªUtf8±àÂë
inline void ConvertACPToUtf8(TCHAR *szAcp, int nAcpLen, TCHAR *szUtf8, int nUtf8Len) 
{ 
	int len=MultiByteToWideChar(CP_ACP, 0, szAcp, -1, NULL,0); 
	unsigned short * wszUtf8 = new unsigned short[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)szAcp, -1, (LPWSTR)wszUtf8, len); 
	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, NULL, 0, NULL, NULL); 
	memset(szUtf8, 0, nUtf8Len); 
	if(len <= nUtf8Len)
	{
		WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, len, NULL,NULL); 
	}
	else
	{
		WideCharToMultiByte (CP_UTF8, 0, (LPCWSTR)wszUtf8, -1, szUtf8, nUtf8Len-1, NULL,NULL);
	}
	delete[] wszUtf8; 
}
//×ª»»ÎÄ×ÖUtf8±àÂëÎªACP±àÂë
inline void ConvertUtf8ToACP(TCHAR *szUtf8, int nUtf8Len,TCHAR *szAcp, int nAcpLen) 
{ 
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)szUtf8, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)szUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 

	memset(szAcp, 0, nAcpLen); 
	if(len <= nAcpLen)
	{
		WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szAcp, len, NULL,NULL); 
	}
	else
	{
		WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szAcp, nAcpLen-1, NULL,NULL); 
	}

	delete[] wszGBK; 
} 