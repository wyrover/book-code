using System;
using System.Windows.Forms;
namespace Mediate
{
	/// <summary>
	/// Receives all command actions from buttons and lists
	/// and sends commands to all the relevant controls
	/// </summary>
	public class Mediator 	{
		private CpyButton cpButton;		//buttons
		private ClrButton clrButton;
		private TextBox txKids;			//text box
		private ListBox pkList;			//list boxes
		private KidList klist;
		private KidData kds;			//list of data from file

		public Mediator(CpyButton cp, ClrButton clr, KidList kl, ListBox pk) 		{
			cpButton = cp;			//copy in buttons
			clrButton = clr;
			klist = kl;				//copy in list boxes
			pkList = pk;
			kds = new KidData ("50free.txt");	//create data list class
			clearClicked();				//clear all controls
			KidIterator kiter = kds.getIterator ();
			while(kiter.MoveNext () ) {		//load list box
				Kid kd = (Kid) kiter.Current ;
				klist.Items .Add (kd.getFrname() +" "+kd.getLname ());
			}
		}
		//-----
		//get text box reference
			public void setText(TextBox tx) {
				txKids = tx;
			}
		//-----
		//clear lists and set buttons to disabled
		public void clearClicked() {
			//disable buttons and clear list
			cpButton.Enabled = false;
			clrButton.Enabled = false;
			pkList.Items.Clear();
		}
		//-----
		//copy data from text box to list box
		public void copyClicked() {
			 //copy name to picked list
			pkList.Items.Add(txKids.Text);
			//clear button enabled
			clrButton.Enabled = true;
			klist.SelectedIndex = -1;
		}
		//-----
		//copy selected kid to text box
		//enable copy button
		public void kidPicked() {
			//copy text from list to textbox
			txKids.Text = klist.Text;
			//copy button enabled
			cpButton.Enabled = true;
		}
	}
}
