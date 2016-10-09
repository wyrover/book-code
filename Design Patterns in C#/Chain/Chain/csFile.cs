using System;
using System.IO ;
using System.IO.IsolatedStorage ;
namespace CsharpPats
{
	/// <summary>
	/// A simple file handlng class
	/// </summary>
	public class csFile
	{
		private string fileName;
		StreamReader ts;
		StreamWriter ws;
		private bool opened, writeOpened;
		//-----------
		public csFile() {
			init();
		}
		//-----------
		private void init() {
			opened = false;
			writeOpened = false;
		}
		//-----------
		public csFile(string file_name) 	{
			fileName = file_name;
			init();
		}
		//-----------
		public bool exists() {
			return File.Exists(fileName);
		}
		//-----------
		public bool OpenForRead(string file_name){
			fileName = file_name;
			try {
				ts = new StreamReader (fileName);
				opened=true;
			}
			catch(FileNotFoundException ) {
				return false;
			}
			return true;
		}
		//-----------
		public bool OpenForRead() {
			return OpenForRead(fileName);
		}
		//-----------
		public string readLine() {
			return ts.ReadLine ();
		}
		//-----------
		public void writeLine(string s) {
			ws.WriteLine (s);
		}
		//-----------
		public void close() {
			if (opened)
				ts.Close ();
			if(writeOpened)
				ws.Close();
		}
		//-----------
		public bool OpenForWrite() {
			return OpenForWrite(fileName);
		}
		//-----------
		public bool OpenForWrite(string file_name) {
			try{
				ws = new StreamWriter (file_name);
				fileName = file_name;
				writeOpened = true;
				return true;
			}
			catch(FileNotFoundException ) {
				return false;
			}
		}
		//-----------
		public string getRootName() {
			int i = fileName.LastIndexOf ("\\");
			string root = fileName;
			if(i >0 ) {
				root = fileName.Substring (i+1);
			}
			return root;
		}
	}
}
