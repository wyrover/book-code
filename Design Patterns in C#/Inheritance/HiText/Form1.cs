using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace HiText
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private HiText.HiTextBox txHi;
		private System.Windows.Forms.TextBox txNormal;
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
			this.txNormal = new System.Windows.Forms.TextBox();
			this.txHi = new HiText.HiTextBox();
			this.SuspendLayout();
			// 
			// txNormal
			// 
			this.txNormal.Location = new System.Drawing.Point(56, 24);
			this.txNormal.Name = "txNormal";
			this.txNormal.Size = new System.Drawing.Size(120, 20);
			this.txNormal.TabIndex = 1;
			this.txNormal.Text = "Normal";
			// 
			// txHi
			// 
			this.txHi.Location = new System.Drawing.Point(56, 64);
			this.txHi.Name = "txHi";
			this.txHi.Size = new System.Drawing.Size(120, 20);
			this.txHi.TabIndex = 0;
			this.txHi.Text = "Highlighted";
			//this.txHi.Enter += new System.EventHandler(this.txHi_Enter);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(240, 181);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.txNormal,
																		  this.txHi});
			this.Name = "Form1";
			this.Text = "Highlighted text demo";
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

		

		
	}
}
