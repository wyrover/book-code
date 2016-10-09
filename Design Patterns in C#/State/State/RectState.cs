using System;

namespace State
{
	/// <summary>
	/// Summary description for RectState.
	/// </summary>
	public class RectState :State 	{
		public RectState(Mediator md):base (md) {}
		//-----
		public override void mouseDown(int x, int y) {
			VisRectangle vr = new VisRectangle(x, y);
			med.addDrawing (vr);
		}
	}
}
