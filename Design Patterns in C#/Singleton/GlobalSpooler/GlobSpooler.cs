using System;

namespace GlobalSpooler
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class GlobSpooler
	{
		static void Main(string[] args) 		{
			Spooler sp1 = Spooler.getSpooler();
			if (sp1 != null)
				Console.WriteLine ("Got 1 spooler");
			Spooler sp2 = Spooler.getSpooler ();
			if (sp2 == null)
				Console.WriteLine ("Can\'t get spooler");
			//fails at compiler time
			Spooler sp3 = new Spooler ();
		}
	}
}
