using System;
using System.Windows.Forms ;
using System.IO ;
using System.IO.IsolatedStorage ;
using CsharpPats;
namespace Chain {
	/// <summary>
	/// Summary description for FileChain.
	/// </summary>
	public class FileChain : Chain {
		ListBox flist;
		public FileChain(ListBox lb)		{
			flist = lb;
		}
	//-----
		public override void sendToChain( string mesg) {
			//if the string matches any part of a filename
			//put those filenames in the file list box
			string[] files;
			string fname = mesg + "*.*";
			files = Directory.GetFiles(Directory.GetCurrentDirectory(), fname);
			//add them all to the listbox
			if (files.Length > 0){
				for (int i = 0; i< files.Length; i++) {
					csFile vbf = new csFile(files[i]);
					flist.Items.Add(vbf.getRootName());
				}
			}
			else {
				if ( hasChain()) {
					chn.sendToChain(mesg);
				}
			}
		}
	}
}
