using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Strategy
{
	/// <summary>
	/// Summary description for BarPlot.
	/// </summary>
	public class BarPlot : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox pic;
		protected float ymin, ymax, xfactor, yfactor;
		protected float xpmin, xpmax, ypmin, ypmax, xp, yp;
		private float xmin, xmax;
		private int w, h;
		protected float[] x, y;
		private ArrayList colors;
		protected Pen bPen;
		protected bool hasData;
		protected const float max = 1.0e38f;

		private System.ComponentModel.Container components = null;

		public BarPlot()
		{
			InitializeComponent();
		}
		public void setPenColor(Color c){
			bPen = new Pen(c);
		}
		public void plot(float[] xp, float[] yp) {
			x = xp;
			y = yp;
			setPlotBounds();    //compute scaling factors
			hasData = true;
			pic.Refresh();
		}
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
			//create array of colors for bars
			colors = new ArrayList();
			colors.Add(new SolidBrush(Color.Red));
			colors.Add(new SolidBrush(Color.Green));
			colors.Add(new SolidBrush(Color.Blue));
			colors.Add(new SolidBrush(Color.Magenta));
			colors.Add(new SolidBrush(Color.Yellow));
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

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.pic = new System.Windows.Forms.PictureBox();
			this.SuspendLayout();
			// 
			// pic
			// 
			this.pic.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.pic.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pic.Location = new System.Drawing.Point(24, 24);
			this.pic.Name = "pic";
			this.pic.Size = new System.Drawing.Size(248, 208);
			this.pic.TabIndex = 0;
			this.pic.TabStop = false;
			this.pic.Paint += new System.Windows.Forms.PaintEventHandler(this.pic_Paint);
			// 
			// BarPlot
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.pic});
			this.Name = "BarPlot";
			this.Text = "BarPlot";
			this.ResumeLayout(false);

		}
		#endregion

		protected virtual void pic_Paint(object sender, PaintEventArgs e) {
			Graphics g = e.Graphics;
			if (hasData) {
            for (int i = 0; i< x.Length; i++){
                int ix = calcx(x[i]);
                int iy = calcy(y[i]);
                Brush br = (Brush)colors[i];
				g.FillRectangle(br, ix, h - iy, 20, iy);
			}
		}
		}
	}
}
