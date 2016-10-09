using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Windows.Forms;

namespace State
{
	/// <summary>
	/// Summary description for ComdToolBarButton.
	/// </summary>
	public class ComdToolBarButton : ToolBarButton , Command 	{
		private System.ComponentModel.Container components = null;
		protected Mediator med;
		protected  bool selected;
		public ComdToolBarButton(string caption, Mediator md) 		{
			InitializeComponent();
			med = md;
			this.Text =caption;
		}
		//------
		public void setSelected(bool b) {
			selected = b;
			if(!selected)
				this.Pushed =false;
		}
		//-----
		public virtual void Execute() {
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
