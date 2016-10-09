using System;
using System.Drawing;
namespace Template
{
	/// <summary>
	/// Summary description for IsocelesTriangle.
	/// </summary>
	public class IsocelesTriangle : Triangle 	{
		private Point newc;
		private int newcx, newcy;
		//-----
		public IsocelesTriangle(Point a, Point b, Point c) :base(a, b, c) {
			float dx1, dy1, dx2, dy2, side1, side2;
			float slope, intercept;
            int incr;
			dx1 = b.X - a.X;
			dy1 = b.Y - a.Y;
			dx2 = c.X  - b.X;
			dy2 = c.Y - b.Y;
        
			side1 = calcSide(dx1, dy1);
			side2 = calcSide(dx2, dy2);
        
			if (side2 < side1) 
				incr = -1;
			else
				incr = 1;
			slope = dy2 / dx2;
			intercept = c.Y - slope * c.X;
        
			//move point c so that this is an isoceles triangle
			newcx = c.X;
			newcy = c.Y;
			while (Math.Abs (side1 - side2) > 1) {
				newcx = newcx + incr;    //iterate a pixel at a time until close
				newcy = (int)(slope * newcx + intercept);
				dx2 = newcx - b.X;
				dy2 = newcy - b.Y;
				side2 = calcSide(dx2, dy2);
			}
			newc = new Point(newcx, newcy);
		}
		//-----
		private float calcSide(float a, float b) {
			return (float)Math.Sqrt (a*a +  b*b);
		}
		//-----
		public override Point draw2ndLine(Graphics g, Point b, Point  c) {
			g.DrawLine (pen, b, newc);
			return newc;
		}

	}
}
