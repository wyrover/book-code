using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using CsharpPats;

namespace BasicBridge
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.Windows.Forms.ListBox lsProd;
		private System.Windows.Forms.DataGrid grdProd;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private ArrayList products;
		private VisList prodList, gridList;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
	
		public Form1() 		{
			InitializeComponent();
			init();
		}
		private void init() {
			products = new ArrayList ();
			readFile(products);	//read in the data file
			//create the product list 
			prodList = new ProductList (lsProd);
			//Bridge to product VisList
			Bridger lbr = new SortBridge (prodList);
			//put the data into the product list
			lbr.addData (products);
			//create the grid VisList
			gridList = new GridList(grdProd);
			//Bridge to the grid list
			Bridger gbr = new SortBridge (gridList);
			//put the data into the grid display
			gbr.addData (products);
		}
		private void readFile(ArrayList products) {
			csFile fl = new csFile("products.txt");
			fl.OpenForRead ();
			string s = fl.readLine ();
			while(s != null ) {
				Product p = new Product(s);
				products.Add (p);
				s = fl.readLine ();
			}
		fl.close();
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
			this.grdProd = new System.Windows.Forms.DataGrid();
			this.lsProd = new System.Windows.Forms.ListBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			((System.ComponentModel.ISupportInitialize)(this.grdProd)).BeginInit();
			this.SuspendLayout();
			// 
			// grdProd
			// 
			this.grdProd.CaptionVisible = false;
			this.grdProd.DataMember = "";
			this.grdProd.Location = new System.Drawing.Point(184, 40);
			this.grdProd.Name = "grdProd";
			this.grdProd.RowHeadersVisible = false;
			this.grdProd.Size = new System.Drawing.Size(160, 176);
			this.grdProd.TabIndex = 1;
			// 
			// lsProd
			// 
			this.lsProd.Location = new System.Drawing.Point(16, 40);
			this.lsProd.Name = "lsProd";
			this.lsProd.Size = new System.Drawing.Size(128, 173);
			this.lsProd.TabIndex = 0;
			// 
			// label1
			// 
			this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label1.ForeColor = System.Drawing.Color.Navy;
			this.label1.Location = new System.Drawing.Point(16, 16);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(112, 16);
			this.label1.TabIndex = 2;
			this.label1.Text = "Customer view";
			// 
			// label2
			// 
			this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.label2.ForeColor = System.Drawing.Color.Navy;
			this.label2.Location = new System.Drawing.Point(192, 16);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(136, 16);
			this.label2.TabIndex = 3;
			this.label2.Text = "Executive view";
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(352, 229);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.label2,
																		  this.label1,
																		  this.grdProd,
																		  this.lsProd});
			this.Name = "Form1";
			this.Text = "Sorted bridge";
			((System.ComponentModel.ISupportInitialize)(this.grdProd)).EndInit();
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
	}
}
