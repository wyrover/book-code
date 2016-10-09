using System;
using System.Windows.Forms;
namespace TreeAdapt
{
	/// <summary>
	/// Summary description for TreeAdapter.
	/// </summary>
	public class TreeAdapter:LstAdapter 	{
		private TreeView tree;
		//------
		public TreeAdapter(TreeView tr) 		{
			tree=tr;
		}
		//------
		public void Add(Swimmer sw) {
			TreeNode nod;
        //add a root node  
			nod = tree.Nodes.Add(sw.getName());
        //add a child node to it
        nod.Nodes.Add(sw.getTime().ToString ());
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
