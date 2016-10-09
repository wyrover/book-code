using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace FileredIterator
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form {
		private System.Windows.Forms.ListBox lsKids;
		private System.Windows.Forms.ListBox lsClubKids;
		private System.Windows.Forms.ComboBox cbClubs;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private KidData kdata;
		public Form1() {
			InitializeComponent();
			init();
		}
		private void init() {
			kdata = new KidData("50free.txt");
			KidIterator kiter = kdata.getIterator ();
			while(kiter.MoveNext() ) {
				Kid kd = (Kid)kiter.Current;
				lsKids.Items.Add (kd.getFrname() +" "+kd.getLname ());
			}
			IDictionaryEnumerator clubiter = kdata.getClubs ();
			while(clubiter.MoveNext ()) {
				cbClubs.Items.Add ((string)clubiter.Value );
			}
			
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if (components != null) {
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
		private void InitializeComponent() {
			this.lsKids = new System.Windows.Forms.ListBox();
			this.lsClubKids = new System.Windows.Forms.ListBox();
			this.cbClubs = new System.Windows.Forms.ComboBox();
			this.SuspendLayout();
			// 
			// lsKids
			// 
			this.lsKids.Location = new System.Drawing.Point(8, 16);
			this.lsKids.Name = "lsKids";
			this.lsKids.Size = new System.Drawing.Size(128, 199);
			this.lsKids.TabIndex = 0;
			// 
			// lsClubKids
			// 
			this.lsClubKids.Location = new System.Drawing.Point(160, 56);
			this.lsClubKids.Name = "lsClubKids";
			this.lsClubKids.Size = new System.Drawing.Size(120, 160);
			this.lsClubKids.TabIndex = 1;
			// 
			// cbClubs
			// 
			this.cbClubs.DropDownWidth = 120;
			this.cbClubs.Location = new System.Drawing.Point(160, 16);
			this.cbClubs.Name = "cbClubs";
			this.cbClubs.Size = new System.Drawing.Size(120, 21);
			this.cbClubs.TabIndex = 2;
			this.cbClubs.SelectedIndexChanged += new System.EventHandler(this.cbClubs_SelectedIndexChanged);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.cbClubs,
																		  this.lsClubKids,
																		  this.lsKids});
			this.Name = "Form1";
			this.Text = "Filtered Iterator demo";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			Application.Run(new Form1());
		}

		private void cbClubs_SelectedIndexChanged(object sender, System.EventArgs e) {
			string club = (String)cbClubs.SelectedItem ;
			FilteredIterator iter = kdata.getFilteredIterator ( club);
			lsClubKids.Items.Clear ();
			while(iter.MoveNext() ) {
				Kid kd = (Kid) iter.Current;
				lsClubKids.Items.Add (kd.getFrname() +" "+kd.getLname ());

			}
		}
	}
}
