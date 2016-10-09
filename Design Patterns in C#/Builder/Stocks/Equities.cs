using System;
using System.Collections ;


	/// <summary>
	/// Summary description for Equities.
	/// </summary>
	public abstract class Equities 	{
		protected ArrayList array;
		public abstract string toString();
	//----------	
		public ArrayList getNames() {
			return array;
		}
	//----------	
		public int count() {
			return array.Count ;
		}
	}
