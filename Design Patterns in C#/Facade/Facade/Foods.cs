using System;

namespace Facade
{
	/// <summary>
	/// Summary description for Foods.
	/// </summary>
	public class Foods: DBTable 	{
		public Foods(DBase db):base(db, "Foods"){	
		}
		//-----
		public void makeTable() {
			base.makeTable ("Foodname");
		}
		//-----
		public string getValue() {
			return base.getValue ("FoodName");
		}
	}
}
