using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace State
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox pic;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private CircleButton circButton;
		private RectButton rctButton;
		private PickButton arowButton ;
		private FillButton flButton;
		private UndoButton undoB;
		private ClearButton clrb;
		private System.Windows.Forms.ToolBar tBar;
		private Mediator med;
		//-----
		public Form1()
		{
			InitializeComponent();
			init();
		}
		//-----
		private void init() {
			//create a Mediator
		    med = new Mediator(pic);
			//create the buttons
			rctButton = new RectButton(med);
			arowButton = new PickButton(med);
			circButton = new CircleButton(med);
			flButton = new FillButton(med);
			undoB = new UndoButton(med);
			clrb = new ClearButton(med);
			//add the buttons into the toolbar
			tBar.Buttons.Add(arowButton);
			tBar.Buttons.Add(rctButton);
			tBar.Buttons.Add(circButton);
			tBar.Buttons.Add(flButton);
			//include a separator
			ToolBarButton sep =new ToolBarButton();
			sep.Style = ToolBarButtonStyle.Separator;
			tBar.Buttons.Add(sep);
			tBar.Buttons.Add(undoB);
			tBar.Buttons.Add(clrb);
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
			this.tBar = new System.Windows.Forms.ToolBar();
			this.SuspendLayout();
			// 
			// pic
			// 
			this.pic.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
			this.pic.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.pic.Location = new System.Drawing.Point(16, 48);
			this.pic.Name = "pic";
			this.pic.Size = new System.Drawing.Size(264, 200);
			this.pic.TabIndex = 1;
			this.pic.TabStop = false;
			this.pic.Paint += new System.Windows.Forms.PaintEventHandler(this.pic_Paint);
			this.pic.MouseUp += new System.Windows.Forms.MouseEventHandler(this.pic_MouseUp);
			this.pic.MouseMove += new System.Windows.Forms.MouseEventHandler(this.pic_MouseMove);
			this.pic.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pic_MouseDown);
			// 
			// tBar
			// 
			this.tBar.DropDownArrows = true;
			this.tBar.Name = "tBar";
			this.tBar.ShowToolTips = true;
			this.tBar.Size = new System.Drawing.Size(292, 22);
			this.tBar.TabIndex = 0;
			this.tBar.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.tBar_ButtonClick);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.pic,
																		  this.tBar});
			this.Name = "Form1";
			this.Text = "State demo";
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

		private void tBar_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e) {
			Command comd = (Command)e.Button ;
			comd.Execute ();
		}

		private void pic_MouseDown(object sender, System.Windows.Forms.MouseEventArgs e) {
		  med.mouseDown (e.X, e.Y);
		}

		private void pic_MouseUp(object sender, System.Windows.Forms.MouseEventArgs e) {
			med.mouseUp (e.X, e.Y);
		}

		private void pic_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e) {
			if(e.Button != MouseButtons.None ) {
				med.mouseDrag (e.X, e.Y);
			}
		}

		private void pic_Paint(object sender, System.Windows.Forms.PaintEventArgs e) {
			Graphics g = e.Graphics ;
			med.reDraw (g);
		}
	}
}
