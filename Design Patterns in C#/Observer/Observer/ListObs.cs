using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace Observer
{
	/// <summary>
	/// Summary description for ListObs.
	/// </summary>
	public class ListObs : System.Windows.Forms.Form, Observer
	{
		private System.Windows.Forms.ListBox lsColors;
		/// <summary>
		/// Adds text of color to list box
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ListObs(Subject subj) 		{
			InitializeComponent();
			init(subj);
		}
		//------
		public void init(Subject subj) {
			subj.registerInterest (this);
		}
		//------
		public void sendNotify(string message){
			lsColors.Items.Add(message);
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
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
			this.lsColors = new System.Windows.Forms.ListBox();
			this.SuspendLayout();
			// 
			// lsColors
			// 
			this.lsColors.Location = new System.Drawing.Point(16, 16);
			this.lsColors.Name = "lsColors";
			this.lsColors.Size = new System.Drawing.Size(216, 173);
			this.lsColors.TabIndex = 0;
			// 
			// ListObs
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(264, 213);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lsColors});
			this.Name = "ListObs";
			this.Text = "List observer";
			this.ResumeLayout(false);

		}
		#endregion
	}
}
