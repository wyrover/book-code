using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Proxy
{
	public class Form1 : System.Windows.Forms.Form 	{
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.PictureBox Pic;
		private System.ComponentModel.Container components = null;
		private ImageProxy imgProxy;
		//-----
		private void init() {
			imgProxy = new ImageProxy ();
		}
		//-----
		public Form1() {
			InitializeComponent();
			init();
		}
		//-----
		protected override void Dispose( bool disposing ) 		{
			if( disposing ) 			{
				if (components != null) 				{
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
			this.button1 = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// Pic
			// 
			this.Pic.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.Pic.Location = new System.Drawing.Point(32, 24);
			this.Pic.Name = "Pic";
			this.Pic.Size = new System.Drawing.Size(224, 200);
			this.Pic.TabIndex = 0;
			this.Pic.TabStop = false;
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(104, 240);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(64, 24);
			this.button1.TabIndex = 1;
			this.button1.Text = "Load";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.button1,
																		  this.Pic});
			this.Name = "Form1";
			this.Text = "Proxy demo";
			this.ResumeLayout(false);

		}
		#endregion

		[STAThread]
		static void Main() 
		{
			Application.Run(new Form1());
		}
		//-----
		private void button1_Click(object sender, System.EventArgs e) {
			Pic.Image = imgProxy.getImage ();
		}
	}
}
