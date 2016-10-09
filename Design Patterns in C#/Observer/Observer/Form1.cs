using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Observer
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form, Subject
	{
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.RadioButton opRed;
		private System.Windows.Forms.RadioButton opGreen;
		private System.Windows.Forms.RadioButton opBlue;
		private ArrayList observers;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public Form1()
		{
			InitializeComponent();
			init();
		}
		private void init() {
			EventHandler evh = new EventHandler (opButton_Click);
			opRed.Click += evh;
			opBlue.Click += evh;
			opGreen.Click += evh;
			observers =  new ArrayList ();
			ListObs lobs = new ListObs (this);
			lobs.Show ();
			ColObserver colObs = new ColObserver (this);
			colObs.Show();
		}
		public void registerInterest(Observer obs ) {
			observers.Add (obs);
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
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.opRed = new System.Windows.Forms.RadioButton();
			this.opGreen = new System.Windows.Forms.RadioButton();
			this.opBlue = new System.Windows.Forms.RadioButton();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.AddRange(new System.Windows.Forms.Control[] {
																					this.opBlue,
																					this.opGreen,
																					this.opRed});
			this.groupBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.groupBox1.Location = new System.Drawing.Point(32, 24);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(160, 144);
			this.groupBox1.TabIndex = 0;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Select colors";
			// 
			// opRed
			// 
			this.opRed.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.opRed.ForeColor = System.Drawing.Color.Red;
			this.opRed.Location = new System.Drawing.Point(32, 32);
			this.opRed.Name = "opRed";
			this.opRed.TabIndex = 0;
			this.opRed.Text = "Red";
			
			// 
			// opGreen
			// 
			this.opGreen.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.opGreen.ForeColor = System.Drawing.Color.FromArgb(((System.Byte)(0)), ((System.Byte)(192)), ((System.Byte)(0)));
			this.opGreen.Location = new System.Drawing.Point(32, 64);
			this.opGreen.Name = "opGreen";
			this.opGreen.Size = new System.Drawing.Size(96, 16);
			this.opGreen.TabIndex = 1;
			this.opGreen.Text = "Green";
			// 
			// opBlue
			// 
			this.opBlue.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.opBlue.ForeColor = System.Drawing.Color.Blue;
			this.opBlue.Location = new System.Drawing.Point(32, 88);
			this.opBlue.Name = "opBlue";
			this.opBlue.Size = new System.Drawing.Size(80, 16);
			this.opBlue.TabIndex = 2;
			this.opBlue.Text = "Blue";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(224, 205);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.groupBox1});
			this.Name = "Form1";
			this.Text = "Observer demo";
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

		private void opButton_Click(object sender, System.EventArgs e) {
			RadioButton but = (RadioButton)sender;
			for(int i=0; i< observers.Count ; i++ ) {
				Observer obs = (Observer)observers[i];
				obs.sendNotify (but.Text );
			}
		}
	}
}
