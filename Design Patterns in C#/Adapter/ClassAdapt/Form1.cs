using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ClassAdapt
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btClone;
		private SwimData swdata;
		private ClassAdapt.MyList lsNewKids;
		private ClassAdapt.MyList lsKids;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private void init() {
			swdata = new SwimData ("swimmers.txt");
			//lskids= new ListAdapter (lsKids);
			//lsnewKids =  new MyAdapter ();
			reload();
		}
		private void reload() {
			lsKids.Clear ();
			swdata.moveFirst ();
			while (swdata.hasMoreElements() ) {
				Swimmer sw = swdata.getSwimmer ();
				lsKids.Add (sw.getName() );
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
			this.btClone = new System.Windows.Forms.Button();
			this.lsNewKids = new ClassAdapt.MyList();
			this.lsKids = new ClassAdapt.MyList();
			this.SuspendLayout();
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
			this.lsNewKids.Location = new System.Drawing.Point(224, 24);
			this.lsNewKids.Name = "lsNewKids";
			this.lsNewKids.Size = new System.Drawing.Size(120, 147);
			this.lsNewKids.TabIndex = 3;
			// 
			// lsKids
			// 
			this.lsKids.Location = new System.Drawing.Point(16, 16);
			this.lsKids.Name = "lsKids";
			this.lsKids.Size = new System.Drawing.Size(120, 160);
			this.lsKids.TabIndex = 4;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(352, 221);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lsKids,
																		  this.lsNewKids,
																		  this.btClone});
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

		private void btClone_Click(object sender, System.EventArgs e) {
			int i = lsKids.SelectedIndex ;
			if( i >= 0) {
				Swimmer sw = swdata.getSwimmer (i);
				lsNewKids.Add (sw);
				lsKids.clearSelection ();
			}
		}

		private void btBack_Click(object sender, System.EventArgs e) {
			reload();
		}
	}
}
