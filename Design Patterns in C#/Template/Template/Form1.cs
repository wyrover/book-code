using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Template
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox Pic;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private ArrayList triangles;
		public Form1()
		{
			InitializeComponent();
			init();
			
		}
		private void init() {
			triangles = new ArrayList();
			StdTriangle t1 = new StdTriangle(new Point(10, 10), 
				new Point(150, 50), 
				new Point(100, 75));
			IsocelesTriangle t2 = new IsocelesTriangle( 
				new Point(150, 100), new Point(240, 40), 
				new Point(175, 150));
			triangles.Add(t1);
			triangles.Add(t2);
			Pic.Paint+= new PaintEventHandler (TPaint);
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
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
			this.Pic = new System.Windows.Forms.PictureBox();
			this.SuspendLayout();
			// 
			// Pic
			// 
			this.Pic.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.Pic.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.Pic.Location = new System.Drawing.Point(24, 8);
			this.Pic.Name = "Pic";
			this.Pic.Size = new System.Drawing.Size(256, 192);
			this.Pic.TabIndex = 0;
			this.Pic.TabStop = false;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(312, 229);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.Pic});
			this.Name = "Form1";
			this.Text = "Template demo";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}
		private void TPaint (object sender,  System.Windows.Forms.PaintEventArgs e ) {
			Graphics g = e.Graphics;
			for (int i = 0; i<  triangles.Count ; i++) {
				Triangle t = (Triangle)triangles[i];
				t.draw(g);
			}
		}

	}
}
