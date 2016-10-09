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
	public class BarPlot : PlotWindow
	{
		private float xmin, xmax;
		protected ArrayList colors;		
		private System.ComponentModel.Container components = null;

		public BarPlot()
		{
			InitializeComponent();
			init();
		}
		protected override void init() {
			base.init ();
			//create array of colors for bars
			colors = new ArrayList();
			colors.Add(new SolidBrush(Color.Red));
			colors.Add(new SolidBrush(Color.Green));
			colors.Add(new SolidBrush(Color.Blue));
			colors.Add(new SolidBrush(Color.Magenta));
			colors.Add(new SolidBrush(Color.Yellow));
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
		public override void repaint(Graphics g) {
			if (hasData) {
				for (int i = 0; i< x.Length; i++){
					int ix = calcx(x[i]);
					int iy = calcy(y[i]);
					Brush br = (Brush)colors[i];
					g.FillRectangle(br, ix, h - iy, 20, iy);
				}
			}
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
			//this.pic.Paint += new System.Windows.Forms.PaintEventHandler(this.pic_Paint);
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

		
	}
}
