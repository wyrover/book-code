using System;
using System.Windows.Forms;
using System.Drawing ;
namespace CoolDec
{
	/// <summary>
	/// Summary description for CoolDecorator.
	/// </summary>
	public class CoolDecorator :Panel, Decorator
	{
		protected Control contl;
		protected Pen bPen, wPen, gPen;
		private bool mouse_over;
		protected float x1, y1, x2, y2;
		//----------------------------------
			public CoolDecorator(Control c) {
				contl = c;		//copy in control
				//add button to controls contained in panel
				this.Controls.AddRange(new Control[] {contl});
				locate(c.Location );
				this.Name = "deco"+contl.Name ;
				this.Size = contl.Size;
				x1 = c.Location.X - 1;
				y1 = c.Location.Y - 1;
				x2 = c.Size.Width;
				y2 = c.Size.Height;
							
				//create the overwrite pens
				gPen = new Pen(c.BackColor, 2);	//gray pen overwrites borders
				bPen = new Pen(Color.Black , 1);	
				wPen = new Pen(Color.White, 1);
		
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
		//------------------------------
		public void locate(Point p) {
			//moves panel to button position
			this.Location = p;
			contl.Location =new Point(0,0);
			x1 = p.X ;
			y1 = p.Y ;
		}
		//------------------------------
		public virtual void locate(Control c) {
			this.Location = c.Location;
			c.Location =new Point (0,0);
			x1 = c.Location.X - 1;
			y1 = c.Location.Y - 1;
			x2 = c.Size.Width;
			y2 = c.Size.Height;
			
		}
		//------------------------------
		public void mouseMove(object sender, MouseEventArgs e){
			mouse_over = true;
		}
		//------------------------------
		public void mouseEnter(object sender, EventArgs e){
			mouse_over = true;
			this.Refresh ();
		}
		//------------------------------
		public void mouseLeave(object sender, EventArgs e){
			mouse_over = false;
			this.Refresh ();
		}
		//------------------------------
		public virtual void paint(object sender, PaintEventArgs e){
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
	}
	}
