using System;
using System.Drawing ;
namespace CsharpPats
{
	/// <summary>
	/// Summary description for Rectangle.
	/// </summary>
	public class Rectangle:Shape 	{
		public Rectangle(int x, int y,int h, int w):base(x,y,h,w) {}
		//-----
		public override void draw(Graphics g) {
			g.DrawRectangle (bPen, xpos, ypos, width, height);
		}
	}
}
