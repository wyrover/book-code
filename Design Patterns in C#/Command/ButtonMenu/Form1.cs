using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace ButtonMenu
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private ButtonMenu.RedButton btRed;
		private System.Windows.Forms.MainMenu mainMenu1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1() 		{
			InitializeComponent();
			init();
		}
		private void init() {
			//create a main menu and install it
			MainMenu main = new MainMenu();
			this.Menu =main;
			
			//create a click-event handler
			EventHandler evh = new EventHandler (commandClick);
			btRed.Click += evh;		//add to existing red button

			//create a "File" top level entry
			MenuItem file = new MenuItem("File");

			//create File Open command
			FileOpen mnflo = new FileOpen ();
			mnflo.Click += evh;		//add same handler
			main.MenuItems.Add ( file );

			//create a File-Exit command
			FileExit fex = new FileExit(this);
			file.MenuItems.AddRange( new MenuItem[]{ mnflo, fex});	
			fex.Click += evh;		//add same handler
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
			this.btRed = new ButtonMenu.RedButton();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.SuspendLayout();
			// 
			// btRed
			// 
			this.btRed.Location = new System.Drawing.Point(88, 48);
			this.btRed.Name = "btRed";
			this.btRed.Size = new System.Drawing.Size(75, 32);
			this.btRed.TabIndex = 0;
			this.btRed.Text = "Red";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(272, 141);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btRed});
			this.Menu = this.mainMenu1;
			this.Name = "Form1";
			this.Text = "Command demo";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 		{
			Application.Run(new Form1());
		}

		private void commandClick(object sender, System.EventArgs e) {
			Command comd = (Command)sender;
			comd.Execute ();
		}
	}
}
