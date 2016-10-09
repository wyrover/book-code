using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace FormatValue
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txEntry;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btEnter;
		private System.Windows.Forms.ListBox lsTimes;
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
			this.txEntry = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.btEnter = new System.Windows.Forms.Button();
			this.lsTimes = new System.Windows.Forms.ListBox();
			this.SuspendLayout();
			// 
			// txEntry
			// 
			this.txEntry.Location = new System.Drawing.Point(104, 32);
			this.txEntry.Name = "txEntry";
			this.txEntry.Size = new System.Drawing.Size(104, 20);
			this.txEntry.TabIndex = 0;
			this.txEntry.Text = "";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 32);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(80, 24);
			this.label1.TabIndex = 1;
			this.label1.Text = "Enter time";
			// 
			// btEnter
			// 
			this.btEnter.Location = new System.Drawing.Point(120, 72);
			this.btEnter.Name = "btEnter";
			this.btEnter.Size = new System.Drawing.Size(56, 24);
			this.btEnter.TabIndex = 2;
			this.btEnter.Text = "Enter";
			this.btEnter.Click += new System.EventHandler(this.btEnter_Click);
			// 
			// lsTimes
			// 
			this.lsTimes.Location = new System.Drawing.Point(48, 120);
			this.lsTimes.Name = "lsTimes";
			this.lsTimes.Size = new System.Drawing.Size(192, 95);
			this.lsTimes.TabIndex = 3;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lsTimes,
																		  this.btEnter,
																		  this.label1,
																		  this.txEntry});
			this.Name = "Form1";
			this.Text = "Format and value";
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

		private void btEnter_Click(object sender, System.EventArgs e) {
			FormatTime fmt = new FormatTime (txEntry.Text );
			lsTimes.Items.Add (fmt.getTime() );
		}
	}
}
