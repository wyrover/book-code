using System;

namespace csPatterns {
		public class Rectangle 	{
		private int x1, x2, y1, y2;
		private int w, h;
		public Rectangle() 		{			}
		//-----
		public void init(int x, int y) {
			x1 = x;
			y1 = y;
			x2 = x1 + w;
			y2 = y1 + h;
		}
		//-----
		public void setSize(int w_, int h_) {
			w = w_;
			h = h_;
		}
		//-----
		public bool contains(int xp, int yp) {
			return (x1 <= xp) && (xp <= x2) && 
					(y1 <= yp) && (yp <= y2);
		}
	}
}
