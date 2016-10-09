using System;
using System.Windows.Forms;
namespace State
{
	/// <summary>
	/// Summary description for RectButton.
	/// </summary>
	public class RectButton :ComdToolBarButton
	{
		public RectButton(Mediator md) :base("Rectangle", md) 	{
			this.Style =ToolBarButtonStyle.ToggleButton ;
			med.registerRectButton (this);
		}
		public override void Execute() {
			med.startRectangle ();
		}
	}
}
