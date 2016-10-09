using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace TreeAdapt
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsKids;
		private System.Windows.Forms.Button btClone;
		private SwimData swdata;
		private ListAdapter lskids;
		private TreeAdapter lsNewKids;
		private DataTable dTable;
		private System.Windows.Forms.TreeView Tree;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private void init() {
			swdata = new SwimData ("swimmers.txt");
			lskids= new ListAdapter (lsKids);
			lsNewKids =  new TreeAdapter (Tree);
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
			this.Tree = new System.Windows.Forms.TreeView();
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
			// Tree
			// 
			this.Tree.ImageIndex = -1;
			this.Tree.Location = new System.Drawing.Point(232, 32);
			this.Tree.Name = "Tree";
			this.Tree.SelectedImageIndex = -1;
			this.Tree.Size = new System.Drawing.Size(168, 160);
			this.Tree.TabIndex = 3;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(416, 237);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.Tree,
																		  this.btClone,
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

		private void btClone_Click(object sender, System.EventArgs e) {
			int i = lskids.SelectedIndex ();
			if( i >= 0) {
				Swimmer sw = swdata.getSwimmer (i);
				lsNewKids.Add (sw);
				lskids.clearSelection ();
			}
		}

		private void btBack_Click(object sender, System.EventArgs e) {
			reload();
		}
	}
}
