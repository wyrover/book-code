using System;
using CombinationsLib;

namespace CombinationEssentials
{
	class Class1
	{
		[STAThread]
		static void Main()
		{
			long n = 4;
			long k = 2;
			Console.WriteLine("With n = " + n + ", and k = " + k);
			Console.WriteLine("There are " + Combination.Choose(n,k) + " combinations\n");

			Combination c = new Combination(n,k);
			Console.WriteLine("The mathematical combinations are:");
			while (c != null)
			{
				Console.WriteLine(c.ToString());
				c = c.Successor();
			}
			Console.ReadLine();
			
		} // Main()

	} // class Class1
} // ns CombinationEssentials
