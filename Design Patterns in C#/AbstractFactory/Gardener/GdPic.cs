using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace Gardener
{
	/// <summary>
	/// Summary description for GdPic.
	/// </summary>
	public class GdPic : System.Windows.Forms.PictureBox 
	{
		/// <summary> 
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private Brush br;
		private Garden gden;
		private void init () {
			br = new SolidBrush (Color.LightGray );
		}
		public GdPic() 	{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			init();
		}
		public void setGarden(Garden garden) {
			gden = garden;
		}
		protected override void OnPaint ( PaintEventArgs pe ){
			Graphics g = pe.Graphics;
			g.FillEllipse (br, 5, 5, 100, 100);
			if(gden != null)
				gden.draw (g);	
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
			// 
			// GdPic
			// 
			
		}
		#endregion

	}
}
