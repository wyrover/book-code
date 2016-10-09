using System;
using System.Collections ;
using System.Windows.Forms ;
using System.Drawing ;


//returns a panel of 0 to 3 check boxes
	public class CheckChoice:MultiChoice 	{
		private ArrayList stocks;
		private Panel panel;
		private ArrayList boxes;
	//------
		public CheckChoice(Equities stks) 		{
		stocks = stks.getNames ();
			panel = new Panel ();
			boxes = new ArrayList ();
			//add the check boxes to the panel
			for (int i=0; i< stocks.Count; i++) {
				CheckBox ck = new CheckBox ();
				//position them
				ck.Location = new Point (8, 16 + i * 32);
				string stk = (string)stocks[i];
				ck.Text =stk;
				ck.Size = new Size (112, 24);
				ck.TabIndex =0;
				ck.TextAlign = ContentAlignment.MiddleLeft ;
				boxes.Add (ck);
				panel.Controls.Add (ck);
			}
		}
		//------
		//uncheck all check boxes
		public  void clear() {
			for(int i=0; i< boxes.Count; i++) {
			  CheckBox ck = (CheckBox)boxes[i];
				ck.Checked =false;
			}
		}
		//------
		//return list of checked items
		public ArrayList getSelected() {
			ArrayList sels = new ArrayList ();
			for(int i=0; i< boxes.Count ; i++) {
				CheckBox ck = (CheckBox)boxes[i];
				if (ck.Checked ) {
					sels.Add (ck.Text );
				}
			}
			return sels;
		}
		//------
		//return panel of checkboxes
		public Panel getWindow() {
			return panel;
		}
	}
