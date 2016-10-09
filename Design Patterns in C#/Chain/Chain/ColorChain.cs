using System;
using System.Collections ;
using System.Drawing ;
using System.Windows.Forms ;
namespace Chain
{
	/// <summary>
	/// receives color names in chain
	/// </summary>
	public class ColorChain : Chain {
		private Hashtable colHash;	//color list kept here
		private Panel panel;		//color goes here
		//-----
		public ColorChain(Panel pnl) 		{
			panel = pnl;			//save reference
			//create Hash table to correlate color names
			//with actual Color objects
			colHash = new Hashtable ();
			colHash.Add ("red", Color.Red);
			colHash.Add ("green", Color.Green);
			colHash.Add ("blue", Color.Blue);
		}
		//-----
		public override void sendToChain(string mesg) {
			mesg = mesg.ToLower ();
			try {
				Color c = (Color)colHash[mesg];
				//if this is a color, put it in the panel
				panel.BackColor =c;
			}
			catch (NullReferenceException e) {
				//send on if this doesn't work
				sendChain(mesg);
			}
			
		}
	}
}
