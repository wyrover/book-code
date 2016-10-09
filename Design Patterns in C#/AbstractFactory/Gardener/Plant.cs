using System;
using System.Drawing;

namespace Gardener
{
	/// <summary>
	/// Summary description for Plant.
	/// </summary>
	public class Plant 	{
		private string name;
		private Brush br;
		private Font font;

		public Plant(string pname) {
			name = pname;     //save name
			font = new Font ("Arial", 12);
			br = new SolidBrush (Color.Black );
		}        
		//-------------
		public void draw(Graphics g, int x, int y) {
			g.DrawString (name, font, br, x, y);
		}
	}
}
