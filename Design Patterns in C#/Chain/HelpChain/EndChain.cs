using System;
using System.Windows.Forms;
namespace HelpChain
{
	/// <summary>
	/// Summary description for EndChain.
	/// </summary>
	public class EndChain:Chain 	{
		public EndChain(Control c, string mesg):base(c, mesg){}
		public override void sendToChain() {
			MessageBox.Show (message);
		}
	}
}
