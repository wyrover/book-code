using System;
using System.Drawing ;
namespace Proxy
{
	/// <summary>
	/// Summary description for QuickImage.
	/// </summary>
	public class QuickImage : Imager 	{
		public QuickImage()	{}
		public Image getImage() {
			return new Bitmap ("Box.gif");
		}
	}
}
