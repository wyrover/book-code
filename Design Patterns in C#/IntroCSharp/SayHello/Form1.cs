using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace SayHello
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txHi;
		private System.Windows.Forms.Button btHello;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()		{
			InitializeComponent();
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
		private void InitializeComponent() 		{
			this.btHello = new System.Windows.Forms.Button();
			this.txHi = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// btHello
			// 
			this.btHello.Location = new System.Drawing.Point(80, 112);
			this.btHello.Name = "btHello";
			this.btHello.Size = new System.Drawing.Size(64, 24);
			this.btHello.TabIndex = 1;
			this.btHello.Text = "Hello";
			this.btHello.Click += new System.EventHandler(this.btHello_Click);
			// 
			// txHi
			// 
			this.txHi.Location = new System.Drawing.Point(64, 48);
			this.txHi.Name = "txHi";
			this.txHi.Size = new System.Drawing.Size(104, 20);
			this.txHi.TabIndex = 0;
			this.txHi.Text = "";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(240, 213);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btHello,
																		  this.txHi});
			this.Name = "Form1";
			this.Text = "Hello window";
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

		private void btHello_Click(object sender, System.EventArgs e) {
			txHi.Text ="Hello there";
		}

			}
}
