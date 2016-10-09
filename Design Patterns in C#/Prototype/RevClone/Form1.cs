using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace DeepSexClone
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsKids;
		private System.Windows.Forms.ListBox lsNewKids;
		private System.Windows.Forms.Button btClone;
		private SexSwimData swdata;
		private System.Windows.Forms.Button btBack;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton opFemale;
		private System.Windows.Forms.RadioButton opMale;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private void init() {
			swdata = new SexSwimData ("swimmers.txt");
			reload();
		}
		private void reload() {
			lsKids.Items.Clear ();
			swdata.moveFirst ();
			while (swdata.hasMoreElements() ) {
				Swimmer sw = swdata.getSwimmer ();
				lsKids.Items.Add (sw.getName() );
			}
		
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
			this.lsKids = new System.Windows.Forms.ListBox();
			this.btClone = new System.Windows.Forms.Button();
			this.btBack = new System.Windows.Forms.Button();
			this.lsNewKids = new System.Windows.Forms.ListBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.opFemale = new System.Windows.Forms.RadioButton();
			this.opMale = new System.Windows.Forms.RadioButton();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// lsKids
			// 
			this.lsKids.Location = new System.Drawing.Point(8, 24);
			this.lsKids.Name = "lsKids";
			this.lsKids.Size = new System.Drawing.Size(128, 160);
			this.lsKids.TabIndex = 0;
			// 
			// btClone
			// 
			this.btClone.Location = new System.Drawing.Point(144, 24);
			this.btClone.Name = "btClone";
			this.btClone.Size = new System.Drawing.Size(48, 24);
			this.btClone.TabIndex = 2;
			this.btClone.Text = "-->";
			this.btClone.Click += new System.EventHandler(this.btClone_Click);
			// 
			// btBack
			// 
			this.btBack.Location = new System.Drawing.Point(144, 152);
			this.btBack.Name = "btBack";
			this.btBack.Size = new System.Drawing.Size(48, 24);
			this.btBack.TabIndex = 3;
			this.btBack.Text = "<---";
			this.btBack.Click += new System.EventHandler(this.btBack_Click);
			// 
			// lsNewKids
			// 
			this.lsNewKids.Location = new System.Drawing.Point(208, 24);
			this.lsNewKids.Name = "lsNewKids";
			this.lsNewKids.Size = new System.Drawing.Size(128, 160);
			this.lsNewKids.TabIndex = 1;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.opMale,
																					this.opFemale});
			this.groupBox1.Location = new System.Drawing.Point(144, 64);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(48, 56);
			this.groupBox1.TabIndex = 4;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Sex";
			// 
			// opFemale
			// 
			this.opFemale.Checked = true;
			this.opFemale.Location = new System.Drawing.Point(8, 16);
			this.opFemale.Name = "opFemale";
			this.opFemale.Size = new System.Drawing.Size(32, 16);
			this.opFemale.TabIndex = 0;
			this.opFemale.TabStop = true;
			this.opFemale.Text = "F";
			// 
			// opMale
			// 
			this.opMale.Location = new System.Drawing.Point(8, 32);
			this.opMale.Name = "opMale";
			this.opMale.Size = new System.Drawing.Size(32, 16);
			this.opMale.TabIndex = 1;
			this.opMale.Text = "M";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(352, 221);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.groupBox1,
																		  this.btBack,
																		  this.btClone,
																		  this.lsNewKids,
																		  this.lsKids});
			this.Name = "Form1";
			this.Text = "Simple Prototype";
			this.groupBox1.ResumeLayout(false);
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

		private void btClone_Click(object sender, System.EventArgs e) {
			SexSwimData newSd = new SexSwimData ();		
			newSd.cloneMe (swdata);
			newSd.sort (opFemale.Checked );
			lsNewKids.Items.Clear() ;
			while(newSd.hasMoreElements() ) {
				Swimmer sw = (Swimmer)newSd.getSwimmer ();
				lsNewKids.Items.Add (sw.getName() );
			}
		}

		private void btBack_Click(object sender, System.EventArgs e) {
			reload();
		}
	}
}
