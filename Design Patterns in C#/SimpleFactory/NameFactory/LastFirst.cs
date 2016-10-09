using System;

namespace NameFactory
{
	/// <summary>
	/// Summary description for LastFirst.
	/// </summary>
	public class LastFirst : Namer
	{
		public LastFirst(string name) 		{
			int i = name.IndexOf (",");
			if(i > 0) {
				lName = name.Substring (0, i);
				frName = name.Substring (i + 1).Trim ();
			}
			else {
				lName = name;
				frName = "";
			}
		}
	}
}
