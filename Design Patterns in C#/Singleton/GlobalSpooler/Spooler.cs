using System;

namespace GlobalSpooler
{
	/// <summary>
	/// Summary description for Spooler.
	/// </summary>
	public class Spooler 	{
		private static bool instance_flag= false;
		private Spooler()  {
		}
		public static Spooler getSpooler() {
			if (! instance_flag) 
				return new Spooler ();
			else
				return null;
		}

	}
}
