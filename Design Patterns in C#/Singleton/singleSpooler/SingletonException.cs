using System;

namespace singleSpooler
{
	/// <summary>
	/// Summary description for SingletonException.
	/// </summary>
	public class SingletonException:Exception 	{
		//new exception type for singleton classes
		public SingletonException(string s):base(s) {	
		}
	}
}
