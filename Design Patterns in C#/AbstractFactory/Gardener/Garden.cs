using System;
using System.Drawing ;
namespace Gardener
{
	/// <summary>
	/// Summary description for Garden.
	/// </summary>
	public class Garden {
		protected Plant center, shade, border;
		protected bool showCenter, showShade, showBorder;
		//select which ones to display
		public void setCenter() {showCenter = true;}
		public void setBorder() {showBorder =true;}
		public void setShade() {showShade =true;}
		//draw each plant
		public void draw(Graphics g) {
			if (showCenter) center.draw (g, 100, 100);
			if (showShade) shade.draw (g, 10, 50);
			if (showBorder) border.draw (g, 50, 150);
		}
	}
}
