using System;

namespace State
{
	/// <summary>
	/// Summary description for ArrowState.
	/// </summary>
	public class ArrowState :State
	{
		public ArrowState(Mediator md) :base(md)	{}
		public override void mouseDown(int x, int y) {
			 med.unpick();
			med.startArrow();
			int i = med.findDrawing(x, y);
			if (i >= 0) {
				med.setSelected(i);
				med.rememberPosition();
			}
		}
		//-----
		public override void mouseDrag(int x, int y  ) {
			int i = med.findDrawing(x, y);
			if (i >= 0){
				Drawing d = med.getDrawing(i);
				d.move(x, y);
			}
		}
	}
}
