using System;
using System.Windows.Forms;
using System.Drawing ;
using CsharpPats;
namespace Chain
{
	/// <summary>
	/// Summary description for ImageChain.
	/// </summary>
	public class ImageChain :Chain 	{
		PictureBox picBox;		//image goes here
		//-----
		public ImageChain(PictureBox pc) 		{
			picBox = pc;		//save reference
		}
		//-----
		public override void sendToChain(string mesg) {
			//put image in picture box
			string fname = mesg + ".jpg";	//assume jpg filename
			csFile fl = new csFile(fname);
			if(fl.exists()) 
				picBox.Image = new Bitmap(fname);
			else{
				if (hasChain()){	//send off down chain
					chn.sendToChain(mesg);
				}
			}
		
		}
	}
}
