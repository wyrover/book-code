using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Mediate
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txName;
		private System.Windows.Forms.ListBox lsSelected;
		private Mediate.KidList lsKids;
		private Mediate.CpyButton btCopy;
		private Mediate.ClrButton btClear;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private Mediator med;
		public Form1() 		{
			InitializeComponent();
			init();
		}
		//-----
		private void init() {
			//set up mediator and pass in referencs to controls
			med = new Mediator (btCopy, btClear, lsKids, lsSelected);
			btCopy.setMediator (med);	//set mediator ref in each control
			btClear.setMediator (med);
			lsKids.setMediator (med);
			med.setText (txName);		//tell mediator about text box
			//create event handler for all command objects
			EventHandler evh = new EventHandler (clickHandler);
			btClear.Click += evh;
			btCopy.Click += evh;
			lsKids.SelectedIndexChanged += evh;
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		//each control is a command object
		public void clickHandler(object obj, EventArgs e) {
			Command comd = (Command)obj;	//get command object
			comd.Execute ();				//and execute command
		}
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
			this.txName = new System.Windows.Forms.TextBox();
			this.lsSelected = new System.Windows.Forms.ListBox();
			this.lsKids = new Mediate.KidList();
			this.btCopy = new Mediate.CpyButton();
			this.btClear = new Mediate.ClrButton();
			this.SuspendLayout();
			// 
			// txName
			// 
			this.txName.Location = new System.Drawing.Point(8, 16);
			this.txName.Name = "txName";
			this.txName.Size = new System.Drawing.Size(104, 20);
			this.txName.TabIndex = 0;
			this.txName.Text = "";
			// 
			// lsSelected
			// 
			this.lsSelected.Location = new System.Drawing.Point(168, 56);
			this.lsSelected.Name = "lsSelected";
			this.lsSelected.Size = new System.Drawing.Size(144, 173);
			this.lsSelected.TabIndex = 2;
			// 
			// lsKids
			// 
			this.lsKids.Location = new System.Drawing.Point(8, 64);
			this.lsKids.Name = "lsKids";
			this.lsKids.Size = new System.Drawing.Size(136, 160);
			this.lsKids.TabIndex = 5;
			// 
			// btCopy
			// 
			this.btCopy.Location = new System.Drawing.Point(168, 16);
			this.btCopy.Name = "btCopy";
			this.btCopy.Size = new System.Drawing.Size(56, 24);
			this.btCopy.TabIndex = 6;
			this.btCopy.Text = "Copy";
			// 
			// btClear
			// 
			this.btClear.Location = new System.Drawing.Point(240, 16);
			this.btClear.Name = "btClear";
			this.btClear.Size = new System.Drawing.Size(56, 24);
			this.btClear.TabIndex = 7;
			this.btClear.Text = "Clear";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(320, 237);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btClear,
																		  this.btCopy,
																		  this.lsKids,
																		  this.lsSelected,
																		  this.txName});
			this.Name = "Form1";
			this.Text = "Mediator Form";
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
