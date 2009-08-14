package 
{
	import flash.display.Sprite;
	import flash.display.Shape;
	import flash.display.BitmapData;
	import flash.text.TextField;

	import org.god.SkinControl.SkinImage;
	import org.god.Common.DrawUtil;
	//金币视图
	public class GoldView
	{
		public static  const GOLD_IMAGE_WIDTH:Number=230/6;//筹码宽
		public static  const GOLD_IMAGE_HEIGHT:Number =45;//筹码高
		//变量定义
		public var m_lGold:int;//筹码数目
		public var m_lMaxLayer:int;//最大层数
		public var m_lGoldCount:Array = new Array(7);//筹码数目

		//辅助变量
		private static  var m_ImageGold:SkinImage;//筹码图片
		//构造函数
		public function GoldView()
		{
			m_lGold=0;
			m_lMaxLayer=4;
			ZeroArray(m_lGoldCount);
			if (m_ImageGold == null)
			{
				m_ImageGold = new SkinImage(IMAGE_GOLD, 230, 44);
			}
		}
		//设置筹码
		public function SetGold(lGold:int):void
		{
			if (m_lGold!=lGold)
			{
				m_lGold=lGold;
				RectifyGoldLayer();
			}
			return;
		}
		//获取筹码
		public function  GetGold():int
		{ 
			return m_lGold; 
		}
		//设置层数
		public function SetMaxGoldLayer(lMaxLayer:int):void
		{
			if (m_lMaxLayer!=lMaxLayer)
			{
				m_lMaxLayer=lMaxLayer;
				RectifyGoldLayer();
			}
			return;
		}
		//绘画筹码
		public function DrawGoldView(pDC:BitmapData, nXPos:Number, nYPos:Number):void
		{

			//绘画筹码
			var nYPosDraw:Number=nYPos-GOLD_IMAGE_HEIGHT/2;
			for (var i:uint=0; i<CountArray(m_lGoldCount); i++)
			{
				for (var j:uint=0; j<m_lGoldCount[i]; j++)
				{
					m_ImageGold.DrawBitmapData(pDC,
					nXPos-GOLD_IMAGE_WIDTH/2,
					nYPosDraw,
					i*GOLD_IMAGE_WIDTH,0,
					GOLD_IMAGE_WIDTH,GOLD_IMAGE_HEIGHT);

					nYPosDraw-=3;
				}
			}
			//绘画数字
			if (m_lGold!=0)
			{
				var szBuffer:String ="￥";
				szBuffer =  szBuffer + GetGlodString(m_lGold);
				DrawUtil.TextOut(pDC, 
								 nXPos,
								 nYPos+10,
								 szBuffer, 
								 14,
								 0xffff00,
								 0,
								 GOLD_IMAGE_HEIGHT,
								 "center");
				
			}
			return;
		}
		//调整筹码层
		public function RectifyGoldLayer():void
		{
			//变量定义
			var lBasicGold:Array =new Array(500,100,50,10,5,1);
			var lGold:int=m_lGold;
			var lGoldLayer:int=0;
			ZeroArray(m_lGoldCount);

			//调整筹码层
			for (var i:uint=0; i<CountArray(lBasicGold); i++)
			{
				if (lGold>=lBasicGold[i])
				{
					m_lGoldCount[i]=lGold/lBasicGold[i];
					lGold-=m_lGoldCount[i]*lBasicGold[i];
					lGoldLayer+=m_lGoldCount[i];
					if (lGoldLayer>=m_lMaxLayer)
					{
						m_lGoldCount[i]-=(lGoldLayer-m_lMaxLayer);
						return;
					}
					if (lGold==0)
					{
						break;
					}
				}
			}
			return;
		}
		//整性变字符
		public function GetGlodString(lGold:int):String
		{
			//转换字符
			var dwNumBit:int=0;
			var dwCharBit:int=0;
			var tmp:Array = new Array;
			do
			{
				dwNumBit++;
				tmp[dwCharBit++]=String(lGold%10);
				if (dwNumBit%3==0)
				{
					tmp[dwCharBit++]=",";
				}
				lGold/=10;
			} while (lGold!=0);

			//调整字符
			if (tmp[dwCharBit-1]==String(","))
			{
				tmp[dwCharBit-1]="";
			}
			tmp.reverse();
			var szString:String = "";
			for(var i = 0; i < tmp.length; i ++)
			{
				szString += tmp[i];
			}
			return szString;
		}
		
	}
}
include "../../Common/Memory.as"