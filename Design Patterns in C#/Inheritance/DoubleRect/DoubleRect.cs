using System;
using System.Drawing ;

namespace RectDraw
{
	/// <summary>
	/// Summary description for DoubleRect.
	/// </summary>
	public class DoubleRect:Rectangle
	{
		private Pen rdPen;
		public DoubleRect(int x, int y, int w, int h):base(x,y,w,h)
		{
			rdPen  = new Pen (Color.Red, 2);
		}
		//-----------------
		public override void draw(Graphics g) {
			base.draw (g); //g.DrawRectangle (rpen, x, y, w, h);
			g.DrawRectangle (rdPen, x +5, y+5, w, h);
		}
	}
}
