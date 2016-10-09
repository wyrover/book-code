using System;
using System.Drawing ;
namespace CsharpPats
{
	/// <summary>
	/// Summary description for Shape.
	/// </summary>
	public abstract class Shape 	{
		protected int height, width;
		protected int xpos, ypos;
		protected Pen bPen;
		//-----
		public Shape(int x, int y, int h, int w)	{
			width = w;
			height = h;
			xpos = x;
			ypos = y;
			bPen = new Pen(Color.Black );
		}
		//-----
		public abstract void draw(Graphics g);
		//-----
		public virtual float getArea() {
			return height * width;
		}
	}
}
