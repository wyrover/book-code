using System;
using System.Drawing ;
namespace CsharpPats
{
	/// <summary>
	/// Draws a rectangle using a supplied Graphics object
	/// </summary>
	public class Rectangle	{
		private int x, y, w, h;
		protected Pen rpen;
		public Rectangle(int x_, int y_, int w_, int h_) 		{
			x = x_;
			y = y_;
			w = w_;
			h = h_;
			rpen = new Pen(Color.Black);
		}
		//-----------------
		public void draw(Graphics g) {
		  g.DrawRectangle (rpen, x, y, w, h);
		}
	}
}
