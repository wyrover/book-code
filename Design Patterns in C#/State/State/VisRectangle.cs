using System;
using System.Drawing ;
using CsharpPats;
namespace State
{
	/// <summary>
	/// Summary description for VisRectangle.
	/// </summary>
	public class VisRectangle : Drawing 	{
		protected int x, y, w, h;
		private const int SIZE=30;
		private CsharpPats.Rectangle rect;
		protected bool selected;
		protected bool filled;
		protected Pen bPen;
		protected SolidBrush bBrush, rBrush;
		//-----
		public VisRectangle(int xp, int yp) 		{
			x = xp; 			y = yp;
			w = SIZE;			h = SIZE;
			saveAsRect();
			bPen = new Pen(Color.Black);
			bBrush = new SolidBrush(Color.Black);
			rBrush = new SolidBrush (Color.Red );
		}
		//-----
		//used by Memento for saving and restoring state
		 public CsharpPats.Rectangle getRects() {
			return rect;
		}
		//-----
		 public void setRects(CsharpPats.Rectangle value) {
				x=value.x;			y=value.y;
				w=value.w;			h=value.h;
				saveAsRect();
		}
		//------
		public void setSelected(bool b) {
			selected = b;
		}
		//-----
		//move to new position
		public void move(int xp, int yp) {
			x = xp;  		y = yp;
			saveAsRect();		
		}
		//-----
		public virtual void draw(Graphics g) { 
			//draw rectangle
			g.DrawRectangle(bPen, x, y, w, h);
			if(filled)
				g.FillRectangle (rBrush, x,y,w,h);
			drawHandles(g);	
		}
		//-----
		public void drawHandles(Graphics g) {
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
		protected void saveAsRect() {
			rect = new CsharpPats.Rectangle (x,y,w,h);
		}
		public void setFill(bool b) {
			filled = b;
		}
	}
}
