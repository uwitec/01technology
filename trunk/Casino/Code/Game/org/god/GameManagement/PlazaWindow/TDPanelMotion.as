package 
{
	import flash.display.Sprite;
	import flash.errors.MemoryError;
	import flash.events.Event;
	
	import org.god.Common.TDRenderer;
	
	import org.papervision3d.scenes.*;
	import org.papervision3d.cameras.*;
	import org.papervision3d.objects.*;
	import org.papervision3d.materials.*;
	//3D转动
	public class TDPanelMotion extends TDRenderer
	{
		protected var m_bMotioning:Boolean;//转动标记
		protected var m_planeNow:Plane;//现在平面
		protected var m_planeNext:Plane;//下一平面
		protected var m_nIndex0:uint;//索引0
		protected var m_nIndex1:uint;//索引1
		protected var m_bDirectBack:Boolean;//后退标记
		protected var m_nFrameCount:uint;//帧率计数
		protected var m_nMaxFrameCount:uint;//最大帧率
		protected var m_nWidth:Number;//宽度
		protected var m_nHeight:Number;//高度
		
		public function TDPanelMotion(container :Sprite,nwidth:Number,nheight:Number)
		{
			m_bMotioning = false;
			m_nWidth = nwidth;
			m_nHeight = nheight;
			if(Init(container) == false)
				throw MemoryError("3DPanelMotion Init Faild");
		}
		//销毁
		public function Destroy():void
		{
			Uninit();
		}
		//判断正在转动
		public function IsMotioning():Boolean
		{
			return m_bMotioning;
		}
		//转动
		public function Motion_SwitchTab(now_sprite:Sprite, 
										 index0:uint,
										 next_sprite:Sprite, 
										 index1:uint,
										 bDirectBack:Boolean = false,
										 nFrameCount:uint = 6):void
		{
			if(IsMotioning())
				return;
			
			m_bMotioning = true;
			m_nIndex0	  = index0;
			m_nIndex1	  = index1;
			m_bDirectBack = bDirectBack;
			m_nFrameCount = nFrameCount;
			m_nMaxFrameCount = nFrameCount;
			m_planeNow 	= CreatePlane(now_sprite);
			m_planeNext = CreatePlane(next_sprite);
			m_scene.addChild(m_planeNow);
			m_scene.addChild(m_planeNext);
			m_container.x += m_nWidth / 2;
			m_container.y += m_nHeight / 2;
			if(m_bDirectBack)
			{
				MovePlane(m_planeNext,1,true,m_nWidth);
			}
			else
			{
				MovePlane(m_planeNext,1,false,m_nWidth);
			}
			MovePlane(m_planeNow,0,false,m_nWidth);
		}
		//渲染3D
		override protected function loop3D(e:Event):void
		{
			if(m_planeNow && m_planeNext && m_bMotioning)
			{
				var p:Number = Number(m_nMaxFrameCount - m_nFrameCount) / Number(m_nMaxFrameCount);
				if(m_bDirectBack)
				{
					MovePlane(m_planeNow,p,true,m_nWidth);
					MovePlane(m_planeNext,1.0 - p,false,m_nWidth);
				}
				else
				{
					MovePlane(m_planeNow,p,false,m_nWidth);
					MovePlane(m_planeNext,1.0 - p,true,m_nWidth);
				}
				m_nFrameCount --;
				
			}
			m_camera.z = -m_nWidth/2;
			
			m_scene.renderCamera( m_camera );
			
			if(m_nFrameCount <= 0 && m_bMotioning)
			{
				m_scene.removeChild(m_planeNow);
				m_scene.removeChild(m_planeNext);
				DestroyPlane(m_planeNow);
				DestroyPlane(m_planeNext);
				m_planeNow = null;
				m_planeNext= null;
				m_scene.renderCamera( m_camera );
				m_container.x -= m_nWidth / 2;
				m_container.y -= m_nHeight / 2;		
				dispatchEvent(new MotionCompleteEvent(m_nIndex0, m_nIndex1));
				m_bMotioning = false;
			}
		}
		//移动平面
		private function MovePlane(plane:Plane, p:Number, back:Boolean, w:Number):void
		{
			var r:Number = p * 65.0;
			var d:Number = w /2 - w /2 * Math.cos(r * Math.PI/180.0);
			if(back)
			{
				plane.x = p * w - d;
				plane.z = p * w/2.0;
				plane.rotationY = - r;
			}
			else
			{
				plane.x = -p * w + d;
				plane.z = p * w/2.0;
				plane.rotationY = r;
			}
		}
	}
}