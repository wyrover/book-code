using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace CalcTemp
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class TempCalc : System.Windows.Forms.Form
	{
		private System.Windows.Forms.TextBox txEntry;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Label lbResult;
		private System.Windows.Forms.RadioButton opCels;
		private System.Windows.Forms.RadioButton opFahr;
		private System.Windows.Forms.Button btCompute;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public TempCalc()
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
			this.opFahr = new System.Windows.Forms.RadioButton();
			this.lbResult = new System.Windows.Forms.Label();
			this.btCompute = new System.Windows.Forms.Button();
			this.txEntry = new System.Windows.Forms.TextBox();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.opCels = new System.Windows.Forms.RadioButton();
			this.label1 = new System.Windows.Forms.Label();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// opFahr
			// 
			this.opFahr.Location = new System.Drawing.Point(24, 40);
			this.opFahr.Name = "opFahr";
			this.opFahr.Size = new System.Drawing.Size(104, 16);
			this.opFahr.TabIndex = 1;
			this.opFahr.Text = "to Fahrenheit";
			// 
			// lbResult
			// 
			this.lbResult.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lbResult.Location = new System.Drawing.Point(64, 72);
			this.lbResult.Name = "lbResult";
			this.lbResult.Size = new System.Drawing.Size(136, 24);
			this.lbResult.TabIndex = 3;
			// 
			// btCompute
			// 
			this.btCompute.Location = new System.Drawing.Point(96, 208);
			this.btCompute.Name = "btCompute";
			this.btCompute.Size = new System.Drawing.Size(72, 24);
			this.btCompute.TabIndex = 4;
			this.btCompute.Text = "Compute";
			this.btCompute.Click += new System.EventHandler(this.btCompute_Click);
			// 
			// txEntry
			// 
			this.txEntry.Location = new System.Drawing.Point(152, 32);
			this.txEntry.Name = "txEntry";
			this.txEntry.Size = new System.Drawing.Size(96, 20);
			this.txEntry.TabIndex = 0;
			this.txEntry.Text = "";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.opFahr,
																					this.opCels});
			this.groupBox1.Location = new System.Drawing.Point(56, 112);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(160, 72);
			this.groupBox1.TabIndex = 2;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Select conversion";
			// 
			// opCels
			// 
			this.opCels.Location = new System.Drawing.Point(24, 16);
			this.opCels.Name = "opCels";
			this.opCels.Size = new System.Drawing.Size(112, 16);
			this.opCels.TabIndex = 0;
			this.opCels.Text = "to Celsius";
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label1.ForeColor = System.Drawing.Color.Blue;
			this.label1.Location = new System.Drawing.Point(8, 32);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(112, 32);
			this.label1.TabIndex = 1;
			this.label1.Text = "Enter temperature";
			// 
			// TempCalc
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btCompute,
																		  this.lbResult,
																		  this.groupBox1,
																		  this.label1,
																		  this.txEntry});
			this.Name = "TempCalc";
			this.Text = "Temperature conversion";
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
			Application.Run(new TempCalc());
		}

		private void btCompute_Click(object sender, System.EventArgs e) {
			string newTemp;
			//use input value to create instance of class
			Temperature temp = new Temperature (txEntry.Text );
			//use radio button to decide which conversion
			newTemp = temp.getConvTemp (opCels.Checked );
			
			//get result and put in label text
			lbResult.Text =newTemp.ToString ();
			txEntry.Text ="";	//clear entry field
		}
	}
}
