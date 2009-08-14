/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.plaf.basic.frame{
	
import org.aswing.Component;
import org.aswing.Container;
import org.aswing.FlowLayout;
import org.aswing.geom.IntDimension;
import org.aswing.Insets;
import org.aswing.LayoutManager;
import org.aswing.plaf.UIResource;

/**
 * @private
 */
public class TitleBarLayout extends FlowLayout implements UIResource{
	
	protected static const ICON_TITLE_WIDTH:int = 50;
	protected static const ICON_TITLE_HEIGHT:int = 20;
	
	
	public function TitleBarLayout() {
		super(FlowLayout.RIGHT, 4, 4);
	}
	
	public function getHorizontalGap():int{
		return getHgap();
	}
	
	protected function fitSize(target:Container, size:IntDimension):IntDimension{
    	size.change(ICON_TITLE_WIDTH, 0);
    	size.height = Math.max(size.height, ICON_TITLE_HEIGHT);
    	return size;
	}
    
	override public function preferredLayoutSize(target:Container):IntDimension {
		var dim:IntDimension = new IntDimension(0, 0);
		var nmembers:int = target.getComponentCount();

		var counted:int = 0;
		for (var i:int = 0 ; i < nmembers ; i++) {
	    	var m:Component = target.getComponent(i);
			var d:IntDimension = m.getPreferredSize();
			dim.height = Math.max(dim.height, d.height);
	    	if (m.isVisible()) {
                if (counted > 0) {
                    dim.width += getHgap();
                }
				dim.width += d.width;
				counted ++;
	    	}
		}
		var insets:Insets = target.getInsets();
		dim.width += insets.left + insets.right + getHgap()*2;
		dim.height += insets.top + insets.bottom + getVgap()*2;
		return fitSize(target, dim);
    }

    override public function minimumLayoutSize(target:Container):IntDimension {
		return preferredLayoutSize(target);
    }

}
}