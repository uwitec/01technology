#pragma	  once

//////////////////////////////////////////////////////////////////////////
//�����궨��

//��������
#ifndef COM_SERVICE_CLASS
#ifdef  COM_SERVICE_DLL
#define COM_SERVICE_CLASS _declspec(dllexport)
#else
#define COM_SERVICE_CLASS _declspec(dllimport)
#endif
#endif