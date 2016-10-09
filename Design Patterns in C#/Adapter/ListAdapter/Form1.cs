using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace LstAdapter
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsKids;
		private System.Windows.Forms.ListBox lsNewKids;
		private System.Windows.Forms.Button btClone;
		private SwimData swdata;
		private ListAdapter lskids, lsnewKids;
		private System.Windows.Forms.Button putBack;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private void init() {
			swdata = new SwimData ("swimmers.txt");
			lskids= new ListAdapter (lsKids);
			lsnewKids =  new ListAdapter (lsNewKids);
			reload();
		}
		private void reload() {
			lskids.Clear ();
			swdata.moveFirst ();
			while (swdata.hasMoreElements() ) {
				Swimmer sw = swdata.getSwimmer ();
				lskids.Add (sw.getName() );
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
			this.lsNewKids = new System.Windows.Forms.ListBox();
			this.putBack = new System.Windows.Forms.Button();
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
			this.btClone.Location = new System.Drawing.Point(144, 56);
			this.btClone.Name = "btClone";
			this.btClone.Size = new System.Drawing.Size(48, 24);
			this.btClone.TabIndex = 2;
			this.btClone.Text = "-->";
			this.btClone.Click += new System.EventHandler(this.btClone_Click);
			// 
			// lsNewKids
			// 
			this.lsNewKids.Location = new System.Drawing.Point(208, 24);
			this.lsNewKids.Name = "lsNewKids";
			this.lsNewKids.Size = new System.Drawing.Size(128, 160);
			this.lsNewKids.TabIndex = 1;
			// 
			// putBack
			// 
			this.putBack.Location = new System.Drawing.Point(144, 104);
			this.putBack.Name = "putBack";
			this.putBack.Size = new System.Drawing.Size(48, 24);
			this.putBack.TabIndex = 3;
			this.putBack.Text = "<--";
			this.putBack.Click += new System.EventHandler(this.putBack_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(352, 221);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.putBack,
																		  this.btClone,
																		  this.lsNewKids,
																		  this.lsKids});
			this.Name = "Form1";
			this.Text = "List adapter";
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

		private void btClone_Click(object sender, EventArgs e) {
			int i = lskids.SelectedIndex ();
			if( i >= 0) {
				Swimmer sw = swdata.getSwimmer (i);
				lsnewKids.Add (sw.getName() +"\t"+sw.getTime ());
				lskids.clearSelection ();
			}
		}

		private void btBack_Click(object sender, System.EventArgs e) {
			reload();
		}

		private void putBack_Click(object sender, EventArgs e) {
			int i = lsnewKids.SelectedIndex ();
			if(i >= 0)
				lsNewKids.Items.RemoveAt (i); 
		}
	}
}
