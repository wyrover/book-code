using System;

namespace State
{
	/// <summary>
	/// Summary description for CircleState.
	/// </summary>
	public class CircleState : State 	{
		public CircleState(Mediator md):base (md){ }
		//-----
		public override void mouseDown(int x, int y) {
			VisCircle c = new VisCircle(x, y);
			med.addDrawing (c);
		}
	}
}
