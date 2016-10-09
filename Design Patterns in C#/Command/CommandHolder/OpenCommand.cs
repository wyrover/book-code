using System;
using System.Windows.Forms ;
namespace CHolder
{
	/// <summary>
	/// Summary description for OpenCommand.
	/// </summary>
	public class OpenCommand :Command 	{
		public OpenCommand()
		{}
		public void Execute() {
			OpenFileDialog fd = new OpenFileDialog ();
			fd.ShowDialog ();
		}
	}
}
