using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace NameFactory
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button btCompute;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.TextBox txFirst;
		private NameFactory nameFact;
		private System.Windows.Forms.TextBox txName;
		private System.Windows.Forms.TextBox txLast;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private void init() {
			nameFact = new NameFactory ();
		}
		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			init();

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
			this.txName = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.btCompute = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.txFirst = new System.Windows.Forms.TextBox();
			this.txLast = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// txName
			// 
			this.txName.Location = new System.Drawing.Point(56, 40);
			this.txName.Name = "txName";
			this.txName.Size = new System.Drawing.Size(152, 20);
			this.txName.TabIndex = 0;
			this.txName.Text = "";
			// 
			// label1
			// 
			this.label1.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(0)), ((System.Byte)(192)));
			this.label1.Location = new System.Drawing.Point(64, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(88, 16);
			this.label1.TabIndex = 1;
			this.label1.Text = "Enter name";
			// 
			// btCompute
			// 
			this.btCompute.Location = new System.Drawing.Point(96, 176);
			this.btCompute.Name = "btCompute";
			this.btCompute.Size = new System.Drawing.Size(72, 24);
			this.btCompute.TabIndex = 2;
			this.btCompute.Text = "Compute";
			this.btCompute.Click += new System.EventHandler(this.btCompute_Click);
			// 
			// label2
			// 
			this.label2.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(0)), ((System.Byte)(192)));
			this.label2.Location = new System.Drawing.Point(40, 88);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(72, 16);
			this.label2.TabIndex = 3;
			this.label2.Text = "First";
			// 
			// label3
			// 
			this.label3.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(0)), ((System.Byte)(192)));
			this.label3.Location = new System.Drawing.Point(40, 120);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(72, 16);
			this.label3.TabIndex = 3;
			this.label3.Text = "Last";
			// 
			// txFirst
			// 
			this.txFirst.Location = new System.Drawing.Point(128, 88);
			this.txFirst.Name = "txFirst";
			this.txFirst.Size = new System.Drawing.Size(88, 20);
			this.txFirst.TabIndex = 4;
			this.txFirst.Text = "";
			// 
			// txLast
			// 
			this.txLast.Location = new System.Drawing.Point(128, 128);
			this.txLast.Name = "txLast";
			this.txLast.Size = new System.Drawing.Size(88, 20);
			this.txLast.TabIndex = 4;
			this.txLast.Text = "";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.txLast,
																		  this.txFirst,
																		  this.label3,
																		  this.label2,
																		  this.btCompute,
																		  this.label1,
																		  this.txName});
			this.Name = "Form1";
			this.Text = "Name splitter";
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

		private void btCompute_Click(object sender, System.EventArgs e) {
			Namer nm = nameFact.getName (txName.Text );
			txFirst.Text = nm.getFrname ();
			txLast.Text = nm.getLname ();
		}
	}
}
