using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Visitor
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private Employee[] empls;
		private Employee empl ;
		private System.Windows.Forms.Button btCompute;
		private System.Windows.Forms.ListBox lsVac;
		private Boss bos;

		public Form1()
		{
			InitializeComponent();
			init();
		}
		private void init() {
			int i = 0;
			empls = new Employee [7];
			empls[i++] = new Employee("Susan Bear", 55000, 12, 1);
            empls[i++] = new Employee("Adam Gehr", 150000, 9, 0);
            empls[i++] = new Employee("Fred Harris", 50000, 15, 2);
	        empls[i++] = new Employee("David Oakley", 57000, 12, 2);
            empls[i++] = new Employee("Larry Thomas", 100000, 20, 6);
            bos = new Boss("Leslie Susi", 175000, 16, 4);
			bos.setBonusDays(12);
			empls[i++] = bos;
            bos = new Boss("Laurence Byerly", 35000, 17, 6);
			bos.setBonusDays(17);
			empls[i++] = bos;
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
			this.lsVac = new System.Windows.Forms.ListBox();
			this.btCompute = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// lsVac
			// 
			this.lsVac.Location = new System.Drawing.Point(32, 32);
			this.lsVac.Name = "lsVac";
			this.lsVac.Size = new System.Drawing.Size(216, 186);
			this.lsVac.TabIndex = 0;
			// 
			// btCompute
			// 
			this.btCompute.Location = new System.Drawing.Point(88, 232);
			this.btCompute.Name = "btCompute";
			this.btCompute.Size = new System.Drawing.Size(72, 24);
			this.btCompute.TabIndex = 1;
			this.btCompute.Text = "Compute";
			this.btCompute.Click += new System.EventHandler(this.btCompute_Click);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.btCompute,
																		  this.lsVac});
			this.Name = "Form1";
			this.Text = "Visitor demo";
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

		private void btCompute_Click(object sender, System.EventArgs e) {
			VacationVisitor vac = new VacationVisitor();
			bVacationVisitor bvac = new bVacationVisitor();
			for (int i = 0;  i< empls.Length; i++) {
				empls[i].accept(vac);      //get the employee
				empls[i].accept(bvac);
			}
        lsVac.Items.Add("Total vacation days=" + vac.getTotalDays().ToString());
        lsVac.Items.Add("Total boss vacation days=" + bvac.getTotalDays().ToString());

		}
	}
}
