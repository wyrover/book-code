using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.IO ;

namespace Filedemo {
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	
	public class Form1 : System.Windows.Forms.Form {
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;
		private StreamReader rf;
		private bool eof;
		//------------
		public String readLine () {
			String s = rf.ReadLine ();
			if(s == null) 
				eof = true;
			return s;
		}
		public String readLineE() {
			String s = "";
			if (rf.Peek() > 0) {
				s = rf.ReadLine ();
			}
			else
				eof=true;
			return s;
		}
		//------------
		public bool fEof() {
			return eof;
		//------------
		}
		private void init() {
			if (File.Exists ("Foo.txt"))
				File.Delete ("foo.txt");
			try {
				//open text file for reading
				StreamReader ts = File.OpenText ("foo1.txt");
				String s =ts.ReadLine ();
				//open any type of file for reading
				FileStream fs = File.OpenRead ("foo2.any");
				//open for writing
				StreamWriter sw = File.CreateText ("foo3.txt");
				sw.WriteLine ("Hello file");

				//append to text file
				StreamWriter asw = new StreamWriter ("foo1.txt", true);
			}
			catch(Exception e ) {
				Console.WriteLine (e.Message );
			}
		}
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
			this.components = new System.ComponentModel.Container();
			this.Size = new System.Drawing.Size(300,300);
			this.Text = "Form1";
		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() {
			Application.Run(new Form1());
		}
	}
}
