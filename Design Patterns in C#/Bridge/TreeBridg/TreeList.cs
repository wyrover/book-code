using System;
using System.Windows.Forms;
using System.Data;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for GridList.
	/// </summary>
	public class TreeList:VisList 	{
		private TreeView tree;
		private TreeAdapter gAdapter;
		//-----
		public TreeList(TreeView tre) {
			tree = tre;
			gAdapter = new TreeAdapter (tree);
		}
		//-----
		public void addLine(Product p) {
			gAdapter.Add (p);
		}
		//-----
		public void removeLine(int num) {
		}
	}
}
