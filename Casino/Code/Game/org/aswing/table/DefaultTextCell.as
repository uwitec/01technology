/*
 Copyright aswing.org, see the LICENCE.txt.
*/

package org.aswing.table{
	
import org.aswing.Component;
import org.aswing.geom.*;
import org.aswing.JLabel;
import org.aswing.JTable;
import org.aswing.ASColor;
/**
 * Default table cell to render text
 * @author iiley
 */
public class DefaultTextCell extends JLabel implements TableCell{
	
	protected static const myColor:ASColor = new ASColor(0xfffab1);
	protected var value:*;
	protected var fixedBackgroundColor:ASColor;
	
	public function DefaultTextCell(){
		super();
		//god
		setHorizontalAlignment(CENTER);
		setOpaque(true);
	}
	
	/**
	 * Simpler this method to speed up performance
	 */
	override public function setComBounds(b:IntRectangle):void{
		readyToPaint = true;
		if(!b.equals(bounds)){
			if(b.width != bounds.width || b.height != bounds.height){
				repaint();
			}
			bounds.setRect(b);
			locate();
			valid = false;
		}
	}
	
	/**
	 * Simpler this method to speed up performance
	 */
	override public function invalidate():void {
		valid = false;
	}
	
	/**
	 * Simpler this method to speed up performance
	 */
	override public function revalidate():void {
		valid = false;
	}
	
	//**********************************************************
	//				  Implementing TableCell
	//**********************************************************
	public function setCellValue(value:*) : void {
		this.value = value;
		setText(value.toString());
	}
	
	public function getCellValue():*{
		return value;
	}
	public function setFixedBackgroundColor(c:ASColor):void
	{
		fixedBackgroundColor = c;
	
	}
	public function getMySelectionForeground(table:JTable):ASColor
	{
		return table.getSelectionForeground();
	}
	public function getMySelectionBackground(table:JTable):ASColor
	{
	  if(fixedBackgroundColor != null)
	  	return fixedBackgroundColor;
		return table.getSelectionBackground();
	}
	public function getMyForeground(table:JTable):ASColor
	{
		return table.getForeground();
	}
	public function getMyBackground(table:JTable):ASColor
	{
	 if(fixedBackgroundColor != null)
	  	return fixedBackgroundColor;
		return table.getBackground();
	}
	public function getMyAnotherBackground(table:JTable):ASColor
	{
		 if(fixedBackgroundColor != null)
	  	return fixedBackgroundColor;
		return myColor;
	}
	public function setTableCellStatus(table:JTable, isSelected:Boolean, row:int, column:int):void{
		if(isSelected){
		  setForeground(getMySelectionForeground(table));
			setBackground(getMySelectionBackground(table));
		}else{
			
			//god
			if(uint(row % 2) == 0)
			{
				setForeground(getMyForeground(table));
				setBackground(getMyBackground(table));
			}
			else
			{
				setForeground(getMyForeground(table));
				setBackground(getMyAnotherBackground(table));
			}
		}
		setFont(table.getFont());
	}
	
	public function getCellComponent() : Component {
		return this;
	}
	
	override public function toString():String{
		return "TextCell[label:" + super.toString() + "]\n";
	}
}
}