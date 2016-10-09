using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace SlashDecorator
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btQuit;
		private System.Windows.Forms.Button btButtonA;
		private SlashDeco slash;
		private CoolDecorator cdec;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			
			InitializeComponent();
			init();
        }
		private void init() {
			//save the position of the button
			Point pt = btButtonA.Location ;
			//create a cool decorator
			cdec = new CoolDecorator (btButtonA, btButtonA);
			//decorate the cool decorator with a slash
			slash = new SlashDeco (cdec, btButtonA);
			slash.locate (pt);  
			//add outside decorator to the layout 
			//and remove the button from the layout
			this.Controls.AddRange(new System.Windows.Forms.Control[] {slash});
			this.Controls.Remove (btButtonA);
			
			//this.Controls.AddRange(new System.Windows.Forms.Control[] {cdec});
			
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
			this.btButtonA = new System.Windows.Forms.Button();
			this.btQuit = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// btButtonA
			// 
			this.btButtonA.Location = new System.Drawing.Point(32, 32);
			this.btButtonA.Name = "btButtonA";
			this.btButtonA.Size = new System.Drawing.Size(80, 40);
			this.btButtonA.TabIndex = 1;
			this.btButtonA.Text = "A button";
			this.btButtonA.Click += new System.EventHandler(this.btButtonA_Click);
			// 
			// btQuit
			// 
			this.btQuit.Location = new System.Drawing.Point(160, 32);
			this.btQuit.Name = "btQuit";
			this.btQuit.Size = new System.Drawing.Size(72, 40);
			this.btQuit.TabIndex = 0;
			this.btQuit.Text = "Quit";
			this.btQuit.Click += new System.EventHandler(this.btQuit_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(264, 117);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btButtonA,
																		  this.btQuit});
			this.Name = "Form1";
			this.Text = "Cool Decorator";
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

		private void btQuit_Click(object sender, System.EventArgs e) {
			this.Close ();
		}

		private void btButtonA_Click(object sender, System.EventArgs e) {
			
		}
	}
}
