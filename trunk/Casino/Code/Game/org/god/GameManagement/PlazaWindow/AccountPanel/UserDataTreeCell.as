package 
{
	import org.aswing.tree.DefaultTreeCell;
	import org.aswing.Icon;
	import org.aswing.JTree;
	//用户数据树单元
	public class UserDataTreeCell extends DefaultTreeCell
	{
		public function UserDataTreeCell()
		{
			super();
		}
		override public function setTreeCellStatus(tree : JTree, selected : Boolean, expanded : Boolean, leaf : Boolean, row : int) : void
		{
			super.setTreeCellStatus(tree, selected, expanded, leaf, row);
			if (value != null)
			{
				if (value.getUserObject().getLessUserDataLoaded() == false)
				{
					if(value.getUserObject().getUserData().wLessUserCount != 0)
					{
						if (expanded)
						{
							setIcon(getExpandedFolderIcon());
						}
						else
						{
							setIcon(getCollapsedFolderIcon());
						}
					}
				}
			}
		}
		
	}
}