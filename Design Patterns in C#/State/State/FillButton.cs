using System;
using System.Windows.Forms;
namespace State
{
	/// <summary>
	/// Summary description for FillButton.
	/// </summary>
	public class FillButton :ComdToolBarButton 	{
		public FillButton(Mediator md):base ("Fill", md) 		{
			this.Style = ToolBarButtonStyle.ToggleButton ;
			med.registerFillButton(this);
		}
		public override void Execute() {
			med.startFill();
		}
	}
}
