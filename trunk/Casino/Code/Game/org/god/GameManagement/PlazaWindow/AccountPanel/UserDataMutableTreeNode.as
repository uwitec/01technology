package 
{
	import org.aswing.tree.DefaultMutableTreeNode;
	//帐号树节点
	public class UserDataMutableTreeNode extends DefaultMutableTreeNode
	{
		public function UserDataMutableTreeNode(userObject:Object = null)
		{
			super(userObject);
		}
		override public function toString():String
		{
			if (userObject == null)
			{
				return null;
			}
			else
			{
				var node:UserDataTreeNodeData = userObject as UserDataTreeNodeData;
				return node.getAccount();
			}
		}
	}
}