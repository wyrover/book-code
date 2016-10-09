using System;
using System.Collections ;
using System.Windows.Forms;


	/// <summary>
	/// Summary description for MultiChoice.
	/// </summary>
	public interface MultiChoice
	{
		ArrayList getSelected();
		void clear();
		Panel getWindow();
	}

