package
{
	import flash.display.MovieClip;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.geom.Matrix;
	
	import org.god.GameModule.Common.IFaceView;
	
	import org.aswing.util.LinkedList;
	
	public class FaceView implements IFaceView
	{
		public static const nIndex_FarmerBoy = 0x00;
		public static const nIndex_FarmerGirl= 0x01;
		public static const nIndex_LandBoy 	 = 0xf0;
		public static const nIndex_LandGirl	 = 0xf1;
		
		public static const nIndex_FLMask	 = 0xf0;
		public static const nIndex_GenderMask= 0x0f;
	
		public static const nDirect_Left  =0;
		public static const nDirect_Right =1;
		
		public static const strMotion_Normal:String 		= "Normal";
		public static const strMotion_Bomb:String 			= "Bomb";
		public static const strMotion_FarmerToLand:String 	= "FTL";
		public static const strMotion_Offline:String 		= "Offline";
		
		private static const strFrameFlag_Farmer:String = "Farmer";
		private static const strFrameFlag_FBomb:String 	= "FBomb";
		private static const strFrameFlag_FOffline:String = "FOffline";
		private static const strFrameFlag_FTL:String 	= "FTL";
		private static const strFrameFlag_Land:String 	= "Land";
		private static const strFrameFlag_LBomb:String 	= "LBomb";
		private static const strFrameFlag_LOffline:String = "LOffline";

		private var m_nIndex:int;
		private var m_nDirect:int;
		private var m_mcFace:MovieClip;
		private var m_listMotion:LinkedList= new LinkedList;
		private var m_bPlaying:Boolean;
		
		public function FaceView(nIndex:int,nDirect:int)
		{
			createFace(nIndex,nDirect);
		}
		
		public function createFace(nIndex:int,nDirect:int):Boolean
		{
			m_nIndex  = nIndex;
			m_nDirect = nDirect;
			
			if(m_nIndex == nIndex_FarmerBoy ||
			   m_nIndex == nIndex_LandBoy )
			{
				if(m_nDirect == nDirect_Left)
				{
					m_mcFace = new FaceBoyLeft;
				}
				else
				{
					//m_mcFace = new FaceBoyRight;
					m_mcFace = new FaceBoyLeft;
				}
				if(m_nIndex == nIndex_FarmerBoy)
				{
					m_mcFace.gotoAndStop(strFrameFlag_Farmer);
				}
				else
				{
					m_mcFace.gotoAndStop(strFrameFlag_Land);
				}
			}
			else
			{
				if(m_nDirect == nDirect_Left)
				{
					m_mcFace = new FaceGirlLeft;
				}
				else
				{
					//m_mcFace = new FaceGirlRight;
					m_mcFace = new FaceGirlLeft;
				}
				if(m_nIndex == nIndex_FarmerGirl)
				{
					m_mcFace.gotoAndStop(strFrameFlag_Farmer);
				}
				else
				{
					m_mcFace.gotoAndStop(strFrameFlag_Land);
				}
			}

			if(m_mcFace)
			{
				if(m_listMotion == null)
				{
					m_listMotion = new LinkedList;
				}

				m_mcFace.addEventListener(Event.ENTER_FRAME, OnEventEnterFrame); 
				return true;
			}
			else
			{
				return false;
			}
		}
		public function destroy():void
		{
			if(m_mcFace)
			{
				m_mcFace.removeEventListener(Event.ENTER_FRAME, OnEventEnterFrame);
				m_mcFace = null;
			}
			if(m_listMotion)
			{
				m_listMotion.clear();
				m_listMotion = null;
			}
		}
		public function switchFaceIndex(nNewIndex:int):Boolean
		{
			if(m_nIndex == nNewIndex)
			{
				return true;
			}
			var nOldIndex:int = m_nIndex;	
			m_nIndex = nNewIndex;
			
			if(IsLandFace(nOldIndex) == false && 
			IsLandFace(nNewIndex) == true)
			{
				showMotion(strMotion_FarmerToLand, true);
			}
			else
			{
				showMotion(strMotion_Normal, true);
			}
			
			return true;
		}
		public function getMovieClip():MovieClip
		{
			return m_mcFace;
		}
		public function moveMovieClip(x:Number,y:Number):void
		{
			if(m_mcFace)
			{
				if(m_nDirect == nDirect_Left)
					m_mcFace.transform.matrix = new Matrix(1,0,0,1,x,y);
				else
					m_mcFace.transform.matrix = new Matrix(-1,0,0,1,x + getSize().x,y);
			}
		}
		
		public function showMotion(strMotionName:String,
						   bImmediately:Boolean):void
		{
			if(m_listMotion)
			{
				m_listMotion.append(String(strMotionName));
				if(bImmediately)
					PlayNextMotion();
			}
		}
		public function setOffLine(bOffLine:Boolean):void
		{
			if(bOffLine)
			{
				showMotion(strMotion_Offline, true);
			}
			else
			{
				showMotion(strMotion_Normal, true);
			}
		}
		/////////////////////////////////////////////////////
		public static function getSize():Point
		{
			return new Point(152, 143);
		}
		/////////////////////////////////////////////////////
		private function OnEventEnterFrame(e:Event):void
		{
			if(IsMotionEnd(m_nIndex, m_mcFace.currentLabel,
						   m_mcFace.currentFrame))
			{
				m_mcFace.stop();
				m_bPlaying = false;
				PlayNextMotion();
			}
		}
		private function IsMotionEnd(nIndex:int,
									 currentLabe:String,
									 currentFrame:int):Boolean
		{
			if(IsGirlFace(nIndex) == false)
			{
				
				if(currentLabe == strFrameFlag_Farmer)
				{
					if(currentFrame ==1)
						return true;
				}						
				else if(currentLabe == strFrameFlag_FBomb)
				{
					if(currentFrame ==32)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_FOffline)
				{
					if(currentFrame ==34)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_FTL)
				{
					if(currentFrame ==50)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_Land)
				{
					if(currentFrame ==52)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_LBomb)
				{
					if(currentFrame ==82)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_LOffline)
				{
					if(currentFrame ==84)
						return true;
						
				}

			}
			else
			{
				
				if(currentLabe == strFrameFlag_Farmer)
				{
					if(currentFrame ==1)
						return true;
						
				}						
				else if(currentLabe == strFrameFlag_FBomb)
				{
					if(currentFrame ==28)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_FOffline)
				{
					if(currentFrame ==30)
						return true;
					
				}
				else if(currentLabe == strFrameFlag_FTL)
				{
					if(currentFrame ==46)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_Land)
				{
					if(currentFrame ==48)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_LBomb)
				{
					if(currentFrame ==70)
						return true;
						
				}
				else if(currentLabe == strFrameFlag_LOffline)
				{
					if(currentFrame ==72)
						return true;
						
				}

			}
			return false;
		}
		private function IsLandFace(nIndex:int):Boolean
		{
			return ((nIndex & nIndex_FLMask) >> 4) ? true : false;
		}
		private function IsGirlFace(nIndex:int):Boolean
		{
			return (nIndex & nIndex_GenderMask) ? true : false;
		}
		private function PlayNextMotion():void
		{			
			if(m_listMotion.size())
			{
				if(m_bPlaying == false)
				{
					var strMotion:String = m_listMotion.shift();
					PlayMotion(strMotion);
					m_bPlaying = true;
				}
			}
			else
			{
				m_mcFace.stop();
				m_bPlaying = false;
			}
		}
		private function PlayMotion(strMotion:String):void
		{
			
				if(strMotion == strMotion_Normal)
				{
					if(IsLandFace(m_nIndex))
					{
						m_mcFace.gotoAndPlay(strFrameFlag_Land);
					}
					else
					{
						m_mcFace.gotoAndPlay(strFrameFlag_Farmer);
					}
					
				}
				else if(strMotion ==  strMotion_Bomb)
				{
					if(IsLandFace(m_nIndex))
					{
						m_mcFace.gotoAndPlay(strFrameFlag_LBomb);
					}
					else
					{
						m_mcFace.gotoAndPlay(strFrameFlag_FBomb);
					}
					
				}
				else if(strMotion ==  strMotion_FarmerToLand)
				{
					m_mcFace.gotoAndPlay(strFrameFlag_FTL);
					
				}
				else if(strMotion == strMotion_Offline)
				{
					if(IsLandFace(m_nIndex))
					{
						m_mcFace.gotoAndPlay(strFrameFlag_LOffline);
					}
					else
					{
						m_mcFace.gotoAndPlay(strFrameFlag_FOffline);
					}
					
				}
			
		}
	}
}