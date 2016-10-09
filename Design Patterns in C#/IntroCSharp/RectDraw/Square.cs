using System;
using CsharpPats;

namespace RectDraw
{
	/// <summary>
	/// Draws a square using the parent Rectangl class
	/// </summary>
	public class Square : Rectangle
	{
		public Square(int x, int y, int w):	base(x, y, w, w) {
		}
	}
}
