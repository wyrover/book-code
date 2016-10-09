using System;

namespace UndoCommand
{
	/// <summary>
	/// Summary description for DrawData.
	/// </summary>
	public class DrawData
	{
		private int x, y, dx, dy;
		public DrawData(int x_, int y_, int dx_, int dy_) 		{
			x = x_;
			dx = dx_;
			y = y_;
			dy = dy_;
		}
		public int getX() {
			return x;
		}
		public int getY() {
			return y;
		}
		public int getDx() {
			return dx;
		}
		public int getDy() {
			return dy;
		}
	}
}
