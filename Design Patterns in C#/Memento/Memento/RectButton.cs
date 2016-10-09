using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace Memento
{
	/// <summary>
	/// Summary description for RectButton.
	/// </summary>
	public class RectButton :  Command 	{
		private ToolBarButton bt;
		private Mediator med;
		//------
		public RectButton(Mediator md, ToolBarButton tb) 	{
			med = md;
			bt = tb;
		}
		//------
		public void setSelected(bool sel) {
			bt.Pushed  = sel;
		}
		//------
		public void Execute() {
			if(bt.Pushed  )
				med.startRectangle ();
		}
	}
}
