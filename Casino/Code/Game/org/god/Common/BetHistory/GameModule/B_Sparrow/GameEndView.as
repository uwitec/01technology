package org.god.Common.BetHistory.GameModule.B_Sparrow
{
	import flash.display.BitmapData;
	import flash.display.Graphics;
	import flash.geom.Point;
	import flash.geom.Rectangle;
	
	import org.god.Common.DrawUtil;
	import org.god.Common.BetHistory.IBetHistoryResource;
	import org.god.Common.BetHistory.GameModule.Common.BaseBattleGameEndView;
	import org.god.SkinControl.SkinImage;
	import org.god.Net.BetHistory.tagBetHistoryData;
	import org.god.GameModule.Common.enXCollocateMode;
	import org.god.GameModule.Common.enYCollocateMode;
	import org.god.GameModule.Common.enDirection;
	import org.god.GameModule.B_Sparrow.Common.CMD_S_GameEnd;
	import org.god.GameModule.B_Sparrow.Common.GameLogic;
	//游戏结束视图
	public class GameEndView extends BaseBattleGameEndView
	{
		protected var m_GameEnd:CMD_S_GameEnd;
		protected var m_TableSparrow:Array=new Array(4);//桌面麻将
		protected var m_WeaveSparrow:Array=new Array(4);//组合麻将
		protected var m_SparrowResource:SparrowResource;
		
		public function GameEndView(bh:tagBetHistoryData,
										pIBetHistoryResource:IBetHistoryResource)
		{
			super(bh,pIBetHistoryResource);
			m_GameEnd = CMD_S_GameEnd.readData(bh.cbEndData);
			SetWinScore(m_GameEnd.fGameScore,4);
			m_SparrowResource = pIBetHistoryResource.GetResourcePackage("SparrowResource");
			for(var i:uint = 0; i < m_TableSparrow.length; i ++)
			{
				m_TableSparrow[i] = new TableSparrow(m_SparrowResource);
				
			}
			for(i = 0; i < m_WeaveSparrow.length; i ++)
			{
				m_WeaveSparrow[i] = new Array(4);
				for(var j:uint = 0; j < m_WeaveSparrow[i].length; j ++)
				{
					m_WeaveSparrow[i][j] = new WeaveSparrow(m_SparrowResource);
					
				}
			}
			for (var i:uint=0; i < 4; i++)
			{
				//用户麻将
				m_TableSparrow[i].SetDirection(enDirection.Direction_South);
				

				//组合麻将
				m_WeaveSparrow[i][0].SetDisplayItem(true);
				m_WeaveSparrow[i][1].SetDisplayItem(true);
				m_WeaveSparrow[i][2].SetDisplayItem(true);
				m_WeaveSparrow[i][3].SetDisplayItem(true);
				m_WeaveSparrow[i][0].SetDirection(enDirection.Direction_South);
				m_WeaveSparrow[i][1].SetDirection(enDirection.Direction_South);
				m_WeaveSparrow[i][2].SetDirection(enDirection.Direction_South);
				m_WeaveSparrow[i][3].SetDirection(enDirection.Direction_South);
			}
			var nXP:int = 188;
			var nYP:int = 60;
			for (var i:uint=0; i < 4; i++)
			{
				//桌面麻将
				if (m_GameEnd.cbChiHuSparrow!=0)
				{
					if(m_GameEnd.wChiHuKind[i]!=0)
					{
						m_TableSparrow[i].SetControlPoint(nXP + 310 + 25,nYP + i * 92 + 12);
					}
					else
					{
						m_TableSparrow[i].SetControlPoint(nXP + 310 ,nYP + i * 92 + 12);
					}
				}
				else
				{
					m_TableSparrow[i].SetControlPoint(nXP + 310 ,nYP + i * 92 + 12);
				}
				
	
				//组合麻将
				m_WeaveSparrow[i][0].SetControlPoint(nXP ,nYP + i * 92 - 40);
				m_WeaveSparrow[i][1].SetControlPoint(nXP + 95,nYP + i * 92 - 40);
				m_WeaveSparrow[i][2].SetControlPoint(nXP + 95 + 95,nYP + i * 92 - 40);
				m_WeaveSparrow[i][3].SetControlPoint(nXP + 95 + 95 + 95,nYP + i * 92 - 40);
	
			}

			//用户麻将
			for (var i:uint=0;i<4;i++)
			{
				m_TableSparrow[i].SetSparrowData(m_GameEnd.cbSparrowData[i],
													m_GameEnd.cbSparrowCount[i]);
			}
			var t_GameLogic:GameLogic = new GameLogic;//游戏逻辑
			//组合麻将
			var cbWeaveSparrow:Array=new Array(0,0,0,0);
			for (var i:uint=0;i<4;i++)
			{
				for (var j:uint=0;j<m_GameEnd.cbWeaveCount[i];j++)
				{
					var cbWeaveKind:uint=m_GameEnd.WeaveItemArray[i][j].cbWeaveKind;
					var cbCenterSparrow:uint=m_GameEnd.WeaveItemArray[i][j].cbCenterSparrow;
					var cbWeaveSparrowCount:uint=t_GameLogic.GetWeaveSparrow(cbWeaveKind,cbCenterSparrow,cbWeaveSparrow);
					m_WeaveSparrow[i][j].SetSparrowData(cbWeaveSparrow,cbWeaveSparrowCount);
				}
			}
		}
		
		//重画函数
		protected override function OnPaint(dc:BitmapData, rcClient:Rectangle):void
		{
			super.OnPaint(dc,rcClient);

			if(m_GameEnd)
			{
				//桌面麻将
				for (var i:int=0;i<4;i++)
				{
					m_TableSparrow[i].DrawSparrowControl(dc);
					
					m_WeaveSparrow[i][0].DrawSparrowControl(dc);
					m_WeaveSparrow[i][1].DrawSparrowControl(dc);
					m_WeaveSparrow[i][2].DrawSparrowControl(dc);
					m_WeaveSparrow[i][3].DrawSparrowControl(dc);
				}
				var nXP:int = 10;
				var nYP:int = 52;
				
				var strText:String;
				for(var i:int = 0; i < m_TableSparrow.length; i ++)
				{
					
					strText = GetChairIDName(i) + ":"
					if(m_GameEnd.wBankerUser == i)
						strText = strText + IDS_BR_B_BANK;
					if (m_GameEnd.cbChiHuSparrow!=0)
					{
						if(m_GameEnd.wChiHuKind[i]!=0)
						{
							strText = strText + " " + IDS_HUPAI;
						}
						else if(m_GameEnd.wProvideUser == i)
						{
							strText = strText + " " + IDS_FANGPAO;
						}
					}

					DrawUtil.TextOut(dc,
					 nXP,
					 nYP + i * 92,
					 strText,
				 	 18,
					 0xcc9900);
				}
			}
		}
		public override function Destroy():void
		{
			super.Destroy();
			m_GameEnd = null;
			ZeroArray(m_TableSparrow, null, TableSparrow.Destroy);
			m_TableSparrow = null;
			FreeTwoDimensionArray(m_WeaveSparrow, null, WeaveSparrow.Destroy);
			m_WeaveSparrow = null;
		}
	}
}
include "../../StringTableDef.as"
include "../../../Memory.as"