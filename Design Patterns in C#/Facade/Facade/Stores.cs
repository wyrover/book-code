using System;

namespace Facade
{
	/// <summary>
	/// Summary description for Stores.
	/// </summary>
	public class Stores :DBTable 	{
		public Stores(DBase db):base(db, "Stores"){	
		}
	   //-----
	    public void makeTable() {
			base.makeTable ("Storename");
		}
	}
}
