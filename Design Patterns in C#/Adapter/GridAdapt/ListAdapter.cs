using System;
using System.Windows.Forms;

namespace GridAdapt
{
	/// <summary>
	/// Summary description for ListAdapter.
	/// </summary>
	public class ListAdapter:LstAdapter
	{
		private ListBox listbox;
		public ListAdapter(ListBox lb) 		{
			listbox = lb;
		}
		public void Add(string s) {
			listbox.Items.Add (s);
		}
        public void Add(Swimmer sw) {
			listbox.Items.Add (sw.getName()+"\t"+sw.getTime());
		}
		public int SelectedIndex() {
			return listbox.SelectedIndex;
		}
		public void Clear() {
			listbox.Items.Clear ();
		}
		public void clearSelection() {
			int i = SelectedIndex();
			if(i >= 0) {
				listbox.SelectedIndex =-1;
			}
		}
	}
}
