using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace SimpleIterator
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsKids;
		private System.ComponentModel.Container components = null;
		private KidData kids;
		public Form1() 		{
			InitializeComponent();
			init();
		}
		private void init() {
			kids = new KidData("50free.txt");
			KidIterator kdi = kids.getIterator ();
			while (kdi.MoveNext () ) {
				Kid kd = (Kid)kdi.Current ;
				lsKids.Items.Add (kd.getFrname()+ " "+ kd.getLname ());
			}
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
			this.SuspendLayout();
			// 
			// lsKids
			// 
			this.lsKids.Location = new System.Drawing.Point(40, 32);
			this.lsKids.Name = "lsKids";
			this.lsKids.Size = new System.Drawing.Size(208, 173);
			this.lsKids.TabIndex = 0;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lsKids});
			this.Name = "Form1";
			this.Text = "Simple Iterator Demo";
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
