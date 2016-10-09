using System;
using System.Drawing ;
namespace Flyweight
{
	/// <summary>
	/// Summary description for Folder.
	/// </summary>
	public class Folder 	{
		//Draws a folder at the specified coordinates
		private const int w  = 50;
		private const int h = 30;
		private Pen blackPen, whitePen;
	    private Pen grayPen;																		
		private SolidBrush backBrush, blackBrush;
		private Font fnt;
		//------
		public Folder(Color col) 		{
			backBrush = new SolidBrush(col);
			blackBrush = new SolidBrush(Color.Black);
			blackPen = new Pen(Color.Black);
			whitePen = new Pen(Color.White);
			grayPen = new Pen(Color.Gray);
			fnt = new Font("Arial", 12);
		}
		//-----
		public void draw(Graphics g, int x, int y, string title) {
			g.FillRectangle(backBrush, x, y, w, h);
			g.DrawRectangle(blackPen, x, y, w, h);
			g.DrawLine(whitePen, x + 1, y + 1, x + w - 1, y + 1);
			g.DrawLine(whitePen, x + 1, y, x + 1, y + h);
	
		    g.DrawRectangle(blackPen, x + 5, y - 5, 15, 5);
			g.FillRectangle(backBrush, x + 6, y - 4, 13, 6);

			g.DrawLine(grayPen, x, y + h - 1, x + w, y + h - 1);
			g.DrawLine(grayPen, x + w - 1, y, x + w - 1, y + h - 1);
			g.DrawString(title, fnt, blackBrush, x, y + h + 5);
		}
	}
}
