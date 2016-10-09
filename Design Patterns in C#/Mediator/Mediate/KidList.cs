using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace Mediate
{
	/// <summary>
	/// Summary description for KidList.
	/// </summary>
	public class KidList : System.Windows.Forms.ListBox, Command
	{
		private System.ComponentModel.Container components = null;
		private Mediator med;
		//-----
		public KidList() 		{
			InitializeComponent();
		}
		//-----
		public void Execute() {
			med.kidPicked ();
		}
		//-----
		public void setMediator(Mediator md) {
			med = md;
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
