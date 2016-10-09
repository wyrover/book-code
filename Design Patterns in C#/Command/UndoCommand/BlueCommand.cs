using System;
using System.Windows.Forms;
using System.Drawing;
using System.Collections ;

namespace UndoCommand
{
	/// <summary>
	/// draws bluelines and caches list for undo
	/// </summary>
	public class BlueCommand :Command 	{
		protected Color color;
		private PictureBox pbox;
		private ArrayList drawList;
		protected int x, y, dx, dy;
	//-----
		public BlueCommand(PictureBox pbx) {
			pbox = pbx;
			color = Color.Blue ;
			drawList = new ArrayList ();
			x = pbox.Width ;
			dx = -20;
			y = 0;
			dy = 0;
		}
		//-----
		public void Execute() {
		      DrawData dl = new DrawData(x, y, dx, dy);
			drawList.Add(dl);
			x = x + dx;
			y = y + dy;
			pbox.Refresh();
		}
		//-----
		public bool isUndo() {
			return false;
		}
		//-----
		public void Undo() {
			DrawData dl;
			int index = drawList.Count - 1;
			if (index >= 0) {
				dl = (DrawData)drawList[index];
				drawList.RemoveAt(index);
				x = dl.getX();
				y = dl.getY();
			}
			pbox.Refresh();
		}
		//-----
		public void draw(Graphics g) {
			Pen rpen = new Pen(color, 1);
			int h = pbox.Height;
			int w = pbox.Width;

			for (int i = 0; i < drawList.Count ; i++) {
				DrawData dl = (DrawData)drawList[i];
				g.DrawLine(rpen, dl.getX(), dl.getY(), dl.getX() + dx, dl.getDy() + h);
			}
		}
	}
}
