using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Memento
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox pic;
		private System.Windows.Forms.ToolBar tbar;
		private System.Windows.Forms.ToolBarButton btRect;
		private System.Windows.Forms.ToolBarButton btUndo;
		private System.Windows.Forms.ToolBarButton btClear;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private bool mouse_down;
		private Mediator med;
		private Hashtable commands;
		//-----
		public Form1() 		{
			InitializeComponent();
			init();						
		}
		//-----
		private void init() {
			med = new Mediator(pic);     //create Mediator
			commands = new Hashtable();  //and Hash table
			//create the command objectsb
			RectButton rbutn = new RectButton(med, tbar.Buttons[0]);
			UndoButton ubutn = new UndoButton(med, tbar.Buttons[1]);
			ClrButton clrbutn = new ClrButton(med);
			med.registerRectButton (rbutn);
			//add them to the hashtable using the button hash values
			commands.Add(btRect.GetHashCode(), rbutn);
			commands.Add(btUndo.GetHashCode(), ubutn);
			commands.Add(btClear.GetHashCode(), clrbutn);
			pic.Paint += new PaintEventHandler (paintHandler);
		}
		private void paintHandler(object sender, PaintEventArgs e ) {
			Graphics g =  e.Graphics ;
			med.reDraw (g);
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
			this.pic = new System.Windows.Forms.PictureBox();
			this.tbar = new System.Windows.Forms.ToolBar();
			this.btRect = new System.Windows.Forms.ToolBarButton();
			this.btUndo = new System.Windows.Forms.ToolBarButton();
			this.btClear = new System.Windows.Forms.ToolBarButton();
			this.SuspendLayout();
			// 
			// pic
			// 
			this.pic.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.pic.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pic.Location = new System.Drawing.Point(16, 56);
			this.pic.Name = "pic";
			this.pic.Size = new System.Drawing.Size(256, 192);
			this.pic.TabIndex = 0;
			this.pic.TabStop = false;
			this.pic.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pic_MouseUp);
			this.pic.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pic_MouseMove);
			this.pic.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pic_MouseDown);
			// 
			// tbar
			// 
			this.tbar.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
																					this.btRect,
																					this.btUndo,
																					this.btClear});
			this.tbar.DropDownArrows = true;
			this.tbar.Name = "tbar";
			this.tbar.ShowToolTips = true;
			this.tbar.Size = new System.Drawing.Size(292, 39);
			this.tbar.TabIndex = 1;
			this.tbar.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.tbar_ButtonClick);
			// 
			// btRect
			// 
			this.btRect.Style = System.Windows.Forms.ToolBarButtonStyle.ToggleButton;
			this.btRect.Text = "Rectangle";
			// 
			// btUndo
			// 
			this.btUndo.Text = "Undo";
			// 
			// btClear
			// 
			this.btClear.Text = "Clear";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.tbar,
																		  this.pic});
			this.Name = "Form1";
			this.Text = "Memento demo";
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

		private void tbar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e) {
			ToolBarButton tbutn = e.Button ;
			Command comd = (Command)commands[tbutn.GetHashCode ()];
			comd.Execute ();
		}
		//------
		private void pic_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e) {
			mouse_down = true;
			med.createRect (e.X, e.Y);
		}
		//------
		private void pic_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e) {
			mouse_down = false;
		}
		//------
		private void pic_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e) {
			if(mouse_down)
				med.drag(e.X , e.Y);
		}
	}
}
