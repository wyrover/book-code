using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace HiText
{
	/// <summary>
	/// Summary description for HiTextBox.
	/// </summary>
	public class HiTextBox : System.Windows.Forms.TextBox 
	{
		private System.ComponentModel.Container components = null;
	//-------------
		private void init() {
		//add event handler to Enter event
			this.Enter += new System.EventHandler (highlight);
		}
		//-------------
		//event handler for highlight event
		private void highlight(object obj, System.EventArgs e) {
			this.SelectionStart =0;
			this.SelectionLength =this.Text.Length ;
		}
		//-------------
		public HiTextBox() 		{
			// This call is required by the Windows.Forms Form Designer.
			InitializeComponent();
			init();
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
