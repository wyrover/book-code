using System;

namespace NameFactory
{
	/// <summary>
	/// Summary description for Namer.
	/// </summary>
	//Base class for getting split names
	public class Namer 	{
		//parts stored here
		protected string frName, lName;
		
		//return first name
		public string getFrname(){
			return frName;
		}
		//return last name
		public string getLname() {
			return lName;
		}
	}
}
