using System;
using System.Drawing ;
namespace Proxy
{
	/// <summary>
	/// Summary description for FinalImage.
	/// </summary>
	public class FinalImage :Imager {
		public FinalImage()	{}
		public Image getImage() {
			return new Bitmap("flowrtree.jpg");
		}
	}
}
