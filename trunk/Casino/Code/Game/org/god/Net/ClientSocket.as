package org.god.Net
{
	import flash.errors.*;
	import flash.events.*;
	import flash.net.Socket;
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	import flash.utils.Timer;
	import flash.system.*;
	
	import org.aswing.util.LinkedList;

	public class ClientSocket implements IClientSocket,IUnknownEx
	{
		//状态变量
		protected var m_bCloseByServer:Boolean;//关闭方式
		protected var m_SocketState:uint;//连接状态
		protected var m_pIClientSocketSink:IClientSocketSink;//回调接口
		protected var m_arrSocketRecvSink:LinkedList;

		//核心变量
		protected var m_hSocket:Socket;//连接句柄
		protected var m_wRecvSize:int;//接收长度
		protected var m_cbRecvBuf:ByteArray;//接收缓冲
		protected var m_timerDetectSocket:Timer;

		//计数变量
		protected var m_dwSendTickCount:uint;//发送时间
		protected var m_dwRecvTickCount:uint;//接收时间
		protected var m_dwSendPacketCount:uint;//发送计数
		protected var m_dwRecvPacketCount:uint;//接受计数

		//加密数据
		protected var m_cbSendRound:uint;//字节映射
		protected var m_cbRecvRound:uint;//字节映射
		protected var m_dwSendXorKey:uint;//发送密钥
		protected var m_dwRecvXorKey:uint;//接收密钥
		protected static  var m_SendByteMap:ByteArray;//字节映射表
		protected static  var m_RecvByteMap:ByteArray;//字节映射表

		public function ClientSocket()
		{
			if (m_SendByteMap == null)
			{
				m_SendByteMap = newLitteEndianByteArray();

				m_SendByteMap.writeByte(0x70);
				m_SendByteMap.writeByte(0x2F);
				m_SendByteMap.writeByte(0x40);
				m_SendByteMap.writeByte(0x5F);
				m_SendByteMap.writeByte(0x44);
				m_SendByteMap.writeByte(0x8E);
				m_SendByteMap.writeByte(0x6E);
				m_SendByteMap.writeByte(0x45);
				m_SendByteMap.writeByte(0x7E);
				m_SendByteMap.writeByte(0xAB);
				m_SendByteMap.writeByte(0x2C);
				m_SendByteMap.writeByte(0x1F);
				m_SendByteMap.writeByte(0xB4);
				m_SendByteMap.writeByte(0xAC);
				m_SendByteMap.writeByte(0x9D);
				m_SendByteMap.writeByte(0x91);
				m_SendByteMap.writeByte(0x0D);
				m_SendByteMap.writeByte(0x36);
				m_SendByteMap.writeByte(0x9B);
				m_SendByteMap.writeByte(0x0B);
				m_SendByteMap.writeByte(0xD4);
				m_SendByteMap.writeByte(0xC4);
				m_SendByteMap.writeByte(0x39);
				m_SendByteMap.writeByte(0x74);
				m_SendByteMap.writeByte(0xBF);
				m_SendByteMap.writeByte(0x23);
				m_SendByteMap.writeByte(0x16);
				m_SendByteMap.writeByte(0x14);
				m_SendByteMap.writeByte(0x06);
				m_SendByteMap.writeByte(0xEB);
				m_SendByteMap.writeByte(0x04);
				m_SendByteMap.writeByte(0x3E);
				m_SendByteMap.writeByte(0x12);
				m_SendByteMap.writeByte(0x5C);
				m_SendByteMap.writeByte(0x8B);
				m_SendByteMap.writeByte(0xBC);
				m_SendByteMap.writeByte(0x61);
				m_SendByteMap.writeByte(0x63);
				m_SendByteMap.writeByte(0xF6);
				m_SendByteMap.writeByte(0xA5);
				m_SendByteMap.writeByte(0xE1);
				m_SendByteMap.writeByte(0x65);
				m_SendByteMap.writeByte(0xD8);
				m_SendByteMap.writeByte(0xF5);
				m_SendByteMap.writeByte(0x5A);
				m_SendByteMap.writeByte(0x07);
				m_SendByteMap.writeByte(0xF0);
				m_SendByteMap.writeByte(0x13);
				m_SendByteMap.writeByte(0xF2);
				m_SendByteMap.writeByte(0x20);
				m_SendByteMap.writeByte(0x6B);
				m_SendByteMap.writeByte(0x4A);
				m_SendByteMap.writeByte(0x24);
				m_SendByteMap.writeByte(0x59);
				m_SendByteMap.writeByte(0x89);
				m_SendByteMap.writeByte(0x64);
				m_SendByteMap.writeByte(0xD7);
				m_SendByteMap.writeByte(0x42);
				m_SendByteMap.writeByte(0x6A);
				m_SendByteMap.writeByte(0x5E);
				m_SendByteMap.writeByte(0x3D);
				m_SendByteMap.writeByte(0x0A);
				m_SendByteMap.writeByte(0x77);
				m_SendByteMap.writeByte(0xE0);
				m_SendByteMap.writeByte(0x80);
				m_SendByteMap.writeByte(0x27);
				m_SendByteMap.writeByte(0xB8);
				m_SendByteMap.writeByte(0xC5);
				m_SendByteMap.writeByte(0x8C);
				m_SendByteMap.writeByte(0x0E);
				m_SendByteMap.writeByte(0xFA);
				m_SendByteMap.writeByte(0x8A);
				m_SendByteMap.writeByte(0xD5);
				m_SendByteMap.writeByte(0x29);
				m_SendByteMap.writeByte(0x56);
				m_SendByteMap.writeByte(0x57);
				m_SendByteMap.writeByte(0x6C);
				m_SendByteMap.writeByte(0x53);
				m_SendByteMap.writeByte(0x67);
				m_SendByteMap.writeByte(0x41);
				m_SendByteMap.writeByte(0xE8);
				m_SendByteMap.writeByte(0x00);
				m_SendByteMap.writeByte(0x1A);
				m_SendByteMap.writeByte(0xCE);
				m_SendByteMap.writeByte(0x86);
				m_SendByteMap.writeByte(0x83);
				m_SendByteMap.writeByte(0xB0);
				m_SendByteMap.writeByte(0x22);
				m_SendByteMap.writeByte(0x28);
				m_SendByteMap.writeByte(0x4D);
				m_SendByteMap.writeByte(0x3F);
				m_SendByteMap.writeByte(0x26);
				m_SendByteMap.writeByte(0x46);
				m_SendByteMap.writeByte(0x4F);
				m_SendByteMap.writeByte(0x6F);
				m_SendByteMap.writeByte(0x2B);
				m_SendByteMap.writeByte(0x72);
				m_SendByteMap.writeByte(0x3A);
				m_SendByteMap.writeByte(0xF1);
				m_SendByteMap.writeByte(0x8D);
				m_SendByteMap.writeByte(0x97);
				m_SendByteMap.writeByte(0x95);
				m_SendByteMap.writeByte(0x49);
				m_SendByteMap.writeByte(0x84);
				m_SendByteMap.writeByte(0xE5);
				m_SendByteMap.writeByte(0xE3);
				m_SendByteMap.writeByte(0x79);
				m_SendByteMap.writeByte(0x8F);
				m_SendByteMap.writeByte(0x51);
				m_SendByteMap.writeByte(0x10);
				m_SendByteMap.writeByte(0xA8);
				m_SendByteMap.writeByte(0x82);
				m_SendByteMap.writeByte(0xC6);
				m_SendByteMap.writeByte(0xDD);
				m_SendByteMap.writeByte(0xFF);
				m_SendByteMap.writeByte(0xFC);
				m_SendByteMap.writeByte(0xE4);
				m_SendByteMap.writeByte(0xCF);
				m_SendByteMap.writeByte(0xB3);
				m_SendByteMap.writeByte(0x09);
				m_SendByteMap.writeByte(0x5D);
				m_SendByteMap.writeByte(0xEA);
				m_SendByteMap.writeByte(0x9C);
				m_SendByteMap.writeByte(0x34);
				m_SendByteMap.writeByte(0xF9);
				m_SendByteMap.writeByte(0x17);
				m_SendByteMap.writeByte(0x9F);
				m_SendByteMap.writeByte(0xDA);
				m_SendByteMap.writeByte(0x87);
				m_SendByteMap.writeByte(0xF8);
				m_SendByteMap.writeByte(0x15);
				m_SendByteMap.writeByte(0x05);
				m_SendByteMap.writeByte(0x3C);
				m_SendByteMap.writeByte(0xD3);
				m_SendByteMap.writeByte(0xA4);
				m_SendByteMap.writeByte(0x85);
				m_SendByteMap.writeByte(0x2E);
				m_SendByteMap.writeByte(0xFB);
				m_SendByteMap.writeByte(0xEE);
				m_SendByteMap.writeByte(0x47);
				m_SendByteMap.writeByte(0x3B);
				m_SendByteMap.writeByte(0xEF);
				m_SendByteMap.writeByte(0x37);
				m_SendByteMap.writeByte(0x7F);
				m_SendByteMap.writeByte(0x93);
				m_SendByteMap.writeByte(0xAF);
				m_SendByteMap.writeByte(0x69);
				m_SendByteMap.writeByte(0x0C);
				m_SendByteMap.writeByte(0x71);
				m_SendByteMap.writeByte(0x31);
				m_SendByteMap.writeByte(0xDE);
				m_SendByteMap.writeByte(0x21);
				m_SendByteMap.writeByte(0x75);
				m_SendByteMap.writeByte(0xA0);
				m_SendByteMap.writeByte(0xAA);
				m_SendByteMap.writeByte(0xBA);
				m_SendByteMap.writeByte(0x7C);
				m_SendByteMap.writeByte(0x38);
				m_SendByteMap.writeByte(0x02);
				m_SendByteMap.writeByte(0xB7);
				m_SendByteMap.writeByte(0x81);
				m_SendByteMap.writeByte(0x01);
				m_SendByteMap.writeByte(0xFD);
				m_SendByteMap.writeByte(0xE7);
				m_SendByteMap.writeByte(0x1D);
				m_SendByteMap.writeByte(0xCC);
				m_SendByteMap.writeByte(0xCD);
				m_SendByteMap.writeByte(0xBD);
				m_SendByteMap.writeByte(0x1B);
				m_SendByteMap.writeByte(0x7A);
				m_SendByteMap.writeByte(0x2A);
				m_SendByteMap.writeByte(0xAD);
				m_SendByteMap.writeByte(0x66);
				m_SendByteMap.writeByte(0xBE);
				m_SendByteMap.writeByte(0x55);
				m_SendByteMap.writeByte(0x33);
				m_SendByteMap.writeByte(0x03);
				m_SendByteMap.writeByte(0xDB);
				m_SendByteMap.writeByte(0x88);
				m_SendByteMap.writeByte(0xB2);
				m_SendByteMap.writeByte(0x1E);
				m_SendByteMap.writeByte(0x4E);
				m_SendByteMap.writeByte(0xB9);
				m_SendByteMap.writeByte(0xE6);
				m_SendByteMap.writeByte(0xC2);
				m_SendByteMap.writeByte(0xF7);
				m_SendByteMap.writeByte(0xCB);
				m_SendByteMap.writeByte(0x7D);
				m_SendByteMap.writeByte(0xC9);
				m_SendByteMap.writeByte(0x62);
				m_SendByteMap.writeByte(0xC3);
				m_SendByteMap.writeByte(0xA6);
				m_SendByteMap.writeByte(0xDC);
				m_SendByteMap.writeByte(0xA7);
				m_SendByteMap.writeByte(0x50);
				m_SendByteMap.writeByte(0xB5);
				m_SendByteMap.writeByte(0x4B);
				m_SendByteMap.writeByte(0x94);
				m_SendByteMap.writeByte(0xC0);
				m_SendByteMap.writeByte(0x92);
				m_SendByteMap.writeByte(0x4C);
				m_SendByteMap.writeByte(0x11);
				m_SendByteMap.writeByte(0x5B);
				m_SendByteMap.writeByte(0x78);
				m_SendByteMap.writeByte(0xD9);
				m_SendByteMap.writeByte(0xB1);
				m_SendByteMap.writeByte(0xED);
				m_SendByteMap.writeByte(0x19);
				m_SendByteMap.writeByte(0xE9);
				m_SendByteMap.writeByte(0xA1);
				m_SendByteMap.writeByte(0x1C);
				m_SendByteMap.writeByte(0xB6);
				m_SendByteMap.writeByte(0x32);
				m_SendByteMap.writeByte(0x99);
				m_SendByteMap.writeByte(0xA3);
				m_SendByteMap.writeByte(0x76);
				m_SendByteMap.writeByte(0x9E);
				m_SendByteMap.writeByte(0x7B);
				m_SendByteMap.writeByte(0x6D);
				m_SendByteMap.writeByte(0x9A);
				m_SendByteMap.writeByte(0x30);
				m_SendByteMap.writeByte(0xD6);
				m_SendByteMap.writeByte(0xA9);
				m_SendByteMap.writeByte(0x25);
				m_SendByteMap.writeByte(0xC7);
				m_SendByteMap.writeByte(0xAE);
				m_SendByteMap.writeByte(0x96);
				m_SendByteMap.writeByte(0x35);
				m_SendByteMap.writeByte(0xD0);
				m_SendByteMap.writeByte(0xBB);
				m_SendByteMap.writeByte(0xD2);
				m_SendByteMap.writeByte(0xC8);
				m_SendByteMap.writeByte(0xA2);
				m_SendByteMap.writeByte(0x08);
				m_SendByteMap.writeByte(0xF3);
				m_SendByteMap.writeByte(0xD1);
				m_SendByteMap.writeByte(0x73);
				m_SendByteMap.writeByte(0xF4);
				m_SendByteMap.writeByte(0x48);
				m_SendByteMap.writeByte(0x2D);
				m_SendByteMap.writeByte(0x90);
				m_SendByteMap.writeByte(0xCA);
				m_SendByteMap.writeByte(0xE2);
				m_SendByteMap.writeByte(0x58);
				m_SendByteMap.writeByte(0xC1);
				m_SendByteMap.writeByte(0x18);
				m_SendByteMap.writeByte(0x52);
				m_SendByteMap.writeByte(0xFE);
				m_SendByteMap.writeByte(0xDF);
				m_SendByteMap.writeByte(0x68);
				m_SendByteMap.writeByte(0x98);
				m_SendByteMap.writeByte(0x54);
				m_SendByteMap.writeByte(0xEC);
				m_SendByteMap.writeByte(0x60);
				m_SendByteMap.writeByte(0x43);
				m_SendByteMap.writeByte(0x0F);
			}
			if (m_RecvByteMap == null)
			{
				m_RecvByteMap = newLitteEndianByteArray();
				m_RecvByteMap.writeByte(0x51);
				m_RecvByteMap.writeByte(0xA1);
				m_RecvByteMap.writeByte(0x9E);
				m_RecvByteMap.writeByte(0xB0);
				m_RecvByteMap.writeByte(0x1E);
				m_RecvByteMap.writeByte(0x83);
				m_RecvByteMap.writeByte(0x1C);
				m_RecvByteMap.writeByte(0x2D);
				m_RecvByteMap.writeByte(0xE9);
				m_RecvByteMap.writeByte(0x77);
				m_RecvByteMap.writeByte(0x3D);
				m_RecvByteMap.writeByte(0x13);
				m_RecvByteMap.writeByte(0x93);
				m_RecvByteMap.writeByte(0x10);
				m_RecvByteMap.writeByte(0x45);
				m_RecvByteMap.writeByte(0xFF);
				m_RecvByteMap.writeByte(0x6D);
				m_RecvByteMap.writeByte(0xC9);
				m_RecvByteMap.writeByte(0x20);
				m_RecvByteMap.writeByte(0x2F);
				m_RecvByteMap.writeByte(0x1B);
				m_RecvByteMap.writeByte(0x82);
				m_RecvByteMap.writeByte(0x1A);
				m_RecvByteMap.writeByte(0x7D);
				m_RecvByteMap.writeByte(0xF5);
				m_RecvByteMap.writeByte(0xCF);
				m_RecvByteMap.writeByte(0x52);
				m_RecvByteMap.writeByte(0xA8);
				m_RecvByteMap.writeByte(0xD2);
				m_RecvByteMap.writeByte(0xA4);
				m_RecvByteMap.writeByte(0xB4);
				m_RecvByteMap.writeByte(0x0B);
				m_RecvByteMap.writeByte(0x31);
				m_RecvByteMap.writeByte(0x97);
				m_RecvByteMap.writeByte(0x57);
				m_RecvByteMap.writeByte(0x19);
				m_RecvByteMap.writeByte(0x34);
				m_RecvByteMap.writeByte(0xDF);
				m_RecvByteMap.writeByte(0x5B);
				m_RecvByteMap.writeByte(0x41);
				m_RecvByteMap.writeByte(0x58);
				m_RecvByteMap.writeByte(0x49);
				m_RecvByteMap.writeByte(0xAA);
				m_RecvByteMap.writeByte(0x5F);
				m_RecvByteMap.writeByte(0x0A);
				m_RecvByteMap.writeByte(0xEF);
				m_RecvByteMap.writeByte(0x88);
				m_RecvByteMap.writeByte(0x01);
				m_RecvByteMap.writeByte(0xDC);
				m_RecvByteMap.writeByte(0x95);
				m_RecvByteMap.writeByte(0xD4);
				m_RecvByteMap.writeByte(0xAF);
				m_RecvByteMap.writeByte(0x7B);
				m_RecvByteMap.writeByte(0xE3);
				m_RecvByteMap.writeByte(0x11);
				m_RecvByteMap.writeByte(0x8E);
				m_RecvByteMap.writeByte(0x9D);
				m_RecvByteMap.writeByte(0x16);
				m_RecvByteMap.writeByte(0x61);
				m_RecvByteMap.writeByte(0x8C);
				m_RecvByteMap.writeByte(0x84);
				m_RecvByteMap.writeByte(0x3C);
				m_RecvByteMap.writeByte(0x1F);
				m_RecvByteMap.writeByte(0x5A);
				m_RecvByteMap.writeByte(0x02);
				m_RecvByteMap.writeByte(0x4F);
				m_RecvByteMap.writeByte(0x39);
				m_RecvByteMap.writeByte(0xFE);
				m_RecvByteMap.writeByte(0x04);
				m_RecvByteMap.writeByte(0x07);
				m_RecvByteMap.writeByte(0x5C);
				m_RecvByteMap.writeByte(0x8B);
				m_RecvByteMap.writeByte(0xEE);
				m_RecvByteMap.writeByte(0x66);
				m_RecvByteMap.writeByte(0x33);
				m_RecvByteMap.writeByte(0xC4);
				m_RecvByteMap.writeByte(0xC8);
				m_RecvByteMap.writeByte(0x59);
				m_RecvByteMap.writeByte(0xB5);
				m_RecvByteMap.writeByte(0x5D);
				m_RecvByteMap.writeByte(0xC2);
				m_RecvByteMap.writeByte(0x6C);
				m_RecvByteMap.writeByte(0xF6);
				m_RecvByteMap.writeByte(0x4D);
				m_RecvByteMap.writeByte(0xFB);
				m_RecvByteMap.writeByte(0xAE);
				m_RecvByteMap.writeByte(0x4A);
				m_RecvByteMap.writeByte(0x4B);
				m_RecvByteMap.writeByte(0xF3);
				m_RecvByteMap.writeByte(0x35);
				m_RecvByteMap.writeByte(0x2C);
				m_RecvByteMap.writeByte(0xCA);
				m_RecvByteMap.writeByte(0x21);
				m_RecvByteMap.writeByte(0x78);
				m_RecvByteMap.writeByte(0x3B);
				m_RecvByteMap.writeByte(0x03);
				m_RecvByteMap.writeByte(0xFD);
				m_RecvByteMap.writeByte(0x24);
				m_RecvByteMap.writeByte(0xBD);
				m_RecvByteMap.writeByte(0x25);
				m_RecvByteMap.writeByte(0x37);
				m_RecvByteMap.writeByte(0x29);
				m_RecvByteMap.writeByte(0xAC);
				m_RecvByteMap.writeByte(0x4E);
				m_RecvByteMap.writeByte(0xF9);
				m_RecvByteMap.writeByte(0x92);
				m_RecvByteMap.writeByte(0x3A);
				m_RecvByteMap.writeByte(0x32);
				m_RecvByteMap.writeByte(0x4C);
				m_RecvByteMap.writeByte(0xDA);
				m_RecvByteMap.writeByte(0x06);
				m_RecvByteMap.writeByte(0x5E);
				m_RecvByteMap.writeByte(0x00);
				m_RecvByteMap.writeByte(0x94);
				m_RecvByteMap.writeByte(0x60);
				m_RecvByteMap.writeByte(0xEC);
				m_RecvByteMap.writeByte(0x17);
				m_RecvByteMap.writeByte(0x98);
				m_RecvByteMap.writeByte(0xD7);
				m_RecvByteMap.writeByte(0x3E);
				m_RecvByteMap.writeByte(0xCB);
				m_RecvByteMap.writeByte(0x6A);
				m_RecvByteMap.writeByte(0xA9);
				m_RecvByteMap.writeByte(0xD9);
				m_RecvByteMap.writeByte(0x9C);
				m_RecvByteMap.writeByte(0xBB);
				m_RecvByteMap.writeByte(0x08);
				m_RecvByteMap.writeByte(0x8F);
				m_RecvByteMap.writeByte(0x40);
				m_RecvByteMap.writeByte(0xA0);
				m_RecvByteMap.writeByte(0x6F);
				m_RecvByteMap.writeByte(0x55);
				m_RecvByteMap.writeByte(0x67);
				m_RecvByteMap.writeByte(0x87);
				m_RecvByteMap.writeByte(0x54);
				m_RecvByteMap.writeByte(0x80);
				m_RecvByteMap.writeByte(0xB2);
				m_RecvByteMap.writeByte(0x36);
				m_RecvByteMap.writeByte(0x47);
				m_RecvByteMap.writeByte(0x22);
				m_RecvByteMap.writeByte(0x44);
				m_RecvByteMap.writeByte(0x63);
				m_RecvByteMap.writeByte(0x05);
				m_RecvByteMap.writeByte(0x6B);
				m_RecvByteMap.writeByte(0xF0);
				m_RecvByteMap.writeByte(0x0F);
				m_RecvByteMap.writeByte(0xC7);
				m_RecvByteMap.writeByte(0x90);
				m_RecvByteMap.writeByte(0xC5);
				m_RecvByteMap.writeByte(0x65);
				m_RecvByteMap.writeByte(0xE2);
				m_RecvByteMap.writeByte(0x64);
				m_RecvByteMap.writeByte(0xFA);
				m_RecvByteMap.writeByte(0xD5);
				m_RecvByteMap.writeByte(0xDB);
				m_RecvByteMap.writeByte(0x12);
				m_RecvByteMap.writeByte(0x7A);
				m_RecvByteMap.writeByte(0x0E);
				m_RecvByteMap.writeByte(0xD8);
				m_RecvByteMap.writeByte(0x7E);
				m_RecvByteMap.writeByte(0x99);
				m_RecvByteMap.writeByte(0xD1);
				m_RecvByteMap.writeByte(0xE8);
				m_RecvByteMap.writeByte(0xD6);
				m_RecvByteMap.writeByte(0x86);
				m_RecvByteMap.writeByte(0x27);
				m_RecvByteMap.writeByte(0xBF);
				m_RecvByteMap.writeByte(0xC1);
				m_RecvByteMap.writeByte(0x6E);
				m_RecvByteMap.writeByte(0xDE);
				m_RecvByteMap.writeByte(0x9A);
				m_RecvByteMap.writeByte(0x09);
				m_RecvByteMap.writeByte(0x0D);
				m_RecvByteMap.writeByte(0xAB);
				m_RecvByteMap.writeByte(0xE1);
				m_RecvByteMap.writeByte(0x91);
				m_RecvByteMap.writeByte(0x56);
				m_RecvByteMap.writeByte(0xCD);
				m_RecvByteMap.writeByte(0xB3);
				m_RecvByteMap.writeByte(0x76);
				m_RecvByteMap.writeByte(0x0C);
				m_RecvByteMap.writeByte(0xC3);
				m_RecvByteMap.writeByte(0xD3);
				m_RecvByteMap.writeByte(0x9F);
				m_RecvByteMap.writeByte(0x42);
				m_RecvByteMap.writeByte(0xB6);
				m_RecvByteMap.writeByte(0x9B);
				m_RecvByteMap.writeByte(0xE5);
				m_RecvByteMap.writeByte(0x23);
				m_RecvByteMap.writeByte(0xA7);
				m_RecvByteMap.writeByte(0xAD);
				m_RecvByteMap.writeByte(0x18);
				m_RecvByteMap.writeByte(0xC6);
				m_RecvByteMap.writeByte(0xF4);
				m_RecvByteMap.writeByte(0xB8);
				m_RecvByteMap.writeByte(0xBE);
				m_RecvByteMap.writeByte(0x15);
				m_RecvByteMap.writeByte(0x43);
				m_RecvByteMap.writeByte(0x70);
				m_RecvByteMap.writeByte(0xE0);
				m_RecvByteMap.writeByte(0xE7);
				m_RecvByteMap.writeByte(0xBC);
				m_RecvByteMap.writeByte(0xF1);
				m_RecvByteMap.writeByte(0xBA);
				m_RecvByteMap.writeByte(0xA5);
				m_RecvByteMap.writeByte(0xA6);
				m_RecvByteMap.writeByte(0x53);
				m_RecvByteMap.writeByte(0x75);
				m_RecvByteMap.writeByte(0xE4);
				m_RecvByteMap.writeByte(0xEB);
				m_RecvByteMap.writeByte(0xE6);
				m_RecvByteMap.writeByte(0x85);
				m_RecvByteMap.writeByte(0x14);
				m_RecvByteMap.writeByte(0x48);
				m_RecvByteMap.writeByte(0xDD);
				m_RecvByteMap.writeByte(0x38);
				m_RecvByteMap.writeByte(0x2A);
				m_RecvByteMap.writeByte(0xCC);
				m_RecvByteMap.writeByte(0x7F);
				m_RecvByteMap.writeByte(0xB1);
				m_RecvByteMap.writeByte(0xC0);
				m_RecvByteMap.writeByte(0x71);
				m_RecvByteMap.writeByte(0x96);
				m_RecvByteMap.writeByte(0xF8);
				m_RecvByteMap.writeByte(0x3F);
				m_RecvByteMap.writeByte(0x28);
				m_RecvByteMap.writeByte(0xF2);
				m_RecvByteMap.writeByte(0x69);
				m_RecvByteMap.writeByte(0x74);
				m_RecvByteMap.writeByte(0x68);
				m_RecvByteMap.writeByte(0xB7);
				m_RecvByteMap.writeByte(0xA3);
				m_RecvByteMap.writeByte(0x50);
				m_RecvByteMap.writeByte(0xD0);
				m_RecvByteMap.writeByte(0x79);
				m_RecvByteMap.writeByte(0x1D);
				m_RecvByteMap.writeByte(0xFC);
				m_RecvByteMap.writeByte(0xCE);
				m_RecvByteMap.writeByte(0x8A);
				m_RecvByteMap.writeByte(0x8D);
				m_RecvByteMap.writeByte(0x2E);
				m_RecvByteMap.writeByte(0x62);
				m_RecvByteMap.writeByte(0x30);
				m_RecvByteMap.writeByte(0xEA);
				m_RecvByteMap.writeByte(0xED);
				m_RecvByteMap.writeByte(0x2B);
				m_RecvByteMap.writeByte(0x26);
				m_RecvByteMap.writeByte(0xB9);
				m_RecvByteMap.writeByte(0x81);
				m_RecvByteMap.writeByte(0x7C);
				m_RecvByteMap.writeByte(0x46);
				m_RecvByteMap.writeByte(0x89);
				m_RecvByteMap.writeByte(0x73);
				m_RecvByteMap.writeByte(0xA2);
				m_RecvByteMap.writeByte(0xF7);
				m_RecvByteMap.writeByte(0x72);
			}
			m_arrSocketRecvSink = new LinkedList;
			m_cbRecvBuf=newLitteEndianByteArray();
			m_SocketState=SocketState_NoConnect;
			m_timerDetectSocket = new Timer(5000);
			m_timerDetectSocket.addEventListener("timer", timerHandler);
		}

		//是否有效
		public function IsValid():Boolean
		{
			return true;
		}
		//释放对象
		public function Release():Boolean
		{
			return true;
		}
		//接口查询
		public function QueryInterface(Guid:String,dwQueryVer:uint):*
		{
			if (Guid == IID_IUnknownEx)
			{
				return this;
			}
			else if (Guid == IID_IClientSocket)
			{
				return this;
			}
			return null;
		}
		//设置接口
		public function SetSocketSink(pIUnknownEx:IClientSocketSink):Boolean
		{
			m_pIClientSocketSink = pIUnknownEx;
			return m_pIClientSocketSink != null;
		}
		//获取接口
		public function GetSocketSink(Guid:String,dwQueryVer:uint):IClientSocketSink
		{

			return m_pIClientSocketSink;
		}
		//添加接口
		public function AddSocketRecvSink(pIUnknownEx:IClientSocketRecvSink):Boolean
		{
			m_arrSocketRecvSink.append(pIUnknownEx);
			return true;
		}
		//添加接口
		public function RemoveSocketRecvSink(pIUnknownEx:IClientSocketRecvSink):Boolean
		{
			m_arrSocketRecvSink.remove(pIUnknownEx);
			return true;
		}
		//获取发送间隔
		public function GetLastSendTick():uint
		{
			return m_dwSendTickCount;
		}
		//获取接收间隔
		public function GetLastRecvTick():uint
		{
			return m_dwRecvTickCount;
		}
		//获取发送数目
		public function GetSendPacketCount():uint
		{
			return m_dwSendPacketCount;
		}
		//获取接收数目
		public function GetRecvPacketCount():uint
		{
			return m_dwRecvPacketCount;
		}
		//获取状态
		public function GetConnectState():int
		{
			return m_SocketState;
		}
		//连接服务器
		public function Connect(szServerIP:String,wPort:int):Boolean
		{
			if (m_hSocket != null)
			{
				trace("Socket Connected");
				throw new Error("Socket Connected");
			}
			m_hSocket=new Socket();
			
			configureListeners();
			if (m_hSocket.connected)
			{
				trace("Socket Connected");
				throw new Error("Socket Connected");
			}
			var myDate1:Date=new Date;

			m_wRecvSize=0;
			m_cbSendRound=0;
			m_cbRecvRound=0;
			m_dwSendXorKey=0x12345678;
			m_dwRecvXorKey=0x12345678;
			m_dwSendTickCount=myDate1.getMilliseconds() / 1000;
			m_dwRecvTickCount=myDate1.getMilliseconds() / 1000;

			m_hSocket.connect(szServerIP,wPort);
			m_SocketState=SocketState_Connecting;
			//m_timerDetectSocket.start();
			
			return true;
		}
		//发送函数
		public function SendCmd(wMainCmdID:int,wSubCmdID:int):Boolean
		{
			//效验状态
			if (m_hSocket == null)
			{
				trace("SendCmd m_hSocket == null");
				return false;
			}
			if (m_SocketState != SocketState_Connected)
			{
				trace("SendCmd m_SocketState != SocketState_Connected");
				return false;
			}

			//构造数据
			var head:CMD_Head = new CMD_Head;
			head.CommandInfo.wMainCmdID=wMainCmdID;
			head.CommandInfo.wSubCmdID=wSubCmdID;

			var cbDataBuffer:ByteArray=head.toByteArray();


			//加密数据
			var wSendSize:int=EncryptBuffer(cbDataBuffer,CMD_Head.sizeof_CMD_Head,cbDataBuffer.bytesAvailable);

			cbDataBuffer.length = CMD_Head.sizeof_CMD_Head;

			//发送数据
			return SendBuffer(cbDataBuffer,wSendSize);
		}
		//发送函数
		public function SendData(wMainCmdID:int,wSubCmdID:int,pData:ByteArray,wDataSize:int):Boolean
		{
			//效验状态
			if (m_hSocket == null)
			{
				return false;
			}
			if (m_SocketState != SocketState_Connected)
			{
				return false;
			}
			//效验大小

			if (wDataSize > SOCKET_PACKAGE)
			{
				return false;
			}

			//构造数据
			var buffer:CMD_Buffer = new CMD_Buffer;
			buffer.Head.CommandInfo.wMainCmdID=wMainCmdID;
			buffer.Head.CommandInfo.wSubCmdID=wSubCmdID;

			if (wDataSize > 0)
			{
				CopyMemory(buffer.cbBuffer,pData,wDataSize);
			}
			var cbDataBuffer:ByteArray=buffer.toByteArray();

			//加密数据
			var wSendSize:int=EncryptBuffer(cbDataBuffer,CMD_Head.sizeof_CMD_Head + wDataSize,cbDataBuffer.bytesAvailable);

			cbDataBuffer.length = CMD_Head.sizeof_CMD_Head + wDataSize;
			//发送数据
			return SendBuffer(cbDataBuffer,wSendSize);
		}
		//关闭连接
		public function CloseSocket(bNotify:Boolean):Boolean
		{
			var bClose:Boolean;
			if (m_SocketState == SocketState_NoConnect)
			{
				bClose= false;
			}
			else
			{
				bClose  = true;

			}
			m_SocketState=SocketState_NoConnect;
			if (m_hSocket != null)
			{
				m_timerDetectSocket.stop();
				try
				{
					m_hSocket.close();
				}
				catch(e:Error)
				{
					trace(e);
				}

				if (bNotify == true && m_pIClientSocketSink != null)
				{

					try
					{
						m_pIClientSocketSink.OnSocketClose(this,m_bCloseByServer);
					}
					catch (e:Error)
					{
						trace(e);
					}
				}
			}
			//恢复数据
			m_wRecvSize=0;
			m_cbSendRound=0;
			m_cbRecvRound=0;
			m_dwSendXorKey=0;
			m_dwRecvXorKey=0;
			m_dwSendTickCount=0;
			m_dwRecvTickCount=0;
			m_dwSendPacketCount=0;
			m_dwRecvPacketCount=0;
			m_bCloseByServer=false;
			deconfigureListeners();
			m_hSocket = null;
			return true;
		}

		private function configureListeners():void
		{
			m_hSocket.addEventListener(Event.CLOSE,closeHandler);
			m_hSocket.addEventListener(Event.CONNECT,connectHandler);
			m_hSocket.addEventListener(IOErrorEvent.IO_ERROR,ioErrorHandler);
			m_hSocket.addEventListener(SecurityErrorEvent.SECURITY_ERROR,securityErrorHandler);
			m_hSocket.addEventListener(ProgressEvent.SOCKET_DATA,socketDataHandler);
		}
		private function deconfigureListeners():void
		{
			if (m_hSocket != null)
			{
				m_hSocket.removeEventListener(Event.CLOSE,closeHandler);
				m_hSocket.removeEventListener(Event.CONNECT,connectHandler);
				m_hSocket.removeEventListener(IOErrorEvent.IO_ERROR,ioErrorHandler);
				m_hSocket.removeEventListener(SecurityErrorEvent.SECURITY_ERROR,securityErrorHandler);
				m_hSocket.removeEventListener(ProgressEvent.SOCKET_DATA,socketDataHandler);
			}
		}
		private function closeHandler(event:Event):void
		{
			trace("closeHandler: " + event);
			m_bCloseByServer=true;
			CloseSocket(true);
		}

		private function connectHandler(event:Event):void
		{
			trace("connectHandler: " + event);

			if (m_hSocket.connected)
			{
				m_SocketState=SocketState_Connected;
			}
			else
			{
				CloseSocket(false);

			}
			var szErrorDesc:String=event.toString();
			if (m_pIClientSocketSink!=null)
			{
				m_pIClientSocketSink.OnSocketConnect(0,szErrorDesc,this);
			}
			//var dataSend:ByteArray = new ByteArray;
			//dataSend.writeUTFBytes("you are good");
			//SendData(11, 22, dataSend, dataSend.length);
		}

		private function ioErrorHandler(event:IOErrorEvent):void
		{
			trace("ioErrorHandler: " + event);
			m_bCloseByServer=true;
			CloseSocket(true);
		}

		private function securityErrorHandler(event:SecurityErrorEvent):void
		{
			trace("securityErrorHandler: " + event);
			m_bCloseByServer=true;
			CloseSocket(true);
		}

		private function socketDataHandler(event:ProgressEvent):void
		{
			try
			{
				//读取数据
				var iRetCode:int=m_hSocket.bytesAvailable;

				//trace("RecvBuf:RecvBuf.position " + m_cbRecvBuf.position + " RecvBuf.length " + m_cbRecvBuf.length + " RecvSize " + m_wRecvSize);
				m_hSocket.readBytes(m_cbRecvBuf,m_wRecvSize,iRetCode);
				trace("RecvBuf:RecvBuf.position " + m_cbRecvBuf.position + " RecvBuf.length " + m_cbRecvBuf.length + " RecvSize " + m_wRecvSize);

				m_wRecvSize+= iRetCode;

				var myDate1:Date=new Date;
				m_dwRecvTickCount=myDate1.getMilliseconds() / 1000;
				//处理数据
				while (m_wRecvSize >= CMD_Head.sizeof_CMD_Head)
				{
					//变量定义
					var wPacketSize:int=0;
					var pHead:CMD_Head=CMD_Head.readData(m_cbRecvBuf);

					wPacketSize=pHead.CmdInfo.wDataSize;
					if (pHead.CmdInfo.cbMessageVer != SOCKET_VER)
					{
						trace("socketDataHandler: 数据包版本错误");
						throw Error("数据包版本错误");
					}
					if (wPacketSize > SOCKET_PACKAGE + CMD_Head.sizeof_CMD_Head)
					{
						trace("socketDataHandler: 数据包太大");
						throw Error("数据包太大");
					}
					if (m_wRecvSize < wPacketSize)
					{
						trace("socketDataHandler: no more data" + m_wRecvSize +"<"+ wPacketSize);
						return;
					}
					//拷贝数据
					m_dwRecvPacketCount++;
					var cbDataBuffer:ByteArray=newLitteEndianByteArray();
					CopyMemory(cbDataBuffer,m_cbRecvBuf,wPacketSize);
					m_wRecvSize-= wPacketSize;

					var cbNewRecvBuf:ByteArray=newLitteEndianByteArray();
					CopyMemory(cbNewRecvBuf,m_cbRecvBuf,m_wRecvSize, 0, wPacketSize);
					m_cbRecvBuf = cbNewRecvBuf;

					//解密数据
					var wRealySize:int=CrevasseBuffer(cbDataBuffer,wPacketSize);

					//解释数据
					var wDataSize:int=wRealySize - CMD_Head.sizeof_CMD_Head;
					cbDataBuffer.position= CMD_Info.sizeof_CMD_Info;
					var Command:CMD_Command=CMD_Command.readData(cbDataBuffer);

					var pDataBuffer:ByteArray=newLitteEndianByteArray();
					CopyMemory(pDataBuffer, cbDataBuffer, wDataSize, 0, CMD_Head.sizeof_CMD_Head);

					//内核命令
					if (Command.wMainCmdID == MDM_KN_COMMAND)
					{
						switch (Command.wSubCmdID)
						{
							case SUB_KN_DETECT_SOCKET ://网络检测
								{
									//发送数据
									SendData(MDM_KN_COMMAND,SUB_KN_DETECT_SOCKET,pDataBuffer,wDataSize);
									break;

							}
						};
						continue;
					}
					var bSuccess:Boolean = false;
					//处理数据
					if (m_pIClientSocketSink)
					{
						bSuccess=m_pIClientSocketSink.OnSocketRead(Command.wMainCmdID,Command.wSubCmdID,pDataBuffer,wDataSize,this as IClientSocket);
					}
					if (bSuccess == false)
					{
						var pRecvSink:IClientSocketRecvSink;

						for(var i:int = 0;i < m_arrSocketRecvSink.size(); i ++)
						{
							pRecvSink = m_arrSocketRecvSink.get(i);
							bSuccess = pRecvSink.OnSocketRead(Command.wMainCmdID,Command.wSubCmdID,pDataBuffer,wDataSize,this as IClientSocket);
							if(bSuccess)
								break;
						}
						if(bSuccess == false)
							throw Error("网络数据包处理失败:" + Command.wMainCmdID + " " + Command.wSubCmdID);
					}
				}
			}
			catch (e:Error)
			{
				trace(e);
				trace(e.getStackTrace());
				System.setClipboard(e.getStackTrace());
				CloseSocket(true);
			}
		}
		//加密数据
		private function EncryptBuffer(pcbDataBuffer:ByteArray,wDataSize:int,wBufferSize:int):int
		{
			//调整长度
			var wEncryptSize:int=wDataSize-CMD_Command.sizeof_CMD_Command;
			var wSnapCount:int=0;
			if ((wEncryptSize%sizeof_DWORD)!=0)
			{
				wSnapCount=sizeof_DWORD-wEncryptSize%sizeof_DWORD;
				memset(pcbDataBuffer,0,wSnapCount,CMD_Info.sizeof_CMD_Info+wEncryptSize);
			}
			//效验码与字节映射
			var cbCheckCode:int=0;
			for (var i:int=CMD_Info.sizeof_CMD_Info; i<wDataSize; i++)
			{
				cbCheckCode+=pcbDataBuffer[i];
/*trace("MapSendByte " + i +
  " v1 " + pcbDataBuffer[i] + 
  " v2 " + MapSendByte(pcbDataBuffer[i]));*/
				pcbDataBuffer[i]=MapSendByte(pcbDataBuffer[i]);
			}
/*trace("EncryptBuffer wEncryptSize " + wEncryptSize + 
  " cbCheckCode " + cbCheckCode +
  " bytesAvailable " + pcbDataBuffer.bytesAvailable);*/

			//填写信息头
			var CmdInfo:CMD_Info=new CMD_Info;
			CmdInfo.wDataSize=wDataSize;
			CmdInfo.cbCheckCode=~cbCheckCode+1;
			CmdInfo.cbMessageVer=SOCKET_VER;
			var CmdInfoByteArray:ByteArray = CmdInfo.toByteArray();

			memsetByByteArray(pcbDataBuffer, CmdInfoByteArray, CmdInfoByteArray.length);

			//设置变量
			m_dwSendPacketCount++;
			return wDataSize;
		}
		//解密数据
		private function CrevasseBuffer(pcbDataBuffer:ByteArray,wDataSize:int):int
		{
			//调整长度
			var wSnapCount:int=0;
			if ((wDataSize%sizeof_DWORD)!=0)
			{
				wSnapCount=sizeof_DWORD-wDataSize%sizeof_DWORD;
				memset(pcbDataBuffer,0,wSnapCount,wDataSize);
			}
			//效验码与字节映射
			var pHead:CMD_Head=CMD_Head.readData(pcbDataBuffer);
			var cbCheckCode:int=pHead.CmdInfo.cbCheckCode;
/*trace("CrevasseBuffer:wDataSize " + pHead.CmdInfo.wDataSize +
  " cbCheckCode " + pHead.CmdInfo.cbCheckCode + 
  " cbMessageVer " + pHead.CmdInfo.cbMessageVer);*/

			for (var i:int=CMD_Info.sizeof_CMD_Info; i<wDataSize; i++)
			{
				//trace(" src " + pcbDataBuffer[i] + " map " + MapRecvByte(pcbDataBuffer[i]) + " " + cbCheckCode);
				pcbDataBuffer[i]=MapRecvByte(pcbDataBuffer[i]);
				cbCheckCode+=pcbDataBuffer[i];
				cbCheckCode /= 256;
			}
			if (cbCheckCode!=0)
			{
				trace("CrevasseBuffer: 数据包效验码错误 " + cbCheckCode);
				throw Error("数据包效验码错误");
			}//设置变量
			m_dwSendPacketCount++;
			return wDataSize;
		}
		//发送数据
		private function SendBuffer(pBuffer:ByteArray,wSendSize:int):Boolean
		{
			m_hSocket.writeBytes(pBuffer);
			m_hSocket.flush();
			return true;
		}
		//映射发送数据
		private function MapSendByte(cbData:int):int
		{
			return m_SendByteMap[cbData];
		}
		//映射接收数据
		private function MapRecvByte(cbData:int):int
		{
			return m_RecvByteMap[cbData];
		}
		private function timerHandler(e:TimerEvent):void
		{
			if(m_hSocket != null && 
			   m_hSocket.connected)
			{
				SendCmd(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
			}
		}
	}

}
include "InterfaceModuleIDef.as"
include "NetModuleIDef.as"
include "GLOBALDEF.as"
include "../Common/Memory.as"