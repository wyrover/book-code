using System;

namespace NameFactory
{
	/// <summary>
	/// Summary description for FirstFirst.
	/// </summary>
	public class FirstFirst : Namer
	{
		public FirstFirst(string name)
		{
			int i = name.IndexOf (" ");
			if(i > 0) {
				frName = name.Substring (0, i).Trim ();
				lName = name.Substring (i + 1).Trim ();
			}
			else {
				lName = name;
				frName = "";
			}
		}
	}
}
