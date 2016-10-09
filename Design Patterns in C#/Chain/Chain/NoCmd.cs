using System;
using System.Windows.Forms;
namespace Chain
{
	/// <summary>
	/// handles command that is not otherwise legal
	/// </summary>
	public class NoCmd :Chain 	{
		private ListBox lsNocmd;	//commands go here
		//-----
		public NoCmd(ListBox lb) 		{
			lsNocmd = lb;			//copy reference
		}
		//-----
		public override void sendToChain(string mesg) {
			//adds unknown commands to list box
			lsNocmd.Items.Add (mesg);
		}
	}
}
