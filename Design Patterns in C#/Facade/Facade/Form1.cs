using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Facade
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsFoods;
		private System.Windows.Forms.ListBox lsPrices;
		private System.Windows.Forms.Button btLoad;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private DBase db;
		private Stores shops;
		private Prices prc;
		//-----------
		private void init() {
			 db = new AxsDatabase("Groceries.mdb");
        shops = new Stores(db);
        prc = new Prices(db);
        loadFoodTable();
        ToolTip tips = new ToolTip();
        tips.SetToolTip(btLoad, "Reload data from groceries.txt file");
		}
		public Form1()
		{
			InitializeComponent();
			init();
		}
		private void loadFoodTable() {
			 Foods fods =new Foods(db);
        fods.openTable();
        while (fods.hasMoreElements()){
			lsFoods.Items.Add(fods.getValue());
			}
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
			this.btLoad = new System.Windows.Forms.Button();
			this.lsPrices = new System.Windows.Forms.ListBox();
			this.lsFoods = new System.Windows.Forms.ListBox();
			this.SuspendLayout();
			// 
			// btLoad
			// 
			this.btLoad.Location = new System.Drawing.Point(32, 216);
			this.btLoad.Name = "btLoad";
			this.btLoad.Size = new System.Drawing.Size(64, 24);
			this.btLoad.TabIndex = 2;
			this.btLoad.Text = "Load";
			this.btLoad.Click += new System.EventHandler(this.btLoad_Click);
			// 
			// lsPrices
			// 
			this.lsPrices.Location = new System.Drawing.Point(200, 24);
			this.lsPrices.Name = "lsPrices";
			this.lsPrices.Size = new System.Drawing.Size(136, 173);
			this.lsPrices.TabIndex = 1;
			// 
			// lsFoods
			// 
			this.lsFoods.Location = new System.Drawing.Point(16, 24);
			this.lsFoods.Name = "lsFoods";
			this.lsFoods.Size = new System.Drawing.Size(120, 173);
			this.lsFoods.TabIndex = 0;
			this.lsFoods.SelectedIndexChanged += new System.EventHandler(this.lsFoods_SelectedIndexChanged);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(360, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btLoad,
																		  this.lsPrices,
																		  this.lsFoods});
			this.Name = "Form1";
			this.Text = "Facade Demo";
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

		private void btLoad_Click(object sender, System.EventArgs e) {
			 lsFoods.Items.Clear();
        Cursor.Current = Cursors.WaitCursor;
        DataLoader dload = new DataLoader(db);
        dload.load("groceries.txt");
        loadFoodTable();
        Cursor.Current = Cursors.Default;
		}

		private void lsFoods_SelectedIndexChanged(object sender, System.EventArgs e) {
			 string food  = lsFoods.Text;
        DataTable dtable = prc.getPrices(food);

        lsPrices.Items.Clear();
        foreach (DataRow rw in dtable.Rows) {
            lsPrices.Items.Add(rw["StoreName"].ToString().Trim() +
				"\t" + rw["Price"].ToString());
			}
		}
	}
}
