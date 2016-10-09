using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using CsharpPats;

namespace SwimmerTokenizer
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsSwimmers;
		private System.Windows.Forms.TextBox txTime;
		private System.Windows.Forms.Label label1;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private ArrayList ar;
		//-------------------------
		private void init() {
			ar = new ArrayList ();	//create array list
			csFile fl = new csFile ("500free.txt");
			//read in liens
			string s =  fl.readLine ();
			while (s != null) {
				//convert to tokens in swimmer object
				Swimmer swm = new Swimmer(s);
				ar.Add (swm);
				s= fl.readLine ();
			}
			fl.close();
			//add names to list box
			for(int i=0; i < ar.Count ; i++) {
				Swimmer swm = (Swimmer)ar[i];
				lsSwimmers.Items.Add (swm.name );
			}
		}
		//-------------------------------
		public Form1()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			init();
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
			this.lsSwimmers = new System.Windows.Forms.ListBox();
			this.txTime = new System.Windows.Forms.TextBox();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(200, 24);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(88, 16);
			this.label1.TabIndex = 2;
			this.label1.Text = "Swimmer\'s time";
			// 
			// lsSwimmers
			// 
			this.lsSwimmers.Location = new System.Drawing.Point(8, 24);
			this.lsSwimmers.Name = "lsSwimmers";
			this.lsSwimmers.Size = new System.Drawing.Size(152, 173);
			this.lsSwimmers.TabIndex = 0;
			this.lsSwimmers.SelectedIndexChanged += new System.EventHandler(this.lsSwimmers_SelectedIndexChanged);
			// 
			// txTime
			// 
			this.txTime.Location = new System.Drawing.Point(192, 56);
			this.txTime.Name = "txTime";
			this.txTime.Size = new System.Drawing.Size(112, 20);
			this.txTime.TabIndex = 1;
			this.txTime.Text = "";
			this.txTime.TextChanged += new System.EventHandler(this.txTime_OnLostFocus);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(336, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.label1,
																		  this.txTime,
																		  this.lsSwimmers});
			this.Name = "Form1";
			this.Text = "Swimmer times";
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

		private void lsSwimmers_SelectedIndexChanged(object sender, System.EventArgs e) {
		   //get index of selected swimmer
			int i = lsSwimmers.SelectedIndex ;
			//get that swimmer
			Swimmer swm = (Swimmer)ar[i];
			//display her time
			txTime.Text =swm.time;
		}
		private void txTime_OnLostFocus(object sender, System.EventArgs e) {
			//get index of selected swimmer
			int i = lsSwimmers.SelectedIndex ;
			//get that swimmer
			Swimmer swm = (Swimmer)ar[i];
			swm.time =txTime.Text ;	
		}
		
	}
}
