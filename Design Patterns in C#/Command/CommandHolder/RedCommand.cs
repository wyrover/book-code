using System;
using System.Windows.Forms;
using System.Drawing ;
namespace CHolder
{
	/// <summary>
	/// Summary description for cmdRed.
	/// </summary>
	public class RedCommand : Command 	{
		private Control window;
		//-----
		public RedCommand(Control win) 		{
			window = win;
		}
		//-----
		void Command.Execute () {
			window.BackColor =Color.Red ;
		}
	}
}
