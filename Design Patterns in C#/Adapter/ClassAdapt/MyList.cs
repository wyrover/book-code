using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace ClassAdapt
{
	public class MyList : System.Windows.Forms.ListBox, ListAdapter {
		private System.ComponentModel.Container components = null;
	  //-----
		public MyList() 		{
			InitializeComponent();
		}
		//-----
		public void Add(string s) {
			this.Items.Add (s);
		}
		//-----
		public void Add(Swimmer sw) {
			this.Items.Add (sw.getName() + 
				"\t" + sw.getAge ().ToString () );
		}
		//-----
		public void Clear() {
			this.Items.Clear ();
		}
		//-----
		public void clearSelection() {
			this.SelectedIndex = -1;
		}
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

		#region Component Designer generated code
		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion
	}
}
