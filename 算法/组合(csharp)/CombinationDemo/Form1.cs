using System;
using System.Text;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using CombinationsLib;

namespace CombinationDemo
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.ListBox lbCombinations;
		private System.Windows.Forms.TextBox txtCaseInputs;
		private System.Windows.Forms.TextBox txtTotalItems;
		private System.Windows.Forms.TextBox txtSubsetSize;
		private System.Windows.Forms.TextBox txtNumCombinations;
		private System.Windows.Forms.Button btnGenerateCombinations;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			InitializeComponent();
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
			this.lbCombinations = new System.Windows.Forms.ListBox();
			this.btnGenerateCombinations = new System.Windows.Forms.Button();
			this.txtCaseInputs = new System.Windows.Forms.TextBox();
			this.txtTotalItems = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.txtSubsetSize = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.txtNumCombinations = new System.Windows.Forms.TextBox();
			this.label4 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// lbCombinations
			// 
			this.lbCombinations.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lbCombinations.ItemHeight = 14;
			this.lbCombinations.Location = new System.Drawing.Point(8, 216);
			this.lbCombinations.Name = "lbCombinations";
			this.lbCombinations.Size = new System.Drawing.Size(248, 214);
			this.lbCombinations.TabIndex = 0;
			// 
			// btnGenerateCombinations
			// 
			this.btnGenerateCombinations.Location = new System.Drawing.Point(8, 184);
			this.btnGenerateCombinations.Name = "btnGenerateCombinations";
			this.btnGenerateCombinations.Size = new System.Drawing.Size(136, 23);
			this.btnGenerateCombinations.TabIndex = 2;
			this.btnGenerateCombinations.Text = "Generate Combinations";
			this.btnGenerateCombinations.Click += new System.EventHandler(this.btnGenerateCombinations_Click);
			// 
			// txtCaseInputs
			// 
			this.txtCaseInputs.Location = new System.Drawing.Point(8, 32);
			this.txtCaseInputs.Multiline = true;
			this.txtCaseInputs.Name = "txtCaseInputs";
			this.txtCaseInputs.Size = new System.Drawing.Size(128, 80);
			this.txtCaseInputs.TabIndex = 3;
			this.txtCaseInputs.Text = "";
			this.txtCaseInputs.TextChanged += new System.EventHandler(this.txtCaseInputs_TextChanged);
			// 
			// txtTotalItems
			// 
			this.txtTotalItems.Location = new System.Drawing.Point(144, 32);
			this.txtTotalItems.Name = "txtTotalItems";
			this.txtTotalItems.ReadOnly = true;
			this.txtTotalItems.Size = new System.Drawing.Size(32, 20);
			this.txtTotalItems.TabIndex = 4;
			this.txtTotalItems.Text = "";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 8);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(128, 23);
			this.label1.TabIndex = 5;
			this.label1.Text = "Enter test case inputs:";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(184, 32);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(80, 23);
			this.label2.TabIndex = 6;
			this.label2.Text = "Total items (n)";
			// 
			// txtSubsetSize
			// 
			this.txtSubsetSize.Location = new System.Drawing.Point(144, 128);
			this.txtSubsetSize.Name = "txtSubsetSize";
			this.txtSubsetSize.Size = new System.Drawing.Size(32, 20);
			this.txtSubsetSize.TabIndex = 7;
			this.txtSubsetSize.Text = "";
			this.txtSubsetSize.TextChanged += new System.EventHandler(this.txtSubsetSize_TextChanged);
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 128);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(120, 23);
			this.label3.TabIndex = 8;
			this.label3.Text = "Enter subset size (k):";
			// 
			// txtNumCombinations
			// 
			this.txtNumCombinations.Location = new System.Drawing.Point(184, 152);
			this.txtNumCombinations.Name = "txtNumCombinations";
			this.txtNumCombinations.Size = new System.Drawing.Size(40, 20);
			this.txtNumCombinations.TabIndex = 9;
			this.txtNumCombinations.Text = "";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 152);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(176, 23);
			this.label4.TabIndex = 11;
			this.label4.Text = "Number of combinations will be =";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(264, 437);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.txtNumCombinations);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.txtSubsetSize);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.txtTotalItems);
			this.Controls.Add(this.txtCaseInputs);
			this.Controls.Add(this.btnGenerateCombinations);
			this.Controls.Add(this.lbCombinations);
			this.Name = "Form1";
			this.Text = "Combinations Demo";
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

		private void btnGenerateCombinations_Click(object sender, System.EventArgs e)
		{
			lbCombinations.Items.Clear();

			int n = txtCaseInputs.Lines.Length;
			int k = int.Parse(txtSubsetSize.Text);
			Combination c = new Combination(n,k);
      
			string[] result = new string[k];

			while (c != null)
			{
				result = c.ApplyTo(txtCaseInputs.Lines);

				StringBuilder sb = new StringBuilder();
				for (int i = 0; i < result.Length; ++i)
				{
					sb.AppendFormat("{0} {1}", result[i], " ");
				}
				lbCombinations.Items.Add(sb.ToString());

				c = c.Successor();
			}
		}

		private void txtCaseInputs_TextChanged(object sender, System.EventArgs e)
		{
			txtTotalItems.Text = (txtCaseInputs.Lines.Length).ToString();
		}

		private void txtSubsetSize_TextChanged(object sender, System.EventArgs e)
		{
			long n = int.Parse(txtTotalItems.Text);
			long k = int.Parse(txtSubsetSize.Text);
			txtNumCombinations.Text = Combination.Choose(n,k).ToString();
		}
	}
}
