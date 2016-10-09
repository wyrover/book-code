using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace PanelDecorator
{
	/// <summary>
	/// Summary description for decoPanel.
	/// </summary>
	public class decoPanel : System.Windows.Forms.Panel 
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private Control cntl;
		private Pen bPen, wPen, gPen;
		private bool mouse_over;
		private int x1, x2, y1, y2;
		public decoPanel()
		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			mouse_over = false;
		}
		public void init(Control c) {
			cntl = c;
			cntl.Location =new Point(1,1);
			this.Size = cntl.Size ;
			gPen = new Pen(c.BackColor, 2);
			bPen =  new Pen(Color.Black ,1);
			wPen = new Pen(Color.White , 1);
			x1=0; y1=0;
			x2 = cntl.Size.Width -1;
			y2 = cntl.Size.Height - 1;
		
			//mouse over, enter handler
			EventHandler evh = new EventHandler(mouseEnter);
			c.MouseHover += evh;
			c.MouseEnter+= evh;
			//mouse move handler
			c.MouseMove += new MouseEventHandler(mouseMove);
			c.MouseLeave += new EventHandler(mouseLeave);
			//paint handler catches button's paint
			c.Paint += new PaintEventHandler( paint);
		}
		public void mouseMove(object sender, MouseEventArgs e){
			mouse_over = true;
		}
		public void mouseEnter(object sender, EventArgs e){
			mouse_over = true;
			//this.Refresh ();
		}
		public void mouseLeave(object sender, EventArgs e){
			mouse_over = false;
		}
		public void paint(object sender, PaintEventArgs e){
			//draw over button to change its outline
			Graphics g = e.Graphics;
			const int  d = 1;
			//draw over everything in gray first
			g.DrawRectangle(gPen, 0, 0, x2 - 1, y2 - 1);
			//draw black and white boundaries
			//if the mouse is over
			if( mouse_over) {
				g.DrawLine(bPen, 0, 0, x2 - d, 0);
				g.DrawLine(bPen, 0, 0, 0, y2 - 1);
				g.DrawLine(wPen, 0, y2 - d, x2 - d, y2 - d);
				g.DrawLine(wPen, x2 - d, 0, x2 - d, y2 - d);
			}
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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion
	}
}
