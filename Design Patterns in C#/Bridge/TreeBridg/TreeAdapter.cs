using System;
using System.Windows.Forms;
namespace BasicBridge
{
	/// <summary>
	/// Summary description for TreeAdapter.
	/// </summary>
	public class TreeAdapter 	{
		private TreeView tree;
		//------
		public TreeAdapter(TreeView tr) 		{
			tree=tr;
		}
		//------
		public void Add(Product p) {
			TreeNode nod;
        //add a root node  
			nod = tree.Nodes.Add(p.getName());
        //add a child node to it
        nod.Nodes.Add(p.getQuantity ());
        tree.ExpandAll ();
		}
		//------
		public int SelectedIndex() {
            return tree.SelectedNode.Index ;
		}
		//------
		public void Clear() {
		TreeNode nod;
			for (int i=0; i< tree.Nodes.Count ; i++) {
				nod = tree.Nodes [i];
				nod.Remove ();
			}
		}
		//------
		public void clearSelection() {}
	}
}
