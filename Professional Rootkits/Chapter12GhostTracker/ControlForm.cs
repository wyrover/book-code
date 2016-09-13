// ControlForm.cs
// Copyright Ric Vieler, 2006
// This file supports control panel operations

using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

namespace GhostTracker
{
	/// <summary>
	/// Summary description for ControlForm.
	/// </summary>
	public class ControlForm : System.Windows.Forms.Form
	{
		private string myIPAddress;
		private TargetController myTarget;
		private System.Windows.Forms.Label IPLabel;
		private System.Windows.Forms.Label statusLabel;
		private System.Windows.Forms.TextBox statusTextBox;
		private System.Windows.Forms.Label checkConnectionLabel;
		private System.Windows.Forms.Label putFileLabel;
		private System.Windows.Forms.Label getFileLabel;
		private System.Windows.Forms.Label putKeyLabel;
		private System.Windows.Forms.Label getKeyLabel;
		private System.Windows.Forms.Label CrashTargetLabel;
		private System.Windows.Forms.Label cleanTargetLabel;
		private System.Windows.Forms.Label destroyTargetLabel;
		private System.Windows.Forms.Button checkConnectionButton;
		private System.Windows.Forms.Button putFileButton;
		private System.Windows.Forms.Button getFileButton;
		private System.Windows.Forms.Button putKeyButton;
		private System.Windows.Forms.Button getKeyButton;
		private System.Windows.Forms.Button crashTargetButton;
		private System.Windows.Forms.Button cleanTargetButton;
		private System.Windows.Forms.Button destroyTargetButton;
		private System.Windows.Forms.Button commandShellButton;
		private System.Windows.Forms.Label commandShellLabel;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public ControlForm( string IPAddress, TargetController target )
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			myTarget = target;
			myIPAddress = IPAddress;
			this.IPLabel.Text = "Tagret IP Address: " + myIPAddress;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
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
			this.IPLabel = new System.Windows.Forms.Label();
			this.statusLabel = new System.Windows.Forms.Label();
			this.statusTextBox = new System.Windows.Forms.TextBox();
			this.checkConnectionLabel = new System.Windows.Forms.Label();
			this.putFileLabel = new System.Windows.Forms.Label();
			this.getFileLabel = new System.Windows.Forms.Label();
			this.putKeyLabel = new System.Windows.Forms.Label();
			this.getKeyLabel = new System.Windows.Forms.Label();
			this.CrashTargetLabel = new System.Windows.Forms.Label();
			this.cleanTargetLabel = new System.Windows.Forms.Label();
			this.destroyTargetLabel = new System.Windows.Forms.Label();
			this.checkConnectionButton = new System.Windows.Forms.Button();
			this.putFileButton = new System.Windows.Forms.Button();
			this.getFileButton = new System.Windows.Forms.Button();
			this.putKeyButton = new System.Windows.Forms.Button();
			this.getKeyButton = new System.Windows.Forms.Button();
			this.crashTargetButton = new System.Windows.Forms.Button();
			this.cleanTargetButton = new System.Windows.Forms.Button();
			this.destroyTargetButton = new System.Windows.Forms.Button();
			this.commandShellButton = new System.Windows.Forms.Button();
			this.commandShellLabel = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// IPLabel
			// 
			this.IPLabel.Location = new System.Drawing.Point(16, 16);
			this.IPLabel.Name = "IPLabel";
			this.IPLabel.Size = new System.Drawing.Size(192, 24);
			this.IPLabel.TabIndex = 0;
			this.IPLabel.Text = "Tagret IP Address: xxx.xxx.xxx.xxx";
			// 
			// statusLabel
			// 
			this.statusLabel.Location = new System.Drawing.Point(8, 280);
			this.statusLabel.Name = "statusLabel";
			this.statusLabel.Size = new System.Drawing.Size(48, 16);
			this.statusLabel.TabIndex = 1;
			this.statusLabel.Text = "Status:";
			// 
			// statusTextBox
			// 
			this.statusTextBox.Location = new System.Drawing.Point(64, 280);
			this.statusTextBox.Name = "statusTextBox";
			this.statusTextBox.Size = new System.Drawing.Size(136, 20);
			this.statusTextBox.TabIndex = 2;
			this.statusTextBox.Text = "";
			// 
			// checkConnectionLabel
			// 
			this.checkConnectionLabel.Location = new System.Drawing.Point(16, 48);
			this.checkConnectionLabel.Name = "checkConnectionLabel";
			this.checkConnectionLabel.Size = new System.Drawing.Size(136, 16);
			this.checkConnectionLabel.TabIndex = 3;
			this.checkConnectionLabel.Text = "Check connection";
			// 
			// putFileLabel
			// 
			this.putFileLabel.Location = new System.Drawing.Point(16, 72);
			this.putFileLabel.Name = "putFileLabel";
			this.putFileLabel.Size = new System.Drawing.Size(136, 16);
			this.putFileLabel.TabIndex = 4;
			this.putFileLabel.Text = "Put file...";
			// 
			// getFileLabel
			// 
			this.getFileLabel.Location = new System.Drawing.Point(16, 96);
			this.getFileLabel.Name = "getFileLabel";
			this.getFileLabel.Size = new System.Drawing.Size(136, 16);
			this.getFileLabel.TabIndex = 5;
			this.getFileLabel.Text = "Get file...";
			// 
			// putKeyLabel
			// 
			this.putKeyLabel.Location = new System.Drawing.Point(16, 120);
			this.putKeyLabel.Name = "putKeyLabel";
			this.putKeyLabel.Size = new System.Drawing.Size(136, 16);
			this.putKeyLabel.TabIndex = 6;
			this.putKeyLabel.Text = "Put key...";
			// 
			// getKeyLabel
			// 
			this.getKeyLabel.Location = new System.Drawing.Point(16, 144);
			this.getKeyLabel.Name = "getKeyLabel";
			this.getKeyLabel.Size = new System.Drawing.Size(136, 16);
			this.getKeyLabel.TabIndex = 7;
			this.getKeyLabel.Text = "Get key...";
			// 
			// CrashTargetLabel
			// 
			this.CrashTargetLabel.Location = new System.Drawing.Point(16, 168);
			this.CrashTargetLabel.Name = "CrashTargetLabel";
			this.CrashTargetLabel.Size = new System.Drawing.Size(136, 16);
			this.CrashTargetLabel.TabIndex = 8;
			this.CrashTargetLabel.Text = "Crash target.";
			// 
			// cleanTargetLabel
			// 
			this.cleanTargetLabel.Location = new System.Drawing.Point(16, 192);
			this.cleanTargetLabel.Name = "cleanTargetLabel";
			this.cleanTargetLabel.Size = new System.Drawing.Size(136, 16);
			this.cleanTargetLabel.TabIndex = 9;
			this.cleanTargetLabel.Text = "Clean target.";
			// 
			// destroyTargetLabel
			// 
			this.destroyTargetLabel.Location = new System.Drawing.Point(16, 216);
			this.destroyTargetLabel.Name = "destroyTargetLabel";
			this.destroyTargetLabel.Size = new System.Drawing.Size(136, 16);
			this.destroyTargetLabel.TabIndex = 10;
			this.destroyTargetLabel.Text = "Destroy target.";
			// 
			// checkConnectionButton
			// 
			this.checkConnectionButton.Location = new System.Drawing.Point(152, 40);
			this.checkConnectionButton.Name = "checkConnectionButton";
			this.checkConnectionButton.Size = new System.Drawing.Size(56, 23);
			this.checkConnectionButton.TabIndex = 11;
			this.checkConnectionButton.Text = "check";
			this.checkConnectionButton.Click += new System.EventHandler(this.checkConnectionButton_Click);
			// 
			// putFileButton
			// 
			this.putFileButton.Location = new System.Drawing.Point(152, 64);
			this.putFileButton.Name = "putFileButton";
			this.putFileButton.Size = new System.Drawing.Size(56, 23);
			this.putFileButton.TabIndex = 12;
			this.putFileButton.Text = "put file";
			// 
			// getFileButton
			// 
			this.getFileButton.Location = new System.Drawing.Point(152, 88);
			this.getFileButton.Name = "getFileButton";
			this.getFileButton.Size = new System.Drawing.Size(56, 23);
			this.getFileButton.TabIndex = 13;
			this.getFileButton.Text = "get file";
			// 
			// putKeyButton
			// 
			this.putKeyButton.Location = new System.Drawing.Point(152, 112);
			this.putKeyButton.Name = "putKeyButton";
			this.putKeyButton.Size = new System.Drawing.Size(56, 23);
			this.putKeyButton.TabIndex = 14;
			this.putKeyButton.Text = "put key";
			// 
			// getKeyButton
			// 
			this.getKeyButton.Location = new System.Drawing.Point(152, 136);
			this.getKeyButton.Name = "getKeyButton";
			this.getKeyButton.Size = new System.Drawing.Size(56, 23);
			this.getKeyButton.TabIndex = 15;
			this.getKeyButton.Text = "get key";
			// 
			// crashTargetButton
			// 
			this.crashTargetButton.Location = new System.Drawing.Point(152, 160);
			this.crashTargetButton.Name = "crashTargetButton";
			this.crashTargetButton.Size = new System.Drawing.Size(56, 23);
			this.crashTargetButton.TabIndex = 16;
			this.crashTargetButton.Text = "crash";
			// 
			// cleanTargetButton
			// 
			this.cleanTargetButton.Location = new System.Drawing.Point(152, 184);
			this.cleanTargetButton.Name = "cleanTargetButton";
			this.cleanTargetButton.Size = new System.Drawing.Size(56, 23);
			this.cleanTargetButton.TabIndex = 17;
			this.cleanTargetButton.Text = "clean";
			// 
			// destroyTargetButton
			// 
			this.destroyTargetButton.Location = new System.Drawing.Point(152, 208);
			this.destroyTargetButton.Name = "destroyTargetButton";
			this.destroyTargetButton.Size = new System.Drawing.Size(56, 23);
			this.destroyTargetButton.TabIndex = 18;
			this.destroyTargetButton.Text = "destroy";
			// 
			// commandShellButton
			// 
			this.commandShellButton.Location = new System.Drawing.Point(152, 232);
			this.commandShellButton.Name = "commandShellButton";
			this.commandShellButton.Size = new System.Drawing.Size(56, 23);
			this.commandShellButton.TabIndex = 20;
			this.commandShellButton.Text = "shell";
			// 
			// commandShellLabel
			// 
			this.commandShellLabel.Location = new System.Drawing.Point(16, 240);
			this.commandShellLabel.Name = "commandShellLabel";
			this.commandShellLabel.Size = new System.Drawing.Size(136, 16);
			this.commandShellLabel.TabIndex = 19;
			this.commandShellLabel.Text = "Open a command shell.";
			// 
			// ControlForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(216, 317);
			this.Controls.Add(this.commandShellButton);
			this.Controls.Add(this.commandShellLabel);
			this.Controls.Add(this.destroyTargetButton);
			this.Controls.Add(this.cleanTargetButton);
			this.Controls.Add(this.crashTargetButton);
			this.Controls.Add(this.getKeyButton);
			this.Controls.Add(this.putKeyButton);
			this.Controls.Add(this.getFileButton);
			this.Controls.Add(this.putFileButton);
			this.Controls.Add(this.checkConnectionButton);
			this.Controls.Add(this.destroyTargetLabel);
			this.Controls.Add(this.cleanTargetLabel);
			this.Controls.Add(this.CrashTargetLabel);
			this.Controls.Add(this.getKeyLabel);
			this.Controls.Add(this.putKeyLabel);
			this.Controls.Add(this.getFileLabel);
			this.Controls.Add(this.putFileLabel);
			this.Controls.Add(this.checkConnectionLabel);
			this.Controls.Add(this.statusTextBox);
			this.Controls.Add(this.statusLabel);
			this.Controls.Add(this.IPLabel);
			this.Name = "ControlForm";
			this.Text = "Target Controller";
			this.ResumeLayout(false);

		}
		#endregion

		private void checkConnectionButton_Click(object sender, System.EventArgs e)
		{
			// Check to see if the target is still there
			if( myTarget.Ping() )
				statusTextBox.Text = "Target is responding.";
			else
				statusTextBox.Text = "Target is not responding.";
		}
	}
}
