using System;

namespace Observer
{
	/// <summary>
	/// Summary description for Subject.
	/// </summary>
	public interface Subject 	{
		 void registerInterest(Observer obs);
	}
}
