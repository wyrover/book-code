using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace Memento
{
	/// <summary>
	/// Summary description for UndoButton.
	/// </summary>
	public class UndoButton :  Command
	{
		private ToolBarButton ubutton;
		private Mediator med;

		public UndoButton(Mediator md, ToolBarButton but) 	{
			med = md;
			ubutton = but;
		}
		public void Execute() {
			med.undo ();
		}
		

	}
}
