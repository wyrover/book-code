using System;
using System.Windows.Forms;
namespace CHolder
{
	/// <summary>
	/// Summary description for ExitCommand.
	/// </summary>
	public class ExitCommand : Command 
	{
		private Form form;
		public ExitCommand(Form frm)
		{
			form = frm;
		}
		public void Execute() {
			form.Close ();
		}
	}
}
