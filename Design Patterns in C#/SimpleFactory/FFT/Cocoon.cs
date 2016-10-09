using System;

namespace FFT
{
	/// <summary>
	/// Summary description for Cocoon.
	/// </summary>
	public class Cocoon 	{
		static public Butterfly getButterfly(float y) {
			if (y != 0)
				return new TrigButterfly(y);
			else
				return new addButterfly(y);
		}
	}
}
