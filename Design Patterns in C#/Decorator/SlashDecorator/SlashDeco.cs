using System;
using System.Windows.Forms;
using System.Drawing ;

namespace SlashDecorator
{
	/// <summary>
	/// Summary description for SlashDeco.
	/// </summary>
	public class SlashDeco:CoolDecorator 	{
		private Pen rPen;
		//----------------
		public SlashDeco(Control c, Control bc):base(c, bc) {
		  rPen = new Pen(Color.Red , 2);
		}
		//----------------
		public override void paint(object sender, PaintEventArgs e){
			Graphics g = e.Graphics ;
			x1=0; y1=0;
			x2=this.Size.Width ;
			y2=this.Size.Height ;
			g.DrawLine (rPen, x1, y1, x2, y2);		
        }
	}
}
