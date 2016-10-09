using System;

namespace CsharpPats
{
	/// <summary>
	/// Summary description for Rectangle.
	/// </summary>
	public class Rectangle
	{
		private int xp, yp, wr, hr;
		public Rectangle(int x, int y, int w, int h) {
			xp = x;
			yp = y;
			wr = w;
			hr = h;
		}
		public Rectangle(float x, float y, float w, float h) {
			xp = (int)x;
			yp = (int)y;
			wr = (int)w;
			hr = (int)h;
		}
		public bool contains(int x, int y) {
			bool cn = xp <= x && x <= xp + wr;
            cn = cn && yp <= y && y <= yp + hr;
            return cn;
		}
		public int x {
			get{
				return xp;
			}
			set {
				xp = value;
			}
		}
		public int y {
			get{
				return yp;
			}
			set {
				yp = value;
			}
		}
		public int w {
			get{
				return wr;
			}
			set {
				wr = value;
			}
		}
		public int h {
			get{
				return hr;
			}
			set {
				hr = value;
			}
		}
	}
}
