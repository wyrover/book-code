using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class WealthBuilder : Form 	{
		private ListBox lsEquities;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private Container components = null;
		private Button btPlot;
		private Panel pnl;
		private MultiChoice mchoice;
		private void init() {
			lsEquities.Items.Add (new Stocks());
			lsEquities.Items.Add (new Bonds());
			lsEquities.Items.Add (new Mutuals());
		}
		public WealthBuilder()
		{
			InitializeComponent();
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
		private void setPanel() {
     		pnl.Location = new Point(152, 24);
			pnl.Size = new Size(128, 168);
			pnl.TabIndex = 1;
			Controls.Add(pnl);
		}


		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.lsEquities = new System.Windows.Forms.ListBox();
			this.btPlot = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lsEquities
			// 
			this.lsEquities.Location = new System.Drawing.Point(24, 24);
			this.lsEquities.Name = "lsEquities";
			this.lsEquities.Size = new System.Drawing.Size(112, 134);
			this.lsEquities.TabIndex = 0;
			this.lsEquities.SelectedIndexChanged += new System.EventHandler(this.lsEquities_SelectedIndexChanged);
			// 
			// btPlot
			// 
			this.btPlot.Location = new System.Drawing.Point(184, 208);
			this.btPlot.Name = "btPlot";
			this.btPlot.Size = new System.Drawing.Size(64, 24);
			this.btPlot.TabIndex = 1;
			this.btPlot.Text = "Plot";
			this.btPlot.Click += new System.EventHandler(this.btPlot_Click);
			// 
			// WealthBuilder
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(312, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btPlot,
																		  this.lsEquities});
			this.Name = "WealthBuilder";
			this.Text = "Form1";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new WealthBuilder());
		}

		private void lsEquities_SelectedIndexChanged(object sender, System.EventArgs e) {
			int i = lsEquities.SelectedIndex ;
			Equities eq = (Equities)lsEquities.Items[i];
			mchoice= StockFactory.getBuilder (eq);
			this.Controls.Remove (pnl);
			pnl = mchoice.getWindow ();
			setPanel();
		}

		private void btPlot_Click(object sender, System.EventArgs e) {
			//display the selected items in a message box
			if(mchoice != null) {
				ArrayList ar  = mchoice.getSelected ();
				string ans = "";
				for(int i=0; i< ar.Count ; i++) {
					ans += (string)ar[i] +" ";
				}
				MessageBox.Show (null, ans, "Selected equities", MessageBoxButtons.OK  );
			}
		}
	}
