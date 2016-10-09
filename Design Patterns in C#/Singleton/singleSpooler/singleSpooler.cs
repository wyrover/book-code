using System;

namespace singleSpooler
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	public class singleSpooler 	{
		static void Main(string[] args) { 
			Spooler pr1, pr2;
      //open one printer--this should always work
      Console.WriteLine ("Opening one spooler");
      try {
		pr1 = new Spooler();
      }
      catch (SingletonException e)	{
		  Console.WriteLine (e.Message);
	  }
      //try to open another printer --should fail
      Console.WriteLine ("Opening two spoolers");
      try{
	      pr2 = new Spooler();
      }
      catch (SingletonException e) {
		  Console.WriteLine (e.Message);
	  }
}
	}
}
