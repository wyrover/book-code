using System;
using System.Drawing ;
namespace Template
{
	/// <summary>
	/// Summary description for StdTriangle.
	/// </summary>
	public class StdTriangle :Triangle 	{
		public StdTriangle(Point a, Point b, Point c) : base(a, b, c) {}
		//------
		public override Point draw2ndLine(Graphics g, Point a, Point b) {
			g.DrawLine (pen, a, b);
			return b;
		}
	}
}
