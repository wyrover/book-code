using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using Frez.Crypt;
using System.IO;

namespace Frez.Crypt
{
	/// <summary>
	/// Test the Rijndael class
	/// </summary>
	public class RijndaelTestForm : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Button uxTest2;
		private System.Windows.Forms.CheckBox uxHexOutput;
		private System.Windows.Forms.GroupBox uxBlockGroup;
		private System.Windows.Forms.GroupBox uxKeyGroup;
		private System.Windows.Forms.GroupBox uxModeGroup;
		private System.Windows.Forms.RadioButton uxBlock128;
		private System.Windows.Forms.RadioButton uxBlock192;
		private System.Windows.Forms.RadioButton uxBlock256;
		private System.Windows.Forms.RadioButton uxKey256;
		private System.Windows.Forms.RadioButton uxKey192;
		private System.Windows.Forms.RadioButton uxKey128;
		private System.Windows.Forms.RadioButton uxModeCBC;
		private System.Windows.Forms.RadioButton uxModeECB;
		private System.Windows.Forms.TextBox uxPlainText;
		private System.Windows.Forms.TextBox uxKeyText;
		private System.Windows.Forms.TextBox uxEncryptedText;
		private System.Windows.Forms.TextBox uxUnencryptedText;
		private System.Windows.Forms.Label label5;
		private System.Windows.Forms.TextBox uxIV;
		private System.Windows.Forms.Button uxTest1;
		private System.Windows.Forms.TextBox uxResults;
		private System.Windows.Forms.Button uxTestecbvk;
		private System.Windows.Forms.Button uxTestecbvt;
		private System.Windows.Forms.Button uxTestecbtbl;
		private System.Windows.Forms.Button uxTestecbem;
		private System.Windows.Forms.Button uxTestecbdm;
		private System.Windows.Forms.Button uxTestcbcem;
		private System.Windows.Forms.Button uxTestcbcdm;
		private System.Windows.Forms.TextBox uxLocations;
		private System.Windows.Forms.Label label6;
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public RijndaelTestForm()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			//
			// TODO: Add any constructor code after InitializeComponent call
			//
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
			this.uxTest2 = new System.Windows.Forms.Button();
			this.uxHexOutput = new System.Windows.Forms.CheckBox();
			this.uxBlockGroup = new System.Windows.Forms.GroupBox();
			this.uxBlock256 = new System.Windows.Forms.RadioButton();
			this.uxBlock192 = new System.Windows.Forms.RadioButton();
			this.uxBlock128 = new System.Windows.Forms.RadioButton();
			this.uxKeyGroup = new System.Windows.Forms.GroupBox();
			this.uxKey256 = new System.Windows.Forms.RadioButton();
			this.uxKey192 = new System.Windows.Forms.RadioButton();
			this.uxKey128 = new System.Windows.Forms.RadioButton();
			this.uxModeGroup = new System.Windows.Forms.GroupBox();
			this.uxModeCBC = new System.Windows.Forms.RadioButton();
			this.uxModeECB = new System.Windows.Forms.RadioButton();
			this.uxPlainText = new System.Windows.Forms.TextBox();
			this.uxKeyText = new System.Windows.Forms.TextBox();
			this.uxEncryptedText = new System.Windows.Forms.TextBox();
			this.uxUnencryptedText = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.uxIV = new System.Windows.Forms.TextBox();
			this.uxTest1 = new System.Windows.Forms.Button();
			this.uxResults = new System.Windows.Forms.TextBox();
			this.uxTestecbvk = new System.Windows.Forms.Button();
			this.uxTestecbvt = new System.Windows.Forms.Button();
			this.uxTestecbtbl = new System.Windows.Forms.Button();
			this.uxTestecbem = new System.Windows.Forms.Button();
			this.uxTestecbdm = new System.Windows.Forms.Button();
			this.uxTestcbcem = new System.Windows.Forms.Button();
			this.uxTestcbcdm = new System.Windows.Forms.Button();
			this.uxLocations = new System.Windows.Forms.TextBox();
			this.label6 = new System.Windows.Forms.Label();
			this.uxBlockGroup.SuspendLayout();
			this.uxKeyGroup.SuspendLayout();
			this.uxModeGroup.SuspendLayout();
			this.SuspendLayout();
			// 
			// uxTest2
			// 
			this.uxTest2.Location = new System.Drawing.Point(8, 264);
			this.uxTest2.Name = "uxTest2";
			this.uxTest2.TabIndex = 11;
			this.uxTest2.Text = "Test 2";
			this.uxTest2.Click += new System.EventHandler(this.uxTest2_Click);
			// 
			// uxHexOutput
			// 
			this.uxHexOutput.Checked = true;
			this.uxHexOutput.CheckState = System.Windows.Forms.CheckState.Checked;
			this.uxHexOutput.Location = new System.Drawing.Point(104, 264);
			this.uxHexOutput.Name = "uxHexOutput";
			this.uxHexOutput.Size = new System.Drawing.Size(96, 24);
			this.uxHexOutput.TabIndex = 12;
			this.uxHexOutput.Text = "Hex Output?";
			// 
			// uxBlockGroup
			// 
			this.uxBlockGroup.Controls.Add(this.uxBlock256);
			this.uxBlockGroup.Controls.Add(this.uxBlock192);
			this.uxBlockGroup.Controls.Add(this.uxBlock128);
			this.uxBlockGroup.Location = new System.Drawing.Point(216, 256);
			this.uxBlockGroup.Name = "uxBlockGroup";
			this.uxBlockGroup.Size = new System.Drawing.Size(88, 96);
			this.uxBlockGroup.TabIndex = 13;
			this.uxBlockGroup.TabStop = false;
			this.uxBlockGroup.Text = "Block Size";
			// 
			// uxBlock256
			// 
			this.uxBlock256.Checked = true;
			this.uxBlock256.Location = new System.Drawing.Point(24, 72);
			this.uxBlock256.Name = "uxBlock256";
			this.uxBlock256.Size = new System.Drawing.Size(48, 16);
			this.uxBlock256.TabIndex = 2;
			this.uxBlock256.TabStop = true;
			this.uxBlock256.Text = "256";
			// 
			// uxBlock192
			// 
			this.uxBlock192.Location = new System.Drawing.Point(24, 48);
			this.uxBlock192.Name = "uxBlock192";
			this.uxBlock192.Size = new System.Drawing.Size(48, 16);
			this.uxBlock192.TabIndex = 1;
			this.uxBlock192.Text = "192";
			// 
			// uxBlock128
			// 
			this.uxBlock128.Location = new System.Drawing.Point(24, 24);
			this.uxBlock128.Name = "uxBlock128";
			this.uxBlock128.Size = new System.Drawing.Size(48, 16);
			this.uxBlock128.TabIndex = 0;
			this.uxBlock128.Text = "128";
			// 
			// uxKeyGroup
			// 
			this.uxKeyGroup.Controls.Add(this.uxKey256);
			this.uxKeyGroup.Controls.Add(this.uxKey192);
			this.uxKeyGroup.Controls.Add(this.uxKey128);
			this.uxKeyGroup.Location = new System.Drawing.Point(312, 256);
			this.uxKeyGroup.Name = "uxKeyGroup";
			this.uxKeyGroup.Size = new System.Drawing.Size(88, 96);
			this.uxKeyGroup.TabIndex = 14;
			this.uxKeyGroup.TabStop = false;
			this.uxKeyGroup.Text = "Key Size";
			// 
			// uxKey256
			// 
			this.uxKey256.Checked = true;
			this.uxKey256.Location = new System.Drawing.Point(24, 72);
			this.uxKey256.Name = "uxKey256";
			this.uxKey256.Size = new System.Drawing.Size(48, 16);
			this.uxKey256.TabIndex = 2;
			this.uxKey256.TabStop = true;
			this.uxKey256.Text = "256";
			// 
			// uxKey192
			// 
			this.uxKey192.Location = new System.Drawing.Point(24, 48);
			this.uxKey192.Name = "uxKey192";
			this.uxKey192.Size = new System.Drawing.Size(48, 16);
			this.uxKey192.TabIndex = 1;
			this.uxKey192.Text = "192";
			// 
			// uxKey128
			// 
			this.uxKey128.Location = new System.Drawing.Point(24, 24);
			this.uxKey128.Name = "uxKey128";
			this.uxKey128.Size = new System.Drawing.Size(48, 16);
			this.uxKey128.TabIndex = 0;
			this.uxKey128.Text = "128";
			// 
			// uxModeGroup
			// 
			this.uxModeGroup.Controls.Add(this.uxModeCBC);
			this.uxModeGroup.Controls.Add(this.uxModeECB);
			this.uxModeGroup.Location = new System.Drawing.Point(408, 256);
			this.uxModeGroup.Name = "uxModeGroup";
			this.uxModeGroup.Size = new System.Drawing.Size(80, 96);
			this.uxModeGroup.TabIndex = 15;
			this.uxModeGroup.TabStop = false;
			this.uxModeGroup.Text = "Mode";
			// 
			// uxModeCBC
			// 
			this.uxModeCBC.Location = new System.Drawing.Point(16, 48);
			this.uxModeCBC.Name = "uxModeCBC";
			this.uxModeCBC.Size = new System.Drawing.Size(48, 16);
			this.uxModeCBC.TabIndex = 1;
			this.uxModeCBC.Text = "CBC";
			// 
			// uxModeECB
			// 
			this.uxModeECB.Checked = true;
			this.uxModeECB.Location = new System.Drawing.Point(16, 24);
			this.uxModeECB.Name = "uxModeECB";
			this.uxModeECB.Size = new System.Drawing.Size(48, 16);
			this.uxModeECB.TabIndex = 0;
			this.uxModeECB.TabStop = true;
			this.uxModeECB.Text = "ECB";
			// 
			// uxPlainText
			// 
			this.uxPlainText.Location = new System.Drawing.Point(128, 368);
			this.uxPlainText.Name = "uxPlainText";
			this.uxPlainText.Size = new System.Drawing.Size(360, 20);
			this.uxPlainText.TabIndex = 17;
			this.uxPlainText.Text = "12345678901234567890123456789012";
			// 
			// uxKeyText
			// 
			this.uxKeyText.Location = new System.Drawing.Point(128, 392);
			this.uxKeyText.Name = "uxKeyText";
			this.uxKeyText.Size = new System.Drawing.Size(360, 20);
			this.uxKeyText.TabIndex = 19;
			this.uxKeyText.Text = "12345678901234567890123456789012";
			// 
			// uxEncryptedText
			// 
			this.uxEncryptedText.Location = new System.Drawing.Point(128, 440);
			this.uxEncryptedText.Name = "uxEncryptedText";
			this.uxEncryptedText.Size = new System.Drawing.Size(360, 20);
			this.uxEncryptedText.TabIndex = 23;
			this.uxEncryptedText.Text = "";
			// 
			// uxUnencryptedText
			// 
			this.uxUnencryptedText.Location = new System.Drawing.Point(128, 464);
			this.uxUnencryptedText.Name = "uxUnencryptedText";
			this.uxUnencryptedText.Size = new System.Drawing.Size(360, 20);
			this.uxUnencryptedText.TabIndex = 25;
			this.uxUnencryptedText.Text = "";
			// 
			// label1
			// 
			this.label1.Location = new System.Drawing.Point(8, 368);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(72, 16);
			this.label1.TabIndex = 16;
			this.label1.Text = "Plain";
			// 
			// label2
			// 
			this.label2.Location = new System.Drawing.Point(8, 392);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(72, 16);
			this.label2.TabIndex = 18;
			this.label2.Text = "Key";
			// 
			// label3
			// 
			this.label3.Location = new System.Drawing.Point(8, 440);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(72, 16);
			this.label3.TabIndex = 22;
			this.label3.Text = "Encrypted";
			// 
			// label4
			// 
			this.label4.Location = new System.Drawing.Point(8, 464);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(72, 16);
			this.label4.TabIndex = 24;
			this.label4.Text = "Plain Again";
			// 
			// label5
			// 
			this.label5.Location = new System.Drawing.Point(8, 416);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(72, 16);
			this.label5.TabIndex = 20;
			this.label5.Text = "IV";
			// 
			// uxIV
			// 
			this.uxIV.Location = new System.Drawing.Point(128, 416);
			this.uxIV.Name = "uxIV";
			this.uxIV.Size = new System.Drawing.Size(360, 20);
			this.uxIV.TabIndex = 21;
			this.uxIV.Text = "";
			// 
			// uxTest1
			// 
			this.uxTest1.Location = new System.Drawing.Point(8, 8);
			this.uxTest1.Name = "uxTest1";
			this.uxTest1.TabIndex = 0;
			this.uxTest1.Text = "Test 1";
			this.uxTest1.Click += new System.EventHandler(this.uxTest1_Click);
			// 
			// uxResults
			// 
			this.uxResults.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((System.Byte)(0)));
			this.uxResults.Location = new System.Drawing.Point(8, 104);
			this.uxResults.Multiline = true;
			this.uxResults.Name = "uxResults";
			this.uxResults.ReadOnly = true;
			this.uxResults.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.uxResults.Size = new System.Drawing.Size(528, 144);
			this.uxResults.TabIndex = 10;
			this.uxResults.Text = "";
			// 
			// uxTestecbvk
			// 
			this.uxTestecbvk.Location = new System.Drawing.Point(96, 8);
			this.uxTestecbvk.Name = "uxTestecbvk";
			this.uxTestecbvk.Size = new System.Drawing.Size(104, 23);
			this.uxTestecbvk.TabIndex = 1;
			this.uxTestecbvk.Text = "Test ecb_vk.txt";
			this.uxTestecbvk.Click += new System.EventHandler(this.uxTestecbvk_Click);
			// 
			// uxTestecbvt
			// 
			this.uxTestecbvt.Location = new System.Drawing.Point(208, 8);
			this.uxTestecbvt.Name = "uxTestecbvt";
			this.uxTestecbvt.Size = new System.Drawing.Size(104, 23);
			this.uxTestecbvt.TabIndex = 2;
			this.uxTestecbvt.Text = "Test ecb_vt.txt";
			this.uxTestecbvt.Click += new System.EventHandler(this.uxTestecbvt_Click);
			// 
			// uxTestecbtbl
			// 
			this.uxTestecbtbl.Location = new System.Drawing.Point(320, 8);
			this.uxTestecbtbl.Name = "uxTestecbtbl";
			this.uxTestecbtbl.Size = new System.Drawing.Size(104, 23);
			this.uxTestecbtbl.TabIndex = 3;
			this.uxTestecbtbl.Text = "Test ecb_tbl.txt";
			this.uxTestecbtbl.Click += new System.EventHandler(this.uxTestecbtbl_Click);
			// 
			// uxTestecbem
			// 
			this.uxTestecbem.Location = new System.Drawing.Point(432, 8);
			this.uxTestecbem.Name = "uxTestecbem";
			this.uxTestecbem.Size = new System.Drawing.Size(104, 23);
			this.uxTestecbem.TabIndex = 4;
			this.uxTestecbem.Text = "Test ecb_e_m.txt";
			this.uxTestecbem.Click += new System.EventHandler(this.uxTestecbem_Click);
			// 
			// uxTestecbdm
			// 
			this.uxTestecbdm.Location = new System.Drawing.Point(96, 40);
			this.uxTestecbdm.Name = "uxTestecbdm";
			this.uxTestecbdm.Size = new System.Drawing.Size(104, 23);
			this.uxTestecbdm.TabIndex = 5;
			this.uxTestecbdm.Text = "Test ecb_d_m.txt";
			this.uxTestecbdm.Click += new System.EventHandler(this.uxTestecbdm_Click);
			// 
			// uxTestcbcem
			// 
			this.uxTestcbcem.Location = new System.Drawing.Point(208, 40);
			this.uxTestcbcem.Name = "uxTestcbcem";
			this.uxTestcbcem.Size = new System.Drawing.Size(104, 23);
			this.uxTestcbcem.TabIndex = 6;
			this.uxTestcbcem.Text = "Test cbc_e_m.txt";
			this.uxTestcbcem.Click += new System.EventHandler(this.uxTestcbcem_Click);
			// 
			// uxTestcbcdm
			// 
			this.uxTestcbcdm.Location = new System.Drawing.Point(320, 40);
			this.uxTestcbcdm.Name = "uxTestcbcdm";
			this.uxTestcbcdm.Size = new System.Drawing.Size(104, 23);
			this.uxTestcbcdm.TabIndex = 7;
			this.uxTestcbcdm.Text = "Test cbc_d_m.txt";
			this.uxTestcbcdm.Click += new System.EventHandler(this.uxTestcbcdm_Click);
			// 
			// uxLocations
			// 
			this.uxLocations.Location = new System.Drawing.Point(128, 72);
			this.uxLocations.Name = "uxLocations";
			this.uxLocations.Size = new System.Drawing.Size(408, 20);
			this.uxLocations.TabIndex = 9;
			this.uxLocations.Text = "";
			// 
			// label6
			// 
			this.label6.Location = new System.Drawing.Point(16, 72);
			this.label6.Name = "label6";
			this.label6.Size = new System.Drawing.Size(112, 16);
			this.label6.TabIndex = 8;
			this.label6.Text = "Test Files Location";
			// 
			// RijndaelTestForm
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(544, 493);
			this.Controls.Add(this.uxLocations);
			this.Controls.Add(this.label6);
			this.Controls.Add(this.uxTestcbcdm);
			this.Controls.Add(this.uxTestcbcem);
			this.Controls.Add(this.uxTestecbdm);
			this.Controls.Add(this.uxTestecbem);
			this.Controls.Add(this.uxTestecbtbl);
			this.Controls.Add(this.uxTestecbvt);
			this.Controls.Add(this.uxTestecbvk);
			this.Controls.Add(this.uxResults);
			this.Controls.Add(this.uxIV);
			this.Controls.Add(this.uxUnencryptedText);
			this.Controls.Add(this.uxEncryptedText);
			this.Controls.Add(this.uxKeyText);
			this.Controls.Add(this.uxPlainText);
			this.Controls.Add(this.uxHexOutput);
			this.Controls.Add(this.uxTest1);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.uxModeGroup);
			this.Controls.Add(this.uxKeyGroup);
			this.Controls.Add(this.uxBlockGroup);
			this.Controls.Add(this.uxTest2);
			this.Name = "RijndaelTestForm";
			this.Text = "Test Rijndael";
			this.Load += new System.EventHandler(this.RijndaelTestForm_Load);
			this.uxBlockGroup.ResumeLayout(false);
			this.uxKeyGroup.ResumeLayout(false);
			this.uxModeGroup.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new RijndaelTestForm());
		}

		private void uxTest2_Click(object sender, System.EventArgs e)
		{
			// Note we are "Using Frez.Crypt" at the top of the class
			try
			{
				this.Cursor = Cursors.WaitCursor;

				Rijndael.BlockSize blockSize;
				Rijndael.KeySize keySize;
				Rijndael.EncryptionMode cryptMode;
				bool toHex;
				string plainText, keyText, encryptedText, unencryptedText, ivText;

				if (uxBlock128.Checked)
					blockSize = Rijndael.BlockSize.Block128;
				else if (uxBlock192.Checked)
					blockSize = Rijndael.BlockSize.Block192;
				else
					blockSize = Rijndael.BlockSize.Block256;

				if (uxKey128.Checked)
					keySize = Rijndael.KeySize.Key128;
				else if (uxKey192.Checked)
					keySize = Rijndael.KeySize.Key192;
				else
					keySize = Rijndael.KeySize.Key256;

				if (uxModeCBC.Checked)
					cryptMode = Rijndael.EncryptionMode.ModeCBC;
				else
					cryptMode = Rijndael.EncryptionMode.ModeECB;

				toHex = uxHexOutput.Checked;

				plainText = uxPlainText.Text;
				keyText = uxKeyText.Text;

				// IV is not required in ECB mode and is optional in CBC mode (an IV of all zeros
				// is used if it is not supplied)
				ivText = uxIV.Text;

				// Encrypt our plain text
				encryptedText = Rijndael.EncryptData(plainText, keyText, ivText, blockSize, keySize, cryptMode, toHex);

				// Immediately unencrypt it using the same parameters
				unencryptedText = Rijndael.DecryptData(encryptedText, keyText, ivText, blockSize, keySize, cryptMode, toHex);

				// Display the encrypted text and unencrypted text (unencrypted text should be
				// the same as the plain text
				uxEncryptedText.Text = encryptedText;
				uxUnencryptedText.Text = unencryptedText;

				this.Cursor = Cursors.Default;
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
				uxEncryptedText.Clear();
				uxUnencryptedText.Clear();
			}
		}

		private void uxTest1_Click(object sender, System.EventArgs e)
		{
			byte[] key = new byte[32];
			byte[] block = new byte[32];
			string results = "";
			string temp;
			int nb=0, nk=0;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				for (int i=0;i<32;i++)
				{
					key[i] = 0;
					block[i] = 0;
				}

				Rijndael.gentables();

				for (nb=4; nb<=8; nb+=2)
					for (nk=4; nk<=8; nk+=2) 
					{
						Rijndael.gkey(nb, nk, key);

						results += "Block Size=" + (nb * 32).ToString() + 
							". Key Size=" + (nk * 32).ToString() + Environment.NewLine;

						results += "Plain" + Environment.NewLine;
						temp = "";
						for (int i=0; i<(nb * 4); i++)
							temp += string.Format("{0:X2}", block[i]);
						results += temp + Environment.NewLine;

						Rijndael.encrypt(block);

						results += "Encrypt" + Environment.NewLine;
						temp = "";
						for (int i=0; i<(nb * 4); i++)
							temp += string.Format("{0:X2}", block[i]);
						results += temp + Environment.NewLine;

						Rijndael.decrypt(block);

						results += "Decrypt" + Environment.NewLine;
						temp = "";
						for (int i=0; i<(nb * 4); i++)
							temp += string.Format("{0:X2}", block[i]);
						results += temp + Environment.NewLine + Environment.NewLine;
					}

				uxResults.Text = results;

				this.Cursor = Cursors.Default;
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestecbvk_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Electronic Codebook (ECB) Mode
			// Variable Key Known Answer Tests
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"ecb_vk.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}

				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 2) && (String.Compare(line, 0, "PT=", 0, 3, true) == 0))
							{
								results += line + Environment.NewLine;
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									block[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;
					
								line = stream.ReadLine();
								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));

								string CT;
								line = stream.ReadLine();
								CT = line.Substring(3);

								Array.Copy(block,0,temp,0,block.Length);

								Rijndael.gkey(nb, nk, key);
								Rijndael.encrypt(temp);

								string myCT;
								myCT = Rijndael.BytesToHex(temp);

								if (CT == myCT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;

				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestecbvt_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Electronic Codebook (ECB) Mode
			// Variable Text Known Answer Tests
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"ecb_vt.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}
				
				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 3) && (String.Compare(line, 0, "KEY=", 0, 3, true) == 0))
							{
								results += line + Environment.NewLine;

								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;
					
								line = stream.ReadLine();
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									block[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								string CT;
								line = stream.ReadLine();
								CT = line.Substring(3);

								Array.Copy(block,0,temp,0,block.Length);

								Rijndael.gkey(nb, nk, key);
								Rijndael.encrypt(temp);

								string myCT;
								myCT = Rijndael.BytesToHex(temp);

								if (CT == myCT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;

				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestecbtbl_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Electronic Codebook (ECB) Mode
			// Tables Known Answer Tests
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"ecb_tbl.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}

				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;

								line = stream.ReadLine();
								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));
					
								line = stream.ReadLine();
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									block[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								string CT;
								line = stream.ReadLine();
								CT = line.Substring(3);

								Array.Copy(block,0,temp,0,block.Length);

								Rijndael.gkey(nb, nk, key);
								Rijndael.encrypt(temp);

								string myCT;
								myCT = Rijndael.BytesToHex(temp);

								if (CT == myCT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;

				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestecbem_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Electronic Codebook (ECB) Mode - ENCRYPTION
			// Monte Carlo Test
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"ecb_e_m.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}

				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;

								line = stream.ReadLine();
								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));
					
								line = stream.ReadLine();
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									block[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								string CT;
								line = stream.ReadLine();
								CT = line.Substring(3);

								Array.Copy(block,0,temp,0,block.Length);

								Rijndael.gkey(nb, nk, key);

								for (int i=0; i<10000; i++)
									Rijndael.encrypt(temp);

								string myCT;
								myCT = Rijndael.BytesToHex(temp);

								if (CT == myCT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;

				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestecbdm_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Electronic Codebook (ECB) Mode - DECRYPTION
			// Monte Carlo Test
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"ecb_d_m.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}

				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;

								line = stream.ReadLine();
								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));
					
								line = stream.ReadLine();
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									block[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								string CT;
								line = stream.ReadLine();
								CT = line.Substring(3);

								Array.Copy(block,0,temp,0,block.Length);

								Rijndael.gkey(nb, nk, key);

								for (int i=0; i<10000; i++)
									Rijndael.decrypt(temp);

								string myCT;
								myCT = Rijndael.BytesToHex(temp);

								if (CT == myCT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;

				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestcbcem_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Cipher Block Chaining (CBC) Mode - ENCRYPTION
			// Monte Carlo Test
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			byte[] iv = new byte[0];
			byte[] last = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"cbc_e_m.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}

				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;

								line = stream.ReadLine();
								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));
					
								line = stream.ReadLine();
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];
								iv = new byte[blockSize / 8];
								last = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									iv[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								line = stream.ReadLine();

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									block[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								string CT;
								line = stream.ReadLine();
								CT = line.Substring(3);

								Rijndael.gkey(nb, nk, key);

								Array.Copy(block,0,temp,0,block.Length);

								for (int i=0; i<10000; i++)
								{
									for (int j=0; j<iv.Length; j++)
										temp[j] = (byte)(block[j] ^ iv[j]);

									Rijndael.encrypt(temp);
									if (i==0)
										Array.Copy(iv,0,block,0,block.Length);
									else
										Array.Copy(last,0,block,0,block.Length);

									Array.Copy(temp,0,iv,0,block.Length);
									Array.Copy(temp,0,last,0,block.Length);
								}

								string myCT;
								myCT = Rijndael.BytesToHex(temp);

								if (CT == myCT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;
				
				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void uxTestcbcdm_Click(object sender, System.EventArgs e)
		{
			// One of the tests required by NIST
			// Cipher Block Chaining (CBC) Mode - DECRYPTION
			// Monte Carlo Test
			byte[] key = new byte[0];
			byte[] block = new byte[0];
			byte[] temp = new byte[0];
			byte[] iv = new byte[0];
			byte[] ct = new byte[0];
			byte[] next = new byte[0];
			string results = "";
			int nb=0;
			int nk=0;
			string fullResults="";
			string filename;

			try
			{
				this.Cursor = Cursors.WaitCursor;

				uxResults.Clear();
				uxResults.Refresh();

				filename = uxLocations.Text;
				if (filename.Substring(filename.Length-1,1) != @"\")
					filename += @"\";

				filename += @"cbc_d_m.txt";

				Rijndael.gentables();

				FileInfo inputFile = new FileInfo(filename);

				if (! inputFile.Exists)
				{
					this.Cursor = Cursors.Default;
					MessageBox.Show("Cannot find the test file");
					return;
				}

				using (StreamReader stream = inputFile.OpenText())
				{
					string line = null;
					int keySize, blockSize;

					while ((line = stream.ReadLine()) != null) 
					{
						if (line.Length > 0)
						{
							if ((line.Length > 7) && (String.Compare(line, 0, "KEYSIZE=", 0, 8, true) == 0))
							{
								results += line + Environment.NewLine;
								keySize = int.Parse(line.Substring(8));
								nk = keySize / 32;
								key = new byte[keySize / 8];
								for (int i=0;i<key.Length;i++)
									key[i]=0;
							}
							else if ((line.Length > 1) && (String.Compare(line, 0, "I=", 0, 2, true) == 0))
							{
								results += line + Environment.NewLine;

								line = stream.ReadLine();
								for (int i=1; i<=line.Substring(4).Length; i+=2)
									key[(i / 2)] = (Convert.ToByte(line.Substring(i+3,2), 16));
					
								line = stream.ReadLine();
								blockSize = line.Substring(3).Length * 4;
								nb = blockSize / 32;
								block = new byte[blockSize / 8];
								temp = new byte[blockSize / 8];
								iv = new byte[blockSize / 8];
								next = new byte[blockSize / 8];
								ct = new byte[blockSize / 8];

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									iv[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								line = stream.ReadLine();

								for (int i=1; i<=line.Substring(3).Length; i+=2)
									ct[(i / 2)] = (Convert.ToByte(line.Substring(i+2,2), 16));

								string PT;
								line = stream.ReadLine();
								PT = line.Substring(3);

								Rijndael.gkey(nb, nk, key);

								Array.Copy(iv,0,next,0,block.Length);

								for (int i=0; i<10000; i++)
								{
									Array.Copy(ct,0,temp,0,block.Length);

									Rijndael.decrypt(temp);

									for (int j=0; j<temp.Length; j++)
										temp[j] ^= next[j];

									Array.Copy(ct,0,next,0,block.Length);
									Array.Copy(temp,0,ct,0,block.Length);
								}

								string myPT;
								myPT = Rijndael.BytesToHex(temp);

								if (PT == myPT)
									results += "PASSED" + Environment.NewLine;
								else
								{
									results += "FAILED" + Environment.NewLine;
									fullResults = "One or more tests failed";
								}
							}
						}
					}
				}
				uxResults.Text = results;

				this.Cursor = Cursors.Default;

				if (fullResults.Length > 0)
					MessageBox.Show(fullResults);
				else
					MessageBox.Show("All tests passed OK");
			}

			catch
			{
				this.Cursor = Cursors.Default;
				MessageBox.Show("Problem encountered");
			}
		}

		private void RijndaelTestForm_Load(object sender, System.EventArgs e)
		{
			uxLocations.Text = Environment.CurrentDirectory;
		}
	}
}
