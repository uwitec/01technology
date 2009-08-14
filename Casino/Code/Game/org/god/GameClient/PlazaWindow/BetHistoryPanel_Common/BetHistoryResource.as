package 
{
	import org.god.SkinControl.SkinImage;
	import org.god.Common.GameTypeAndGameKindUtil;
	import org.god.Common.BetHistory.IBetHistoryResource;
	import org.god.Common.BetHistory.GameModule.B_Sparrow.SparrowResource;
	
	import org.aswing.util.HashMap;
	//游戏记录资源
	public class BetHistoryResource implements IBetHistoryResource
	{
		private var m_GameEndViewImageMap:HashMap;
		private var m_ImageResourceMap:HashMap;
		private var m_SparrowResource:SparrowResource;
		
		public function BetHistoryResource()
		{
		}
		public function Create():void
		{
			m_GameEndViewImageMap = new HashMap;
			
			var imageCommonGameEndView:SkinImage = new SkinImage(IMAGE_COMMONGAMEENDVIEW,680,640);
			var arrKind:Array = GameTypeAndGameKindUtil.GetAllGameKindArray();
			for(var i:uint = 0; i < arrKind.length; i ++)
			{
				m_GameEndViewImageMap.put(i, imageCommonGameEndView);
			}
			
			m_ImageResourceMap = new HashMap;
			m_ImageResourceMap.put("IMAGE_CARD_BH", new SkinImage(IMAGE_CARD_BH,819,428));
			m_ImageResourceMap.put("IMAGE_CGEV_CELL_UNCOVERPIG", new SkinImage(IMAGE_CGEV_CELL_UNCOVERPIG,650,103));
			m_ImageResourceMap.put("IMAGE_CGEV_CELL_WINSCORE", new SkinImage(IMAGE_CGEV_CELL_WINSCORE,650,149));
			m_ImageResourceMap.put("IMAGE_HEARTS_2", new SkinImage(IMAGE_HEARTS_2,28,12)); 
			m_ImageResourceMap.put("IMAGE_HEARTS_3", new SkinImage(IMAGE_HEARTS_3,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_4", new SkinImage(IMAGE_HEARTS_4,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_5", new SkinImage(IMAGE_HEARTS_5,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_6", new SkinImage(IMAGE_HEARTS_6,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_7", new SkinImage(IMAGE_HEARTS_7,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_8", new SkinImage(IMAGE_HEARTS_8,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_9", new SkinImage(IMAGE_HEARTS_9,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_10", new SkinImage(IMAGE_HEARTS_10,28,12));                    
			m_ImageResourceMap.put("IMAGE_HEARTS_J", new SkinImage(IMAGE_HEARTS_J,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_Q", new SkinImage(IMAGE_HEARTS_Q,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_K", new SkinImage(IMAGE_HEARTS_K,28,12));                     
			m_ImageResourceMap.put("IMAGE_HEARTS_A", new SkinImage(IMAGE_HEARTS_A,28,12));                     
			m_ImageResourceMap.put("IMAGE_DIAMONDS_J", new SkinImage(IMAGE_DIAMONDS_J,28,12));                   
			m_ImageResourceMap.put("IMAGE_SPADE_Q", new SkinImage(IMAGE_SPADE_Q,28,12));  
			m_ImageResourceMap.put("IMAGE_CLUBS_10", new SkinImage(IMAGE_CLUBS_10,28,12)); 
			
			//加载位图
			m_SparrowResource = new SparrowResource;
			m_SparrowResource.NewResource();
			//用户麻将
			m_SparrowResource.m_ImageUserTop.Load(IMAGE_SPARROW_USER_TOP,26,37);
			m_SparrowResource.m_ImageUserLeft.Load(IMAGE_SPARROW_USER_LEFT,19,49);
			m_SparrowResource.m_ImageUserRight.Load(IMAGE_SPARROW_USER_RIGHT,19,49);
			m_SparrowResource.m_ImageUserBottom.LoadResource(IMAGE_SPARROW_USER_BOTTOM,
															 1806,
															 60,
															 42,
															 60);

			//桌子麻将
			m_SparrowResource.m_ImageTableTop.LoadResource(IMAGE_SPARROW_TABLE_TOP,
														   1118,36,
														   26,25);
			m_SparrowResource.m_ImageTableLeft.LoadResource(IMAGE_SPARROW_TABLE_LEFT,
															1419,32,
															33,21);
			m_SparrowResource.m_ImageTableRight.LoadResource(IMAGE_SPARROW_TABLE_RIGHT,
															 1419,32,
															 33,21);
			m_SparrowResource.m_ImageTableBottom.LoadResource(IMAGE_SPARROW_TABLE_BOTTOM,
															  1118,36,
															  26,25);

			m_SparrowResource.m_ImageBackH.Load(IMAGE_SPARROW_BACK_H,26,36);
			m_SparrowResource.m_ImageBackV.Load(IMAGE_SPARROW_BACK_V,33,32);
			m_SparrowResource.m_ImageHeapSingleV.Load(IMAGE_SPARROW_HEAP_SINGLE_V,22,23);
			m_SparrowResource.m_ImageHeapSingleH.Load(IMAGE_SPARROW_HEAP_SINGLE_H,17,26);
			m_SparrowResource.m_ImageHeapDoubleV.Load(IMAGE_SPARROW_HEAP_DOUBLE_V,22,34);
			m_SparrowResource.m_ImageHeapDoubleH.Load(IMAGE_SPARROW_HEAP_DOUBLE_H,17,37);
		}
		public function Destroy():void
		{
			var arrImage:Array = m_GameEndViewImageMap.values();
			for(var i:uint = 0; i < arrImage.length; i ++)
			{
				arrImage[i].Unload();
			}
			m_GameEndViewImageMap.clear();
			m_GameEndViewImageMap = null;
			
			arrImage = m_ImageResourceMap.values();
			for(var i:uint = 0; i < arrImage.length; i ++)
			{
				arrImage[i].Unload();
			}
			m_ImageResourceMap.clear();
			m_ImageResourceMap = null;
			m_SparrowResource.UnloadResource();
			m_SparrowResource = null;
		}
		public function GetGameEndViewImage(wProcessType:uint):SkinImage
		{
			return m_GameEndViewImageMap.get(wProcessType);
		}
		public function GetResource(strResName:String):SkinImage
		{
			return m_ImageResourceMap.get(strResName);
		}
		public function GetResourcePackage(strResName:String):*
		{
			if(strResName == "SparrowResource")
				return m_SparrowResource;
			else
				return null;
		}
	}
}