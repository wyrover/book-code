using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace PanelDecorator
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private PanelDecorator.decoPanel decoPanel1;
		private System.Windows.Forms.Button btQuit;
		private System.Windows.Forms.Button btDecor;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			decoPanel1.init (btDecor);
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			this.btQuit = new System.Windows.Forms.Button();
			this.btDecor = new System.Windows.Forms.Button();
			this.decoPanel1 = new PanelDecorator.decoPanel();
			this.decoPanel1.SuspendLayout();
			this.SuspendLayout();
			// 
			// btQuit
			// 
			this.btQuit.Location = new System.Drawing.Point(152, 40);
			this.btQuit.Name = "btQuit";
			this.btQuit.Size = new System.Drawing.Size(80, 32);
			this.btQuit.TabIndex = 1;
			this.btQuit.Text = "Quit";
			this.btQuit.Click += new System.EventHandler(this.btQuit_Click);
			// 
			// btDecor
			// 
			this.btDecor.Location = new System.Drawing.Point(8, 8);
			this.btDecor.Name = "btDecor";
			this.btDecor.Size = new System.Drawing.Size(80, 32);
			this.btDecor.TabIndex = 0;
			this.btDecor.Text = "A Button";
			// 
			// decoPanel1
			// 
			this.decoPanel1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					 this.btDecor});
			this.decoPanel1.Location = new System.Drawing.Point(24, 40);
			this.decoPanel1.Name = "decoPanel1";
			this.decoPanel1.Size = new System.Drawing.Size(88, 40);
			this.decoPanel1.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(264, 133);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btQuit,
																		  this.decoPanel1});
			this.Name = "Form1";
			this.Text = "Form1";
			this.decoPanel1.ResumeLayout(false);
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
	}
}
