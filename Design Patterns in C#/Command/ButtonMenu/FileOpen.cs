using System;
using System.Windows.Forms;

namespace ButtonMenu
{
	/// <summary>
	/// Summary description for FileOpen.
	/// </summary>
	public class FileOpen : System.Windows.Forms.MenuItem , Command
	{
		public FileOpen():base("Open")
		{
			}
		public void Execute() {
			OpenFileDialog fd = new OpenFileDialog ();
			fd.ShowDialog ();
		}
	}
}
