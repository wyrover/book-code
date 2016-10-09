using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace Composite
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class Form1 : System.Windows.Forms.Form {
		private System.Windows.Forms.Label lbSalary;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		AbstractEmployee prez, marketVP, salesMgr;
		TreeNode rootNode;
		AbstractEmployee advMgr, emp, prodVP, prodMgr, shipMgr;
		private System.Windows.Forms.TreeView EmpTree;
		private Random rand;
	
		private void init() {
			rand = new Random ();
			buildEmployeeList();
			buildTree();
		}
		//---------------
		private void buildEmployeeList() {
			prez = new Boss("CEO", 200000);
			marketVP = new Boss("Marketing VP", 100000);
			prez.add(marketVP);
			salesMgr = new Boss("Sales Mgr", 50000);
			advMgr = new Boss("Advt Mgr", 50000);
			marketVP.add(salesMgr);
			marketVP.add(advMgr);
			prodVP = new Boss("Production VP", 100000);
			prez.add(prodVP);
			advMgr.add("Secy", 20000);

			//add salesmen reporting to sales manager
			for (int i = 1; i<=5; i++){
				salesMgr.add("Sales" + i.ToString(), rand_sal(30000));
			}

			prodMgr = new Boss("Prod Mgr", 40000);
			shipMgr = new Boss("Ship Mgr", 35000);
			prodVP.add(prodMgr);
			prodVP.add(shipMgr);

			for (int i = 1; i<=3; i++){
				shipMgr.add("Ship" + i.ToString(), rand_sal(25000));
			}
			for (int i = 1; i<=4; i++){
				prodMgr.add("Manuf" + i.ToString(), rand_sal(20000));
			}
		}
		//-----
		private void buildTree() {
			EmpNode nod;

			nod = new EmpNode(prez);
			rootNode = nod;
			EmpTree.Nodes.Add(nod);
			addNodes(nod, prez);
		}
		
		//------
		private void getNodeSum(EmpNode node) {
			AbstractEmployee emp;
			float sum;

        emp = node.getEmployee();
        sum = emp.getSalaries();
        lbSalary.Text = sum.ToString ();
		}
		//------
		private void addNodes(EmpNode nod, AbstractEmployee emp) {
		AbstractEmployee newEmp;
        EmpNode newNode;
        IEnumerator empEnum;
        empEnum = emp.getSubordinates();

        while (empEnum.MoveNext()) {
            newEmp = (AbstractEmployee)empEnum.Current;
            newNode = new EmpNode(newEmp);
            nod.Nodes.Add(newNode);
				addNodes(newNode, newEmp);
				}
		}
		//------
		private float rand_sal(float sal) {
			float rnum = rand.Next ();
			rnum = rnum / Int32.MaxValue;
			return rnum * sal / 5 + sal;
		}
		//------
		public Form1() {
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			init();
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing ) {
			if( disposing ) {
				if (components != null) {
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
		private void InitializeComponent() {
			this.lbSalary = new System.Windows.Forms.Label();
			this.EmpTree = new System.Windows.Forms.TreeView();
			this.SuspendLayout();
			// 
			// lbSalary
			// 
			this.lbSalary.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.lbSalary.Location = new System.Drawing.Point(64, 208);
			this.lbSalary.Name = "lbSalary";
			this.lbSalary.Size = new System.Drawing.Size(152, 24);
			this.lbSalary.TabIndex = 1;
			// 
			// EmpTree
			// 
			this.EmpTree.ImageIndex = -1;
			this.EmpTree.Location = new System.Drawing.Point(32, 24);
			this.EmpTree.Name = "EmpTree";
			this.EmpTree.SelectedImageIndex = -1;
			this.EmpTree.Size = new System.Drawing.Size(224, 168);
			this.EmpTree.TabIndex = 0;
			this.EmpTree.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.EmpTree_AfterSelect);
			// 
			// Form1
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(292, 273);
			this.Controls.AddRange(new System.Windows.Forms.Control[] {
																		  this.lbSalary,
																		  this.EmpTree});
			this.Name = "Form1";
			this.Text = "Basic composite";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			Application.Run(new Form1());
		}

		private void EmpTree_AfterSelect(object sender, TreeViewEventArgs e) {
			EmpNode node;
			node = (EmpNode)EmpTree.SelectedNode;
			getNodeSum(node);
		}
	}
}
