using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace FileReader
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.ListBox lsFiles;
		private System.Windows.Forms.TextBox txFilename;
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
			this.button1 = new System.Windows.Forms.Button();
			this.lsFiles = new System.Windows.Forms.ListBox();
			this.txFilename = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(80, 152);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(88, 32);
			this.button1.TabIndex = 0;
			this.button1.Text = "Start";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// lsFiles
			// 
			this.lsFiles.Location = new System.Drawing.Point(48, 40);
			this.lsFiles.Name = "lsFiles";
			this.lsFiles.Size = new System.Drawing.Size(176, 95);
			this.lsFiles.TabIndex = 1;
			// 
			// txFilename
			// 
			this.txFilename.Location = new System.Drawing.Point(48, 16);
			this.txFilename.Name = "txFilename";
			this.txFilename.Size = new System.Drawing.Size(168, 20);
			this.txFilename.TabIndex = 2;
			this.txFilename.Text = "outFile.txt";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(248, 221);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.txFilename,
																		  this.lsFiles,
																		  this.button1});
			this.Name = "Form1";
			this.Text = "Form1";
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

		private void button1_Click(object sender, System.EventArgs e) {
			csFile fl=new csFile (txFilename.Text );
			fl.OpenForWrite ();
			fl.writeLine ("hi there");
			fl.close ();
			fl.OpenForRead ();
			lsFiles.Items.Add (fl.readLine ());
			fl.close ();

		}
	}
}
