using System;

namespace singleSpooler
{
	/// <summary>
	/// Prototype of Spooler Singleton
	/// such that only one instane can ever exist.
	/// </summary>
	public class Spooler 	{
		static bool instance_flag = false; //true if one instance
		public Spooler()  {
		if (instance_flag)
			throw new SingletonException("Only one printer allowed");
		else
			instance_flag=true;     //set flag for one instance
		Console.WriteLine ("printer opened");
    	}
	}
}
