package 
{
	import flash.display.Sprite;
	import flash.events.MouseEvent;
	import flash.geom.Rectangle;
	
	import org.god.SkinControl.ImageButton;

	import org.aswing.graphics.*;
	import org.aswing.ASColor;
	
	public class CallCardWnd extends Sprite
	{
		//花色定义
		public static  const CALL_NONE:uint=0x00;//叫牌定义
		public static  const CALL_NT:uint=0x10;//叫牌定义
		public static  const CALL_HEI_TAO:uint=0x01;//叫牌定义
		public static  const CALL_HONG_TAO:uint=0x02;//叫牌定义
		public static  const CALL_MEI_HUA:uint=0x04;//叫牌定义
		public static  const CALL_FANG_KUAI:uint=0x08;//叫牌定义

		public var m_btCallNT:ImageButton;//叫牌按钮
		public var m_btCallHeiTao:ImageButton;//叫牌按钮
		public var m_btCallHongTao:ImageButton;//叫牌按钮
		public var m_btCallMeiHua:ImageButton;//叫牌按钮
		public var m_btCallFangKuai:ImageButton;//叫牌按钮

		public function CallCardWnd()
		{
			m_btCallNT=new ImageButton(IMAGE_CALL_NT,120,24);
			addChild(m_btCallNT);
			m_btCallNT.addEventListener(MouseEvent.CLICK, OnEventCOLOR_NT);
			
			m_btCallHeiTao=new ImageButton(IMAGE_CALL_HEI_TAO,120,24);
			addChild(m_btCallHeiTao);
			m_btCallHeiTao.addEventListener(MouseEvent.CLICK, OnEventCOLOR_HEI_TAO);
			
			m_btCallHongTao=new ImageButton(IMAGE_CALL_HONG_TAO,120,24);
			addChild(m_btCallHongTao);
			m_btCallHongTao.addEventListener(MouseEvent.CLICK, OnEventCOLOR_HONG_TAO);
			
			m_btCallMeiHua=new ImageButton(IMAGE_CALL_MEI_HUA,120,24);
			addChild(m_btCallMeiHua);
			m_btCallMeiHua.addEventListener(MouseEvent.CLICK, OnEventCOLOR_MEI_HUA);
			
			m_btCallFangKuai=new ImageButton(IMAGE_CALL_FANG_KUAI,120,24);
			addChild(m_btCallFangKuai);
			m_btCallFangKuai.addEventListener(MouseEvent.CLICK, OnEventCOLOR_FANG_KUAI);

			//获取位置
			var rcButton:Rectangle=m_btCallNT.getButtonRect();

			//移动按钮
			m_btCallNT.MoveWindow(0,0);
			m_btCallHeiTao.MoveWindow(rcButton.width,0);
			m_btCallHongTao.MoveWindow(rcButton.width * 2,0);
			m_btCallMeiHua.MoveWindow(rcButton.width * 3,0);
			m_btCallFangKuai.MoveWindow(rcButton.width * 4,0);

		}
		//更新控制
		public function UpdateCallControl1(cbCallColor:uint):void
		{
			//更新控件
			m_btCallNT.EnableWindow(cbCallColor & CALL_NT?true:false);
			m_btCallHeiTao.EnableWindow(cbCallColor & CALL_HEI_TAO?true:false);
			m_btCallHongTao.EnableWindow(cbCallColor & CALL_HONG_TAO?true:false);
			m_btCallMeiHua.EnableWindow(cbCallColor & CALL_MEI_HUA?true:false);
			m_btCallFangKuai.EnableWindow(cbCallColor & CALL_FANG_KUAI?true:false);

			return;
		}
		//更新控制
		public function UpdateCallControl5(bCallNT:Boolean,bCallHeiTao:Boolean,bCallHongTao:Boolean,CallMeiHua:Boolean,bCallFangKuai:Boolean):void
		{
			//更新控件
			m_btCallNT.EnableWindow((bCallNT==true)?true:false);
			m_btCallHeiTao.EnableWindow((bCallHeiTao==true)?true:false);
			m_btCallHongTao.EnableWindow((bCallHongTao==true)?true:false);
			m_btCallMeiHua.EnableWindow((CallMeiHua==true)?true:false);
			m_btCallFangKuai.EnableWindow((bCallFangKuai==true)?true:false);

			return;
		}
		public function MoveWindow(nXPos:Number, nYPos:Number):void
		{
			this.x = nXPos;
			this.y = nYPos;
		}
		public function ShowWindow(bShow:Boolean):void
		{
			this.visible = bShow;
		}
		private function OnEventCOLOR_NT(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CALL_CARD,
											COLOR_NT,COLOR_NT));
		}
		private function OnEventCOLOR_HEI_TAO(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CALL_CARD,
											COLOR_HEI_TAO,COLOR_HEI_TAO));
		}
		private function OnEventCOLOR_HONG_TAO(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CALL_CARD,
											COLOR_HONG_TAO,COLOR_HONG_TAO));
		}
		private function OnEventCOLOR_MEI_HUA(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CALL_CARD,
											COLOR_MEI_HUA,COLOR_MEI_HUA));
		}
		private function OnEventCOLOR_FANG_KUAI(e:MouseEvent):void
		{
			dispatchEvent(new GameViewEvent(GameViewEvent.nGVM_CALL_CARD,
											COLOR_FANG_KUAI,COLOR_FANG_KUAI));
		}
	}
}
include "./Common/GameLogicDef.as"