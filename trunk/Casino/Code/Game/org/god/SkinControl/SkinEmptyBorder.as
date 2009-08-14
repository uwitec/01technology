package org.god.SkinControl
{

	import org.aswing.border.EmptyBorder;
	import org.aswing.plaf.UIResource;
	import org.aswing.Insets;

	public class SkinEmptyBorder extends EmptyBorder implements UIResource
	{
		public function SkinEmptyBorder(top:int=0, left:int=0, bottom:int=0, right:int=0)
		{
			super(null, new Insets(top, left, bottom, right));
		}
	}
}