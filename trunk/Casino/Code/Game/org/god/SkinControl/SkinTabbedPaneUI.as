package org.god.SkinControl
{
	import org.aswing.plaf.basic.BasicTabbedPaneUI;
	import org.aswing.plaf.basic.tabbedpane.Tab;
	import org.aswing.plaf.UIResource;
	import org.aswing.AbstractButton;
	import org.aswing.Component;
	import org.aswing.GroundDecorator;
	import org.aswing.Insets;
	import org.aswing.AsWingUtils;
	import org.aswing.JTabbedPane;
	import org.aswing.JButton;
	import org.aswing.graphics.Graphics2D;
	import org.aswing.geom.IntRectangle;
	import org.aswing.event.InteractiveEvent;
	
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	
	public class SkinTabbedPaneUI extends BasicTabbedPaneUI implements GroundDecorator,UIResource
	{

		protected var contentRoundImageContainer:Sprite;
		protected var top_contentRoundImage:DisplayObject;
		protected var bottom_contentRoundImage:DisplayObject;
		protected var left_contentRoundImage:DisplayObject;
		protected var right_contentRoundImage:DisplayObject;
		protected var contentRoundImageSetPlacement:int;

		protected var topTabCom:Component;

		public function SkinTabbedPaneUI()
		{
			super();
			tabBorderInsets=new Insets(0,4,0,4);
			contentRoundImageContainer=AsWingUtils.createSprite(null,"contentRoundImageContainer");
		}
		
		override protected  function installDefaults():void
		{
			super.installDefaults();

			top_contentRoundImage=getInstance(getPropertyPrefix() + "top.contentRoundImage")  as  DisplayObject;
			contentRoundImageContainer.addChild(top_contentRoundImage);
			contentRoundImageSetPlacement=JTabbedPane.TOP;

			bottom_contentRoundImage=getInstance(getPropertyPrefix() + "bottom.contentRoundImage")  as  DisplayObject;
			left_contentRoundImage=getInstance(getPropertyPrefix() + "left.contentRoundImage")  as  DisplayObject;
			right_contentRoundImage=getInstance(getPropertyPrefix() + "right.contentRoundImage")  as  DisplayObject;

			tabbedPane.setBackgroundDecorator(this);
		}
		override protected  function createNewTab():Tab
		{
			var tab:Tab=new SkinTabbedPaneTab  ;
			tab.getComponent().setFocusable(false);
			topTabCom=tab.getComponent();
			return tab;
		}
		override protected  function createNextButton():AbstractButton
		{
			var b:JButton=new JButton  ;
			b.setIcon(new SkinButtonIcon(-1,-1,getPropertyPrefix() + "arrowRight."));
			b.setBackgroundDecorator(null);
			return b;
		}
		override protected  function createPrevButton():AbstractButton
		{
			var b:JButton=new JButton  ;
			b.setIcon(new SkinButtonIcon(-1,-1,getPropertyPrefix() + "arrowLeft."));
			b.setBackgroundDecorator(null);
			return b;
		}

		public function updateDecorator(com:Component,g:Graphics2D,bounds:IntRectangle):void
		{
		}
		public function getDisplay(c:Component):DisplayObject
		{
			return contentRoundImageContainer;
		}
		override protected  function drawBaseLine(tabBarBounds:IntRectangle,g:Graphics2D,fullB:IntRectangle):void
		{
			var b:IntRectangle=tabBarBounds.clone();
			var placement:int=tabbedPane.getTabPlacement();

			var contentRoundImage:DisplayObject;
			if (placement == JTabbedPane.TOP)
			{
				contentRoundImage=top_contentRoundImage;
			}
			else if (placement == JTabbedPane.BOTTOM)
			{
				contentRoundImage=bottom_contentRoundImage;
			}
			else if (placement == JTabbedPane.LEFT)
			{
				contentRoundImage=left_contentRoundImage;
			}
			else
			{
				contentRoundImage=right_contentRoundImage;
			}
			//god
			if(contentRoundImage == null)
				return;
			if (contentRoundImage.parent != contentRoundImageContainer)
			{
				contentRoundImageContainer.removeChildAt(0);
				contentRoundImageContainer.addChild(contentRoundImage);
			}
			if (isTabHorizontalPlacing())
			{
				var isTop:Boolean=placement == JTabbedPane.TOP;
				if (isTop)
				{
					b.y=b.y + b.height - contentMargin.top;
				}
				b.height=contentMargin.top;
				b.width=fullB.width;
				b.x=fullB.x;
				if (isTop)
				{
					contentRoundImage.x=b.x;
					contentRoundImage.y=b.y;
					contentRoundImage.width=fullB.width;
					contentRoundImage.height=fullB.rightBottom().y - b.y;
				}
				else
				{
					contentRoundImage.x=fullB.x;
					contentRoundImage.y=fullB.y;
					contentRoundImage.width=fullB.width;
					contentRoundImage.height=b.y + b.height - fullB.y;
				}
			}
			else
			{
				var isLeft:Boolean=placement == JTabbedPane.LEFT;
				if (isLeft)
				{
					b.x=b.x + b.width - contentMargin.top;
				}
				b.width=contentMargin.top;
				b.height=fullB.height;
				b.y=fullB.y;
				if (isLeft)
				{
					contentRoundImage.x=b.x;
					contentRoundImage.y=b.y;
					contentRoundImage.width=fullB.rightTop().x - b.x;
					contentRoundImage.height=b.height;
				}
				else
				{
					contentRoundImage.x=fullB.x;
					contentRoundImage.y=fullB.y;
					contentRoundImage.width=b.x + b.width - fullB.x;
					contentRoundImage.height=b.height;
				}
			}
		}
		override protected  function drawTabBorderAt(index:int,b:IntRectangle,paneBounds:IntRectangle,g:Graphics2D):void
		{
			var placement:int=tabbedPane.getTabPlacement();
			if (index == tabbedPane.getSelectedIndex())
			{
				b=b.clone();//make a clone to be safty modification
				if (isTabHorizontalPlacing())
				{
					b.x-= tabBorderInsets.left;
					b.width+= tabBorderInsets.left + tabBorderInsets.right;
					b.height+= Math.round(topBlankSpace / 2 + contentRoundLineThickness);
					if (placement == JTabbedPane.BOTTOM)
					{
						b.y-= contentRoundLineThickness;
					}
					else
					{
						b.y-= Math.round(topBlankSpace / 2);
					}
				}
				else
				{
					b.y-= tabBorderInsets.left;
					b.height+= tabBorderInsets.left + tabBorderInsets.right;
					b.width+= Math.round(topBlankSpace / 2 + contentRoundLineThickness);
					if (placement == JTabbedPane.RIGHT)
					{
						b.x-= contentRoundLineThickness;
					}
					else
					{
						b.x-= Math.round(topBlankSpace / 2);
					}
				}
			}
			//for the gap
			if (placement == JTabbedPane.TOP || placement == JTabbedPane.BOTTOM)
			{
				b.width-= 1;
			}
			else
			{
				b.height-= 1;
			}
			//This is important, should call this in sub-implemented drawTabBorderAt method
			setDrawnTabBounds(index,b,paneBounds);
			var tab:SkinTabbedPaneTab=getTab(index)  as  SkinTabbedPaneTab;
			tab.setTabPlacement(placement);
		}
		
		override protected  function drawTabAt(index:int,bounds:IntRectangle,paneBounds:IntRectangle,g:Graphics2D,transformedTabMargin:Insets):void
		{
			//trace("drawTabAt : " + index + ", bounds : " + bounds + ", g : " + g);
			drawTabBorderAt(index,bounds,paneBounds,g);

			var viewRect:IntRectangle=transformedTabMargin.getInsideBounds(bounds);
			var tab:Tab=getTab(index);
			tab.setSelected(index == tabbedPane.getSelectedIndex());
			var tc:Component=tab.getComponent();
			tc.setComBounds(getDrawnTabBounds(index));
			if (index == tabbedPane.getSelectedIndex())
			{
				tc.parent.swapChildren(tc,topTabCom);
				topTabCom=tc;
			}
		}/**/
		override protected  function __onSelectionChanged(e:InteractiveEvent):void
		{
			tabbedPane.revalidate();
			tabbedPane.paintImmediately();
		}
	}
}