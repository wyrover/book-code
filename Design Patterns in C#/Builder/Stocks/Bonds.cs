using System;
using System.Collections ;

	/// <summary>
	/// Summary description for Bonds.
	/// </summary>
	public class Bonds:Equities
	{
		public Bonds()
		{
			array = new ArrayList();
			array.Add ("CT GO 2005");
			array.Add ("NY GO 2012");
			array.Add ("GE Corp Bonds");
		}
		public override string toString() {
			return "Bonds";
		}
	}

