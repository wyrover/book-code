using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace HelpChain
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form {
		private System.Windows.Forms.Button btNew;
		private System.Windows.Forms.Button btFile;
		private System.Windows.Forms.Button btQuit;
		private System.Windows.Forms.CheckBox ckBinary;
		private System.Windows.Forms.CheckBox ckText;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private Chain chn;

		public Form1()		{
			InitializeComponent();
			init();
		}
		private void init() {
			chn = new ControlChain(btNew, "Create new files");
			Chain fl =new ControlChain (btFile, "Select a file");
			chn.addToChain (fl);
			Chain bq = new ControlChain (btQuit, "Exit from program");
			fl.addToChain (bq);
			Chain cb =new ControlChain (ckBinary, "Use binary files");
			bq.addToChain (cb);
			Chain ct =  new ControlChain (ckText, "Use text files");
			cb.addToChain (ct);
			Chain ce = new EndChain (this, "General message");
			ct.addToChain (ce);
			KeyEventHandler keyev =  new KeyEventHandler(Form1_KeyDown);
			btNew.KeyDown += keyev;
			btFile.KeyDown += keyev;
			btQuit.KeyDown += keyev;
			ckBinary.KeyDown += keyev;
			ckText.KeyDown += keyev;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if (components != null) {
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
		private void InitializeComponent() {
			this.btFile = new System.Windows.Forms.Button();
			this.ckBinary = new System.Windows.Forms.CheckBox();
			this.btQuit = new System.Windows.Forms.Button();
			this.ckText = new System.Windows.Forms.CheckBox();
			this.btNew = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// btFile
			// 
			this.btFile.Location = new System.Drawing.Point(40, 64);
			this.btFile.Name = "btFile";
			this.btFile.Size = new System.Drawing.Size(64, 32);
			this.btFile.TabIndex = 1;
			this.btFile.Text = "File";
			// 
			// ckBinary
			// 
			this.ckBinary.Location = new System.Drawing.Point(168, 72);
			this.ckBinary.Name = "ckBinary";
			this.ckBinary.Size = new System.Drawing.Size(80, 16);
			this.ckBinary.TabIndex = 4;
			this.ckBinary.Text = "Binary files";
			// 
			// btQuit
			// 
			this.btQuit.Location = new System.Drawing.Point(40, 112);
			this.btQuit.Name = "btQuit";
			this.btQuit.Size = new System.Drawing.Size(64, 32);
			this.btQuit.TabIndex = 2;
			this.btQuit.Text = "Quit";
			this.btQuit.Click += new System.EventHandler(this.btQuit_Click);
			// 
			// ckText
			// 
			this.ckText.Location = new System.Drawing.Point(168, 32);
			this.ckText.Name = "ckText";
			this.ckText.Size = new System.Drawing.Size(80, 24);
			this.ckText.TabIndex = 3;
			this.ckText.Text = "Text files";
			// 
			// btNew
			// 
			this.btNew.Location = new System.Drawing.Point(40, 16);
			this.btNew.Name = "btNew";
			this.btNew.Size = new System.Drawing.Size(64, 32);
			this.btNew.TabIndex = 0;
			this.btNew.Text = "New";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(312, 165);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.ckText,
																		  this.ckBinary,
																		  this.btQuit,
																		  this.btFile,
																		  this.btNew});
			this.Name = "Form1";
			this.Text = "Help chain";
			this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			Application.Run(new Form1());
		}

		private void Form1_KeyDown(object sender, KeyEventArgs e) {
			if(e.KeyCode  == Keys.F1 )
				chn.sendToChain ();
		}

		private void btQuit_Click(object sender, System.EventArgs e) {
			this.Dispose();
		}
	}
}
