package org.god.Common
{
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	
	import org.papervision3d.scenes.*;
	import org.papervision3d.cameras.*;
	import org.papervision3d.objects.*;
	import org.papervision3d.materials.*;
	//3D渲染
	public class TDRenderer extends EventDispatcher
	{
		protected var m_container :Sprite;
		protected var m_scene     :Scene3D;
		protected var m_camera    :Camera3D;
		//构造函数
		public function TDRenderer()
		{
		}
		//初始化
		public function Init(container :Sprite):Boolean
		{
			m_container = container;
			m_scene = new Scene3D( m_container );
			m_camera = new Camera3D();
			m_camera.zoom = 6.11;
			m_container.addEventListener( Event.ENTER_FRAME, loop3D );
			
			return true;
		}
		//销毁
		public function Uninit():void
		{
			m_container.removeEventListener( Event.ENTER_FRAME, loop3D );
			m_container = null;
			m_scene 	= null;
			m_camera	= null;
		}
		//创建面
		protected function CreatePlane(sprite:Sprite):Plane
		{
			var materialSpace :SpriteMaterial = new SpriteMaterial( sprite );
			var plane:Plane = new Plane( materialSpace, 
												  sprite.width,
												  sprite.height,
												  8, 8 );
			
			return plane;
		}
		//销毁
		protected function DestroyPlane(plane:Plane):void
		{
			if(plane && plane.material)
			{
				var materialSpace :SpriteMaterial = plane.material as SpriteMaterial;
				materialSpace.destroyBitmap();
			}
		}
		protected function loop3D(e:Event):void
		{
			m_scene.renderCamera( m_camera );
		}
	}
}