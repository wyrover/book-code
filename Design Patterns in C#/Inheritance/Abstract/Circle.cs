using System;
using System.Drawing ;
using CsharpPats;
namespace Abstract
{
	/// <summary>
	/// Summary description for Circle.
	/// </summary>
	public class Circle :Shape 	{
		public Circle(int x, int y, int r): 
			base(x,y,r,r)	{	}
		//-----
		public override void draw(Graphics g) {
			g.DrawEllipse (bPen, xpos, ypos, width, height);
		}
	}
}
