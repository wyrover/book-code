using System;

namespace Flyweight
{
	/// <summary>
	/// Summary description for Positioner.
	/// </summary>
	public class Positioner 	{
		private const int pLeft = 30;
		private const int pTop  = 30;
		private const int HSpace = 70;
		private const int VSpace = 80;
		private const int rowMax = 2;
		private int x, y, cnt;
		//-----
		public Positioner()	{
			reset();
		}
		//-----
		public void reset() {
			x = pLeft;
			y = pTop;
			cnt = 0;
		}
		//-----
		public int nextX() {
			return x;
		}
		//-----
		public void incr() {
			cnt++;
			if (cnt > rowMax) {	//reset to start new row
				cnt = 0;
				x = pLeft;
				y += VSpace;
			}
			else {
				x += HSpace;
			}	
		}
		//-----
		public int nextY() {
			return y;
		}
	}
}
