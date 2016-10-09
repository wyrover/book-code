using System;
using System.Drawing ;
namespace State
{
	/// <summary>
	/// Summary description for VisCircle.
	/// </summary>
	public class VisCircle : VisRectangle 	{
		private int r;
		public VisCircle(int x, int y):base(x, y) 		{
			r = 15; w = 30; h = 30;
			saveAsRect();
		}
		//-----
		public override void draw(Graphics g) {
			if (filled) {
				g.FillEllipse(rBrush, x, y, w, h);
			}
        g.DrawEllipse(bPen, x, y, w, h);
        if (selected ){
			drawHandles(g);
			}
	    }
	}
}
