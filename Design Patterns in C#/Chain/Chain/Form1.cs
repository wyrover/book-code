using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Chain
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.PictureBox picImage;
		private System.Windows.Forms.ListBox lsFiles;
		private System.Windows.Forms.ListBox lsNocmd;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.TextBox txCommand;
		private System.Windows.Forms.Button btSend;
		private System.Windows.Forms.Panel pnlColor;
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
			//set up chains
			ColorChain clrChain = new ColorChain(pnlColor);
			FileChain flChain = new FileChain(lsFiles);
			NoCmd noChain = new NoCmd(lsNocmd);
			//create chain links
			chn = new ImageChain(picImage);
			chn.addToChain(clrChain);
			clrChain.addToChain(flChain);
			flChain.addToChain(noChain);
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
			this.picImage = new System.Windows.Forms.PictureBox();
			this.lsFiles = new System.Windows.Forms.ListBox();
			this.lsNocmd = new System.Windows.Forms.ListBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.txCommand = new System.Windows.Forms.TextBox();
			this.btSend = new System.Windows.Forms.Button();
			this.pnlColor = new System.Windows.Forms.Panel();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// picImage
			// 
			this.picImage.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.picImage.Location = new System.Drawing.Point(24, 8);
			this.picImage.Name = "picImage";
			this.picImage.Size = new System.Drawing.Size(152, 136);
			this.picImage.TabIndex = 0;
			this.picImage.TabStop = false;
			// 
			// lsFiles
			// 
			this.lsFiles.Location = new System.Drawing.Point(208, 8);
			this.lsFiles.Name = "lsFiles";
			this.lsFiles.Size = new System.Drawing.Size(104, 134);
			this.lsFiles.TabIndex = 1;
			// 
			// lsNocmd
			// 
			this.lsNocmd.Location = new System.Drawing.Point(344, 8);
			this.lsNocmd.Name = "lsNocmd";
			this.lsNocmd.Size = new System.Drawing.Size(104, 134);
			this.lsNocmd.TabIndex = 2;
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.btSend,
																					this.txCommand});
			this.groupBox1.Location = new System.Drawing.Point(24, 160);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(160, 96);
			this.groupBox1.TabIndex = 3;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Send commands";
			// 
			// txCommand
			// 
			this.txCommand.Location = new System.Drawing.Point(24, 24);
			this.txCommand.Name = "txCommand";
			this.txCommand.Size = new System.Drawing.Size(112, 20);
			this.txCommand.TabIndex = 0;
			this.txCommand.Text = "";
			// 
			// btSend
			// 
			this.btSend.Location = new System.Drawing.Point(40, 64);
			this.btSend.Name = "btSend";
			this.btSend.Size = new System.Drawing.Size(64, 24);
			this.btSend.TabIndex = 1;
			this.btSend.Text = "Send";
			this.btSend.Click += new System.EventHandler(this.btSend_Click);
			// 
			// pnlColor
			// 
			this.pnlColor.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.pnlColor.Location = new System.Drawing.Point(208, 160);
			this.pnlColor.Name = "pnlColor";
			this.pnlColor.Size = new System.Drawing.Size(112, 96);
			this.pnlColor.TabIndex = 4;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(464, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.pnlColor,
																		  this.groupBox1,
																		  this.lsNocmd,
																		  this.lsFiles,
																		  this.picImage});
			this.Name = "Form1";
			this.Text = "Chain demo";
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

		private void btSend_Click(object sender, System.EventArgs e) {
			chn.sendToChain (txCommand.Text );
		}
	}
}
