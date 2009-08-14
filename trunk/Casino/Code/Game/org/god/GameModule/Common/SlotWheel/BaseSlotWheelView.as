package org.god.GameModule.Common.SlotWheel
{
	import flash.display.MovieClip;
	import flash.utils.Timer;
	import flash.events.TimerEvent;
	import flash.events.Event;
	
	public class BaseSlotWheelView extends MovieClip
	{
		public static const STOPMODE_SINGLE:int = 0;
		public static const STOPMODE_ROW:int = STOPMODE_SINGLE + 1;
		public static const STOPMODE_COL:int = STOPMODE_ROW + 1;
		public static const STOPMODE_ALL:int = STOPMODE_COL + 1;
		
		public static const EVENTNAME_START:String = "SlotWheelView_Start";
		public static const EVENTNAME_STOP:String = "SlotWheelView_Stop";
		
		protected var m_SlotWheelResource:SlotWheelResource;
		protected var m_SlotWheelConfig:BaseSlotWheelConfig;
		protected var m_BaseWeaveSlotItems:Array;
		protected var m_BaseWeaveSlotItemClipMasks:Array;
		protected var m_BaseWeaveSlotItemStartFlag:Array;
		protected var m_StartTimer:Timer;
		protected var m_bStartAndStopFlag:Boolean;
		protected var m_nStartAndStopItemIndex:int;
		protected var m_astSlotItemTypes:Array;
		protected var m_stopMode:int;

		public function BaseSlotWheelView(res:SlotWheelResource,
									  config:BaseSlotWheelConfig)
		{
			super();
			m_SlotWheelResource = res;
			m_SlotWheelConfig	= config;
			
			MoveWindow(m_SlotWheelConfig.m_ViewBasicStationX,
					   m_SlotWheelConfig.m_ViewBasicStationY);
			
			m_BaseWeaveSlotItemStartFlag = new Array(m_SlotWheelConfig.m_cbSlotWheelRow);
			m_BaseWeaveSlotItems = new Array(m_SlotWheelConfig.m_cbSlotWheelRow);
			m_BaseWeaveSlotItemClipMasks = new Array(m_SlotWheelConfig.m_cbSlotWheelRow);
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				m_BaseWeaveSlotItemStartFlag[r] = new Array(m_SlotWheelConfig.m_cbSlotWheelCol);
				m_BaseWeaveSlotItems[r] = new Array(m_SlotWheelConfig.m_cbSlotWheelCol);
				m_BaseWeaveSlotItemClipMasks[r] = new Array(m_SlotWheelConfig.m_cbSlotWheelCol);
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c ++)
				{
					m_BaseWeaveSlotItems[r][c] = CreateWeaveSlotItem();
					addChild(m_BaseWeaveSlotItems[r][c]);
					m_BaseWeaveSlotItemClipMasks[r][c] = CreateWeaveSlotItemClipMask();
					addChild(m_BaseWeaveSlotItemClipMasks[r][c]);
					m_BaseWeaveSlotItems[r][c].mask = m_BaseWeaveSlotItemClipMasks[r][c];
					
					m_BaseWeaveSlotItemStartFlag[r][c] = false;
				}
			}
			
			RectifyWeaveSlotItem();
			
			m_StartTimer = new Timer(200,m_SlotWheelConfig.m_cbSlotWheelRow*m_SlotWheelConfig.m_cbSlotWheelCol);
			m_StartTimer.addEventListener(TimerEvent.TIMER, OnEventStartTimer);
			m_astSlotItemTypes = null;
		}
		public function Destroy():void
		{
			m_StartTimer.removeEventListener(TimerEvent.TIMER, OnEventStartTimer);
			m_StartTimer = null;
			
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c ++)
				{
					removeChild(m_BaseWeaveSlotItems[r][c]);
					m_BaseWeaveSlotItems[r][c].Destroy();
					m_BaseWeaveSlotItems[r][c] = null;
					if(m_BaseWeaveSlotItemClipMasks[r][c])
					{
						removeChild(m_BaseWeaveSlotItemClipMasks[r][c]);
						m_BaseWeaveSlotItemClipMasks[r][c] = null;
					}
				}
				m_BaseWeaveSlotItems[r] = null;
				m_BaseWeaveSlotItemClipMasks[r] = null;
				m_BaseWeaveSlotItemStartFlag[r] = null;
			}
			m_BaseWeaveSlotItemStartFlag = null;
			m_BaseWeaveSlotItems = null;
			m_BaseWeaveSlotItemClipMasks = null;
			m_SlotWheelResource.Destroy();
			m_SlotWheelResource = null;
			m_SlotWheelConfig.Destroy();
			m_SlotWheelConfig 	= null;
		}
		public function GetResource():SlotWheelResource
		{
			return m_SlotWheelResource;
		}
		public function GetConfig():BaseSlotWheelConfig
		{
			return m_SlotWheelConfig;
		}
		public function Start():void
		{
			if(m_bStartAndStopFlag || IsWeaveSlotItemAllStop() == false)
				return;
			m_nStartAndStopItemIndex= 0;
			m_bStartAndStopFlag = true;
			m_StartTimer.reset();
			m_StartTimer.start();
			dispatchEvent(new Event(EVENTNAME_START));
		}
		public function Stop(lastSlotItemTypes:Array, stopMode:int = 0):void
		{
			if(!m_bStartAndStopFlag || IsWeaveSlotItemAllStop() == true)
				return;
			m_stopMode = stopMode;
			m_astSlotItemTypes = lastSlotItemTypes;
			m_nStartAndStopItemIndex= 0;
			m_bStartAndStopFlag = false;
			
			if(m_stopMode == STOPMODE_ALL)
			{
				StopSlotWheelScrollMotion_All();
			}
			else if(m_stopMode == STOPMODE_COL)
			{
				StopNextSlotWheelScrollMotion_Col();
			}
			else if(m_stopMode == STOPMODE_ROW)
			{
				StopNextSlotWheelScrollMotion_Row();
			}
			else if(m_stopMode == STOPMODE_SINGLE)
			{
				StopNextSlotWheelScrollMotion_Single();
			}
		}
		public function MoveWindow(xp:int,yp:int):void
		{
			this.x = xp;
			this.y = yp;
		}
		public function AddActionListener_Start(func:Function):void
		{
			this.addEventListener(EVENTNAME_START,func);
		}
		public function RemoveActionListener_Start(func:Function):void
		{
			this.removeEventListener(EVENTNAME_START,func);
		}
		public function AddActionListener_Stop(func:Function):void
		{
			this.addEventListener(EVENTNAME_STOP,func);
		}
		public function RemoveActionListener_Stop(func:Function):void
		{
			this.removeEventListener(EVENTNAME_STOP,func);
		}
		public function IsWeaveSlotItemAllStop():Boolean
		{
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c ++)
				{
					if(m_BaseWeaveSlotItemStartFlag[r][c])
					{
						return false;
					}
				}
			}
			return true;
		}
		public function IsWeaveSlotItemRowStop(r:int):Boolean
		{
			for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c ++)
			{
				if(m_BaseWeaveSlotItemStartFlag[r][c])
				{
					return false;
				}
			}
			return true;
		}

		public function IsWeaveSlotItemColStop(c:int):Boolean
		{
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				if(m_BaseWeaveSlotItemStartFlag[r][c])
				{
					return false;
				}
			}
			return true;
		}
		public function StopSlotWheelScrollMotion_All():void
		{
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c ++)
				{
					m_BaseWeaveSlotItems[r][c].Stop(m_astSlotItemTypes[r][c]);
				}
			}
		}
		public function StopNextSlotWheelScrollMotion_Single():void
		{
			var r:int = m_nStartAndStopItemIndex / m_SlotWheelConfig.m_cbSlotWheelCol;
			var c:int = m_nStartAndStopItemIndex % m_SlotWheelConfig.m_cbSlotWheelCol;
			if(m_BaseWeaveSlotItems[r][c])
				m_BaseWeaveSlotItems[r][c].Stop(m_astSlotItemTypes[r][c]);
		}
		public function StopNextSlotWheelScrollMotion_Row():void
		{
			var r:int = m_nStartAndStopItemIndex % m_SlotWheelConfig.m_cbSlotWheelRow;
			for(var c:int = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol; c++)
			{
				if(m_BaseWeaveSlotItems[r][c])
					m_BaseWeaveSlotItems[r][c].Stop(m_astSlotItemTypes[r][c]);
			}
		}
		public function StopNextSlotWheelScrollMotion_Col():void
		{
			var c:int = m_nStartAndStopItemIndex % m_SlotWheelConfig.m_cbSlotWheelCol;
			for(var r:int = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow; r++)
			{
				if(m_BaseWeaveSlotItems[r][c])
					m_BaseWeaveSlotItems[r][c].Stop(m_astSlotItemTypes[r][c]);
			}
		}
		//////////
		protected function OnEventStartTimer(e:TimerEvent):void
		{
			if(m_bStartAndStopFlag)
			{
				var r:int = m_nStartAndStopItemIndex / m_SlotWheelConfig.m_cbSlotWheelCol;
				var c:int = m_nStartAndStopItemIndex % m_SlotWheelConfig.m_cbSlotWheelCol;
				
				if(m_BaseWeaveSlotItems[r][c])
					m_BaseWeaveSlotItems[r][c].Start();
				m_nStartAndStopItemIndex ++;
			
				if(m_nStartAndStopItemIndex >= (m_SlotWheelConfig.m_cbSlotWheelCol*
											m_SlotWheelConfig.m_cbSlotWheelRow))
				{
					m_StartTimer.reset();
					m_StartTimer.stop();
				}
			}
			
		}
		public function OnSlotWheelScrollMotionStart(weaveSlotItem:BaseWeaveSlotItem):void
		{
			var b:Boolean = true;
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow && b; r++)
			{
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol && b; c ++)
				{
					if(m_BaseWeaveSlotItems[r][c] == weaveSlotItem)
					{
						m_BaseWeaveSlotItemStartFlag[r][c] = true;
						b = false;
					}
				}
			}
		}
		public function OnSlotWheelScrollMotionFinish(weaveSlotItem:BaseWeaveSlotItem):void
		{
			var b:Boolean = true;
			for(var r:uint = 0; r < m_SlotWheelConfig.m_cbSlotWheelRow && b; r++)
			{
				for(var c:uint = 0; c < m_SlotWheelConfig.m_cbSlotWheelCol && b; c ++)
				{
					if(m_BaseWeaveSlotItems[r][c] == weaveSlotItem)
					{
						m_BaseWeaveSlotItemStartFlag[r][c] = false;
						
						switch(m_stopMode)
						{
							case STOPMODE_SINGLE:
							{
								m_nStartAndStopItemIndex ++;
								if(m_nStartAndStopItemIndex < 
								   m_SlotWheelConfig.m_cbSlotWheelRow * m_SlotWheelConfig.m_cbSlotWheelCol)
								{
									StopNextSlotWheelScrollMotion_Single();
								}
								break;
							}
							case STOPMODE_ROW:
							{
								if(IsWeaveSlotItemRowStop(r))
								{
									m_nStartAndStopItemIndex ++;
									if(m_nStartAndStopItemIndex < m_SlotWheelConfig.m_cbSlotWheelRow)
									{
										StopNextSlotWheelScrollMotion_Row();
									}
								}
								break;
							}
							case STOPMODE_COL:
							{
								if(IsWeaveSlotItemColStop(c))
								{
									m_nStartAndStopItemIndex ++;
									if(m_nStartAndStopItemIndex < m_SlotWheelConfig.m_cbSlotWheelCol)
									{
										StopNextSlotWheelScrollMotion_Col();
									}
								}
								break;
							}
							default:
							break;
						}
						b = false;
					}
				}
			}

			if(IsWeaveSlotItemAllStop())
			{
				dispatchEvent(new Event(EVENTNAME_STOP));
			}
		}
		//////////
		//调整位置
		public function RectifyWeaveSlotItem():void
		{
		}
		public function CreateWeaveSlotItem():BaseWeaveSlotItem
		{
			return null;
		}
		public function CreateWeaveSlotItemClipMask():DefaultWeaveSlotItemClipMask
		{
			var clipmask:DefaultWeaveSlotItemClipMask = 
			new DefaultWeaveSlotItemClipMask(m_SlotWheelConfig.m_rectWeaveSlotItemClipMask);
			return clipmask;
		}
	}
}