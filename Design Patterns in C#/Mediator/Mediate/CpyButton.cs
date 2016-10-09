using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace Mediate
{
	/// <summary>
	/// Button command passes its click to the mediator
	/// </summary>
	public class CpyButton : System.Windows.Forms.Button, Command 	{
		private Container components = null;
		private Mediator med;
		//-----
		public CpyButton() 		{
			InitializeComponent();
		}
		//-----
		public void setMediator(Mediator md) {
			med = md;
		}
		//-----
		public void Execute() {
			med.copyClicked ();
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
