using System;
using System.Drawing ;
using System.Windows.Forms;
namespace Strategy {
	/// <summary>
	/// Summary description for LinePlot.
	/// </summary>
	public class LinePlot :BarPlot {
		public LinePlot() {
			init();
		}
		protected override void init() {
			base.init ();
			bPen = new Pen(Color.Black);
			this.Text = "Line Plot";
		}
		//protected override void pic_Paint(object sender, PaintEventArgs e) {
		public override void repaint(Graphics g) {
			//Graphics g= e.Graphics;
			if (hasData) {
				for (int i = 1; i< x.Length; i++) {
					int ix = calcx(x[i - 1]);
					int iy = calcy(y[i - 1]);
					int ix1 = calcx(x[i]);
					int iy1 = calcy(y[i]);
					g.DrawLine(bPen, ix, iy, ix1, iy1);
				}
			}
		}
	}
}
