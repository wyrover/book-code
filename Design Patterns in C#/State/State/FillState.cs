using System;

namespace State
{
	/// <summary>
	/// Summary description for FillState.
	/// </summary>
	public class FillState : State 	{
		public FillState(Mediator md): base(md)	{ }
		//-----
		public override void mouseDown(int x, int y) {
			//Fill drawing if you click inside one
			int i = med.findDrawing(x, y);
			if (i >= 0) {
				Drawing d = med.getDrawing(i);
				d.setFill(true);  //fill drawing
			 }
		}
		//-----
		public override void selectOne(Drawing d) {
			//fill drawing if selected
			d.setFill (true);
		}
	}
}
