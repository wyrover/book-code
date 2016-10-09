using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using CsharpPats;

namespace TokenDemo
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.TextBox txEntry;
		private System.Windows.Forms.Button btToken;
		private System.Windows.Forms.ListBox lsTokens;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			this.label1 = new System.Windows.Forms.Label();
			this.txEntry = new System.Windows.Forms.TextBox();
			this.btToken = new System.Windows.Forms.Button();
			this.lsTokens = new System.Windows.Forms.ListBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(0, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(184, 24);
			this.label1.TabIndex = 0;
			this.label1.Text = "Enter string to tokenize";
			// 
			// txEntry
			// 
			this.txEntry.Location = new System.Drawing.Point(8, 40);
			this.txEntry.Name = "txEntry";
			this.txEntry.Size = new System.Drawing.Size(344, 20);
			this.txEntry.TabIndex = 1;
			this.txEntry.Text = "";
			// 
			// btToken
			// 
			this.btToken.Location = new System.Drawing.Point(24, 88);
			this.btToken.Name = "btToken";
			this.btToken.Size = new System.Drawing.Size(88, 32);
			this.btToken.TabIndex = 2;
			this.btToken.Text = "Tokenize";
			this.btToken.Click += new System.EventHandler(this.btToken_Click);
			// 
			// lsTokens
			// 
			this.lsTokens.Location = new System.Drawing.Point(152, 88);
			this.lsTokens.Name = "lsTokens";
			this.lsTokens.Size = new System.Drawing.Size(168, 147);
			this.lsTokens.TabIndex = 3;
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(352, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lsTokens,
																		  this.btToken,
																		  this.txEntry,
																		  this.label1});
			this.Name = "Form1";
			this.Text = "Tokenizer demo";
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
	//call tokenizer when button is clicked
		private void btToken_Click(object sender, System.EventArgs e) {
			StringTokenizer tok = new StringTokenizer (txEntry.Text );
			while(tok.hasMoreElements () ) {
				lsTokens.Items.Add (tok.nextElement());
			}
		}
	}
}
