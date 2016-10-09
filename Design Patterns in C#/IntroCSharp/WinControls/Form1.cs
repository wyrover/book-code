using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace WinControls
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txBox;
		private System.Windows.Forms.Button btPush;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.CheckBox ckBold;
		private System.Windows.Forms.ListBox lsCommands;
		private System.Windows.Forms.ComboBox cbFont;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton opGreen;
		private System.Windows.Forms.RadioButton opRed;
		private System.Windows.Forms.Label lbText;
		private System.Windows.Forms.MenuItem mnuExit;
		private System.Windows.Forms.RadioButton opBlack;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.MenuItem mnuFile;
		private System.Windows.Forms.ToolTip tips;
		private int fontSize;
		public Form1() 		{
			InitializeComponent();
			init();
		}
		private void init() {
			fontSize = 12;
			cbFont.Items.Add ("8");
			cbFont.Items.Add ("10");
			cbFont.Items.Add ("12");
			cbFont.Items.Add ("14");
			cbFont.Items.Add ("18");
			lbText.Text ="Greetings";
			tips.SetToolTip (btPush, "Press to add text to list box");
			tips.SetToolTip (lsCommands, "Click to copy to text box");
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
			this.components = new System.ComponentModel.Container();
			this.opGreen = new System.Windows.Forms.RadioButton();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.opBlack = new System.Windows.Forms.RadioButton();
			this.opRed = new System.Windows.Forms.RadioButton();
			this.lsCommands = new System.Windows.Forms.ListBox();
			this.cbFont = new System.Windows.Forms.ComboBox();
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.mnuFile = new System.Windows.Forms.MenuItem();
			this.mnuExit = new System.Windows.Forms.MenuItem();
			this.ckBold = new System.Windows.Forms.CheckBox();
			this.txBox = new System.Windows.Forms.TextBox();
			this.btPush = new System.Windows.Forms.Button();
			this.lbText = new System.Windows.Forms.Label();
			this.tips = new System.Windows.Forms.ToolTip(this.components);
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// opGreen
			// 
			this.opGreen.Location = new System.Drawing.Point(16, 40);
			this.opGreen.Name = "opGreen";
			this.opGreen.Size = new System.Drawing.Size(72, 24);
			this.opGreen.TabIndex = 0;
			this.opGreen.Text = "Green";
			this.opGreen.CheckedChanged += new System.EventHandler(this.opGreen_CheckedChanged);
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.opBlack,
																					this.opRed,
																					this.opGreen});
			this.groupBox1.Location = new System.Drawing.Point(160, 120);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(104, 88);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Grouping";
			// 
			// opBlack
			// 
			this.opBlack.Checked = true;
			this.opBlack.Location = new System.Drawing.Point(16, 24);
			this.opBlack.Name = "opBlack";
			this.opBlack.Size = new System.Drawing.Size(64, 16);
			this.opBlack.TabIndex = 2;
			this.opBlack.TabStop = true;
			this.opBlack.Text = "Black";
			this.opBlack.CheckedChanged += new System.EventHandler(this.opBlack_CheckedChanged);
			// 
			// opRed
			// 
			this.opRed.Location = new System.Drawing.Point(16, 64);
			this.opRed.Name = "opRed";
			this.opRed.Size = new System.Drawing.Size(64, 16);
			this.opRed.TabIndex = 1;
			this.opRed.Text = "Red";
			this.opRed.CheckedChanged += new System.EventHandler(this.opRed_CheckedChanged);
			// 
			// lsCommands
			// 
			this.lsCommands.Location = new System.Drawing.Point(16, 120);
			this.lsCommands.Name = "lsCommands";
			this.lsCommands.Size = new System.Drawing.Size(112, 121);
			this.lsCommands.TabIndex = 3;
			this.lsCommands.SelectedIndexChanged += new System.EventHandler(this.lsCommands_SelectedIndexChanged);
			// 
			// cbFont
			// 
			this.cbFont.DropDownWidth = 112;
			this.cbFont.Location = new System.Drawing.Point(16, 88);
			this.cbFont.Name = "cbFont";
			this.cbFont.Size = new System.Drawing.Size(112, 21);
			this.cbFont.TabIndex = 4;
			this.cbFont.SelectedIndexChanged += new System.EventHandler(this.cbFont_SelectedIndexChanged);
			// 
			// mainMenu1
			// 
			this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					  this.mnuFile});
			// 
			// mnuFile
			// 
			this.mnuFile.Index = 0;
			this.mnuFile.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
																					this.mnuExit});
			this.mnuFile.Text = "File";
			// 
			// mnuExit
			// 
			this.mnuExit.Index = 0;
			this.mnuExit.Text = "Exit";
			this.mnuExit.Click += new System.EventHandler(this.menuItem2_Click);
			// 
			// ckBold
			// 
			this.ckBold.Location = new System.Drawing.Point(176, 72);
			this.ckBold.Name = "ckBold";
			this.ckBold.Size = new System.Drawing.Size(88, 16);
			this.ckBold.TabIndex = 2;
			this.ckBold.Text = "Bold";
			this.ckBold.CheckedChanged += new System.EventHandler(this.ckBold_CheckedChanged);
			// 
			// txBox
			// 
			this.txBox.Location = new System.Drawing.Point(8, 16);
			this.txBox.Name = "txBox";
			this.txBox.Size = new System.Drawing.Size(128, 20);
			this.txBox.TabIndex = 0;
			this.txBox.Text = "Greetings";
			// 
			// btPush
			// 
			this.btPush.Location = new System.Drawing.Point(160, 16);
			this.btPush.Name = "btPush";
			this.btPush.Size = new System.Drawing.Size(88, 24);
			this.btPush.TabIndex = 1;
			this.btPush.Text = "Push here";
			this.btPush.Click += new System.EventHandler(this.btPush_Click);
			// 
			// lbText
			// 
			this.lbText.FlatStyle = System.Windows.Forms.FlatStyle.Popup;
			this.lbText.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lbText.Location = new System.Drawing.Point(16, 48);
			this.lbText.Name = "lbText";
			this.lbText.Size = new System.Drawing.Size(120, 32);
			this.lbText.TabIndex = 6;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lbText,
																		  this.groupBox1,
																		  this.cbFont,
																		  this.lsCommands,
																		  this.ckBold,
																		  this.btPush,
																		  this.txBox});
			this.Menu = this.mainMenu1;
			this.Name = "Form1";
			this.Text = "Windows controls";
			this.groupBox1.ResumeLayout(false);
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

		private void btPush_Click(object sender, System.EventArgs e) {
			lsCommands.Items .Add  (txBox.Text +" "+cbFont.SelectedItem );
		}

		private void menuItem2_Click(object sender, System.EventArgs e) {
			this.Close ();
		}

		private void opGreen_CheckedChanged(object sender, System.EventArgs e) {
			lbText.ForeColor =Color.Green; 
		}

		private void opRed_CheckedChanged(object sender, System.EventArgs e) {
			lbText.ForeColor =Color.Red ;
		}

		private void opBlack_CheckedChanged(object sender, System.EventArgs e) {
			lbText.ForeColor =Color.Black ;
		}

		private void ckBold_CheckedChanged(object sender, System.EventArgs e) {
			if (ckBold.Checked ) {
				lbText.Font  =new Font ("Arial", fontSize,FontStyle.Bold );
			}
			else {
				lbText.Font = new Font ("Arial", fontSize);
			}
		}

		private void cbFont_SelectedIndexChanged(object sender, System.EventArgs e) {
			fontSize= Convert.ToInt16 (cbFont.SelectedItem );
			ckBold_CheckedChanged(null, null);
		}

		private void lsCommands_SelectedIndexChanged(object sender, System.EventArgs e) {
			ListBox.SelectedIndexCollection it = new ListBox.SelectedIndexCollection (lsCommands);
			ListBox.SelectedObjectCollection so = new ListBox.SelectedObjectCollection (lsCommands);
			txBox.Text = lsCommands.SelectedItem.ToString () ;
		}
	}
}
