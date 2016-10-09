using System;
using System.Drawing ;
using System.Threading ;
namespace Proxy
{
	/// <summary>
	/// Summary description for ImageProxy.
	/// </summary>
	public class ImageProxy 	{
		private bool done;
		private Timer timer;
		//-----
		public ImageProxy() 		{
			timer = new Timer (new TimerCallback (timerCall), this, 5000, 0);
		}
		//-----
		private void timerCall(object obj) {
			done = true;
			timer.Dispose ();
		}
		//-----
		public Image getImage() {
			Imager img;
			if (done)
				img = new FinalImage ();
			else
				img = new QuickImage ();
			return img.getImage ();
		}
	}
}
