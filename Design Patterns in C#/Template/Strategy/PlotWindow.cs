using System;
using System.Windows.Forms;
using System.Drawing ;
using System.Collections ;
namespace Strategy
{
	/// <summary>
	/// Summary description for PlotWindow.
	/// </summary>
	public abstract class PlotWindow : Form {
		protected float ymin, ymax, xfactor, yfactor;
		protected float xpmin, xpmax, ypmin, ypmax, xp, yp;
		private float xmin, xmax;
		protected int w, h;
		protected float[] x, y;
		protected Pen bPen;
		protected bool hasData;
		protected const float max = 1.0e38f;
		protected PictureBox pic;
		//-----
		protected virtual void init() {
			pic.Paint += new PaintEventHandler (pic_Paint);
		}
		//-----
		public void setPenColor(Color c){
			bPen = new Pen(c);
		}
		//-----
		public void plot(float[] xp, float[] yp) {
			x = xp;
			y = yp;
			setPlotBounds();    //compute scaling factors
			hasData = true;
		}
		//-----
		public void findBounds() {
			xmin = max;
			xmax = -max;
			ymin = max;
			ymax = -max;

			for (int i = 0; i<  x.Length ; i++) {
				if (x[i] > xmax) xmax = x[i];
				if (x[i] < xmin) xmin = x[i];
				if (y[i] > ymax) ymax = y[i];
				if (y[i] < ymin) ymin = y[i];
			}
		}
		//-----
		public virtual void setPlotBounds() {
			findBounds();
			//compute scaling factors
			h = pic.Height;
			w = pic.Width;
			xfactor = 0.8F * w / (xmax - xmin);
			xpmin = 0.05F * w;
			xpmax = w - xpmin;
			yfactor = 0.9F * h / (ymax - ymin);
			ypmin = 0.05F * h;
			ypmax = h - ypmin;	
		}
		//-----
		public int calcx(float xp) {
			int ix = (int)((xp - xmin) * xfactor + xpmin);
			return ix;
		}
		//-----
		public int calcy(float yp) {
			yp = ((yp - ymin) * yfactor);
			int iy = h - (int)(ypmax - yp);
			return iy;
		}
		//-----
		public abstract void repaint(Graphics g) ;
		//-----
		protected virtual void pic_Paint(object sender, PaintEventArgs e) {
			Graphics g = e.Graphics;
			repaint(g);		
		}
	}
}
