using System;
using System.Windows.Forms;
namespace State
{
	/// <summary>
	/// Summary description for PickButton.
	/// </summary>
	public class PickButton :ComdToolBarButton
	{
		public PickButton(Mediator md) : base("Select", md)
		{
			this.Style = ToolBarButtonStyle.ToggleButton ;
			med.registerArrowButton (this);
		}
		public override void Execute() {
			med.startArrow ();
		}
	}
}
