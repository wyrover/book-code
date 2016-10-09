using System;
using System.Collections ;
using System.Windows.Forms ;
using System.Drawing ;

	/// <summary>
	/// Summary description for ListChoice.
	/// </summary>
	/// creates a Panel containing a list box
	public class ListChoice:MultiChoice {
		private ArrayList stocks;
		private Panel panel;
		private ListBox list;
	//------
		//constructor creates and loads the list box
		public ListChoice(Equities stks) {
			stocks = stks.getNames ();
			panel = new Panel ();
			list = new ListBox ();
			list.Location = new Point (16, 0);
			list.Size = new Size (120, 160);
			list.SelectionMode =SelectionMode.MultiExtended ;
			list.TabIndex =0;
			panel.Controls.Add (list);
			for(int i=0; i< stocks.Count ; i++) {
				list.Items.Add (stocks[i]);
			}
		}
		//returns the Panel
		//------
		public  Panel getWindow() {
			return panel;
		}
		//returns an array of selected elements
		//------
		public ArrayList getSelected() {
			ArrayList sels = new ArrayList ();
			ListBox.SelectedObjectCollection  coll = list.SelectedItems  ;
			for(int i=0; i< coll.Count; i++) {
				string item = (string)coll[i];
					sels.Add (item );
				}
		return sels;	
		}
		//------	
		//clear selected elements
		public void clear() { 
			list.Items.Clear();
		}
	}
