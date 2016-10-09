using System;
using System.Collections ;



	/// <summary>
	/// Summary description for Stocks.
	/// </summary>
	public class Stocks:Equities 	{
		public Stocks() 		{
			array = new ArrayList();
			array.Add ("Cisco");
			array.Add ("Coca Cola");
			array.Add ("GE");
			array.Add ("Harley Davidson");
			array.Add ("IBM");
			array.Add ("Microsoft");
		}
		public override string toString() {
			return "Stocks";
		}
	}
