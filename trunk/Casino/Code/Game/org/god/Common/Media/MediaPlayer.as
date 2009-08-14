package org.god.Common.Media
{
	import flash.media.Video;
	import flash.net.NetConnection;
 	import flash.net.NetStream;
	import flash.net.ObjectEncoding;
	import flash.events.NetStatusEvent;
	import flash.events.SecurityErrorEvent;
	import flash.events.AsyncErrorEvent;
	//视频播放器
	public class MediaPlayer extends Video
	{
		protected var m_pNetConnection:NetConnection;
		protected var m_pNetStream:NetStream;
		protected var m_strMediaAddress:String = "";
		protected var m_strMediaName:String = "";
		protected var m_fBufferTime:Number = 0.1;
		
		public function MediaPlayer(w:int = 320,h:int = 240,fBufferTime:Number = 0.1)
		{
			super(w,h);
			
			m_fBufferTime = fBufferTime;
			
			m_pNetConnection = new NetConnection();
			m_pNetConnection.objectEncoding = ObjectEncoding.AMF0;
            m_pNetConnection.addEventListener(NetStatusEvent.NET_STATUS, OnEventNetStatusHandler);
            m_pNetConnection.addEventListener(SecurityErrorEvent.SECURITY_ERROR, OnEventSecurityErrorHandler);
		}
		
		public function Play(strURL:String):void
		{
			Stop();
			var s:int = strURL.lastIndexOf("/");
			if(s != -1)
			{
				m_strMediaAddress = strURL.substr(0, s);
				m_strMediaName = strURL.substring(s+1, strURL.length);
				
				trace(m_strMediaAddress);
				m_pNetConnection.connect(m_strMediaAddress);
			}
		}
		public function Stop():void
		{
			m_pNetConnection.close();
			disconnectStream();
		}
		public function Destroy():void
		{
			Stop();
            m_pNetConnection.removeEventListener(NetStatusEvent.NET_STATUS, OnEventNetStatusHandler);
            m_pNetConnection.removeEventListener(SecurityErrorEvent.SECURITY_ERROR, OnEventSecurityErrorHandler);
			m_pNetConnection = null;
		}
		public function GetLiveDelay():Number
		{
			if(m_pNetConnection != null&& 
			   m_pNetConnection.connected == true&&
			   m_pNetStream != null)
				return m_pNetStream.liveDelay;
			else
				return -1.0;
		}
		////////////////////////////////////////////////////////////////
		protected function connectStream():void
		{
			disconnectStream();
            m_pNetStream = new NetStream(m_pNetConnection);
			m_pNetStream.client = this;
			m_pNetStream.bufferTime = m_fBufferTime;
            m_pNetStream.addEventListener(NetStatusEvent.NET_STATUS, OnEventNetStatusHandler);
            m_pNetStream.addEventListener(AsyncErrorEvent.ASYNC_ERROR, OnEventAsyncErrorHandler);
            this.attachNetStream(m_pNetStream);
            m_pNetStream.play(m_strMediaName);
        }
		protected function disconnectStream():void
		{
			if(m_pNetStream)
			{
				m_pNetStream.close();
				this.attachNetStream(null);
				//m_pNetStream.client = m_pNetStream;
           	 	m_pNetStream.removeEventListener(NetStatusEvent.NET_STATUS, OnEventNetStatusHandler);
            	m_pNetStream.removeEventListener(AsyncErrorEvent.ASYNC_ERROR, OnEventAsyncErrorHandler);
				m_pNetStream = null;
			}
        }
		protected function OnEventNetStatusHandler(e:NetStatusEvent):void
		{
			trace(e.info.code);
			switch (e.info.code) 
			{
                case "NetConnection.Connect.Success":
                    connectStream();
                    break;
                case "NetStream.Play.StreamNotFound":
                    break;
				default:
					break;
            }
		}
		protected function OnEventSecurityErrorHandler(e:SecurityErrorEvent):void
		{
			trace(e);
		}
		protected function OnEventAsyncErrorHandler(e:AsyncErrorEvent):void 
		{
			trace(e);
		}
		public function onMetaData(info:Object):void 
		{
       	 	trace("metadata: duration=" + info.duration + " width=" + info.width + " height=" + info.height + " framerate=" + info.framerate);
    	}
  		public function onCuePoint(info:Object):void 
		{
        	trace("cuepoint: time=" + info.time + " name=" + info.name + " type=" + info.type);
   		}
		public function onPlayStatus(info:Object):void
		{
			trace(info);
		}
	}
}