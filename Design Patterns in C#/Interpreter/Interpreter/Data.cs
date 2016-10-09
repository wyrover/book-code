using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for Data.
	/// </summary>
	public class Data {
	 private Kid[] kids;
		public Data(Kid[] kds)
		{
			kids = kds;
		}
		public Kid[] getData() {
			return kids;
		}
	}
}
