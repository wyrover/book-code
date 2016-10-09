using System;
using System.Windows.Forms;
namespace HelpChain
{
	/// <summary>
	/// Summary description for ControlChain.
	/// </summary>
	public class ControlChain:Chain 	{
		public ControlChain(Control c, string mesg):base(c, mesg)		{
		}
		public override void sendToChain() {
			//if it has the focus display the message
			if (control.Focused ) {
				MessageBox.Show (message);
			}
			else
				//otherweise pass on down the chain
				sendChain();
		}
	}
}
