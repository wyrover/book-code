using System;
using System.Windows.Forms;
namespace State
{
	/// <summary>
	/// Summary description for CircleButton.
	/// </summary>
	public class CircleButton:ComdToolBarButton
	{
		public CircleButton(Mediator md):base("Circle", md)
		{
			this.Style =ToolBarButtonStyle.ToggleButton ;
			med.registerCircleButton (this);
		}
		public override void Execute() {
			med.startCircle ();
		}
	}
}
