using System;
using System.Drawing ;
using CsharpPats;
namespace Memento
{
	/// <summary>
	/// Summary description for VisRectangle.
	/// </summary>
	public class VisRectangle 	{
		private int x, y, w, h;
		private const int SIZE=30;
		private CsharpPats.Rectangle rect;
		private bool selected;
		private Pen bPen;
		private SolidBrush bBrush;
		//-----
		public VisRectangle(int xp, int yp) 		{
			x = xp; 			y = yp;
			w = SIZE;			h = SIZE;
			saveAsRect();
			bPen = new Pen(Color.Black);
			bBrush = new SolidBrush(Color.Black);
		}
		//-----
		//used by Memento for saving and restoring state
		internal CsharpPats.Rectangle rects {
			get {
				return rect;
			}
			set {
				x=value.x;
				y=value.y;
				w=value.w;
				h=value.h;
				saveAsRect();
			}
		}
		//------
		public void setSelected(bool b) {
			selected = b;
		}
		//-----
		//move to new position
		public void move(int xp, int yp) {
			x = xp; 
			y = yp;
			saveAsRect();		
		}
		//-----
		public void draw(Graphics g) { 
			//draw rectangle
			g.DrawRectangle(bPen, x, y, w, h);
			
			if (selected) {   //draw handles
				g.FillRectangle(bBrush, x + w / 2, y - 2, 4, 4);
				g.FillRectangle(bBrush, x - 2, y + h / 2, 4, 4);
				g.FillRectangle(bBrush, x + (w / 2), y + h - 2, 4, 4);
				g.FillRectangle(bBrush, x + (w - 2), y + (h / 2), 4, 4);
			}
		}
		//-----
		//return whether point is inside rectangle
		public bool contains(int x, int y) {
			return rect.contains (x, y);
		}
		//------
		//create Rectangle object from new position
		private void saveAsRect() {
			rect = new CsharpPats.Rectangle (x,y,w,h);
		}
	}
}
