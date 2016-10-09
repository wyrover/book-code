using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace CHolder
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private ComdButton btRed;
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
			//note: btRed was added in the IDE
			EventHandler evh = new EventHandler (commandClick);
			btRed.Click += evh;		//add to existing red button
			RedCommand cRed = new RedCommand (this);
			btRed.setCommand (cRed);

			//create a "File" top level entry
			MenuItem file = new CommandMenu("File");
			main.MenuItems.Add ( file );
			//create File Open command
			CommandMenu mnuFlo = new CommandMenu("Open");
			mnuFlo.setCommand (new OpenCommand ());
			mnuFlo.Click += evh;		//add same handler
			
			//create a Red menu item, too
			CommandMenu mnuRed = new CommandMenu("Red");
			mnuRed.setCommand(cRed);
			mnuRed.Click += evh;		//add same handler
			
			//create a File-Exit command
			CommandMenu mnuFex = new CommandMenu("Exit");
			mnuFex.setCommand (new ExitCommand(this));
			file.MenuItems.AddRange( new CommandMenu[]{ mnuFlo, mnuRed, mnuFex});	
			mnuFex.Click += evh;		//add same handler
		}
		private void commandClick(object sender, EventArgs e) {
			Command comd = ((CommandHolder)sender).getCommand ();
			comd.Execute ();
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
			this.btRed = new ComdButton();
			this.SuspendLayout();
			// 
			// btRed
			// 
			this.btRed.Location = new System.Drawing.Point(104, 64);
			this.btRed.Name = "btRed";
			this.btRed.Size = new System.Drawing.Size(72, 32);
			this.btRed.TabIndex = 0;
			this.btRed.Text = "Red";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(280, 173);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btRed});
			this.Name = "Form1";
			this.Text = "Command Holder demo";
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
