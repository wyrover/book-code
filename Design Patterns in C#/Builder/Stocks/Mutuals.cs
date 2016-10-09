using System;
using System.Collections ;


	/// <summary>
	/// Summary description for Mutuals.
	/// </summary>
	public class Mutuals:Equities
	{
		public Mutuals()
		{
			array = new ArrayList();
			array.Add ("Fidelity Magellan");
			array.Add ("Lindner");
			array.Add ("T Rowe Price");
			array.Add ("Vanguard Primecap");
			
		}
		public override string toString() {
			return "Mutuals";
		}
	}
