using System;
using System.Drawing ;

namespace Template
{
	/// <summary>
	/// Summary description for Triangle.
	/// </summary>
	public abstract class Triangle 	{
		private Point p1, p2, p3;
		protected Pen pen;
		//-----
		public Triangle(Point a, Point b, Point c) 	{
			p1 = a;
			p2 =  b;
			p3 = c;
			pen = new Pen(Color.Black , 1);
		}
		//-----
		public virtual void draw(Graphics g) {
			g.DrawLine (pen, p1, p2);
			Point c = draw2ndLine(g, p2, p3);
			closeTriangle(g, c);
		}
		//-----
		public abstract Point draw2ndLine(Graphics g, Point a, Point b);
		//-----
		public void closeTriangle(Graphics g, Point c) {
			g.DrawLine (pen, c, p1);
		}
	}
}
