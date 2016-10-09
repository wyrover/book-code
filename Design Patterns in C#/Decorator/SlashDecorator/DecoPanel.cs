using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace SlashDecorator
{
	/// <summary>
	/// Summary description for DecoPanel.
	/// </summary>
	public class DecoPanel : System.Windows.Forms.Panel 
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private Control c;
		private Pen bPen, wPen, gPen;
		private bool gotControl;
		private bool mouse_over;
		private float x1, y1, x2, y2;
		public DecoPanel()		{
			// This call is required by the Windows.Forms Form Designer.
		   InitializeComponent();
		   init();
		}
		//------
		private void init() {
			bPen = new Pen(System.Drawing.Color.Black, 1);
			wPen = new Pen(System.Drawing.Color.White, 1);//
			gotControl = false;      //no control 
		}
		//-----
		protected override void Dispose( bool disposing )	{
			if( disposing )			{
				if(components != null)				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}
		public void ctMouseEnter(object sender,  EventArgs e){
			mouse_over = true;
			Refresh();
		}
		public void ctMouseLeave(object sender,  EventArgs e){
			mouse_over = false;
			this.Refresh();
		}
		public void ctMouseMove(object sender,  MouseEventArgs e){
			mouse_over = true;
		}
		public void ctPaint(object sender, PaintEventArgs e) {
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
		protected override void OnPaint ( System.Windows.Forms.PaintEventArgs e ) {
			//This is where we find out about the control
			if (! gotControl) {		//once only
				//get the control
				c = this.Controls[0];
				//set the panel size 
				//1 pixel bigger all around
				Size sz = new Size ();
				sz.Width = c.Size.Width + 2;
				sz.Height = c.Size.Height + 2;
				this.Size = sz;
            
				x1 = c.Location.X - 1;
				y1 = c.Location.Y - 1;
				x2 = c.Size.Width;
				y2 = c.Size.Height;
				//create the overwrite pen
				gPen = new Pen(c.BackColor, 2);
				gotControl = true;        //only once
				//mouse over, enter handler
				EventHandler evh = new EventHandler(ctMouseEnter);
				c.MouseHover += evh;
				c.MouseEnter+= evh;
				//mouse move handler
				c.MouseMove += new MouseEventHandler(ctMouseMove);
				c.MouseLeave += new EventHandler(ctMouseLeave);
				//paint handler catches button's paint
				c.Paint += new PaintEventHandler( ctPaint);
				}
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
