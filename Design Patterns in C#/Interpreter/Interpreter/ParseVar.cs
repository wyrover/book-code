using System;
using System.Collections ;

namespace Interpreter
{
	/// <summary>
	/// Summary description for ParseVar.
	/// </summary>
	public class ParseVar: ParseObject {
		public const int FRNAME = 1;
		public const int LNAME = 2;
		public const int AGE = 3;
		public const int CLUB = 4;
		public const int TIME = 5;
		public const int tabMax = 5;
		//-----
		private static Hashtable hash, keyhash;
		
		//-----
		static  ParseVar() {
			hash = new Hashtable ();
			keyhash = new Hashtable ();
			hash.Add("frname", FRNAME);
			hash.Add ("lname", LNAME);
			hash.Add ("age", AGE);
			hash.Add ("club", CLUB);
			hash.Add ("time", TIME);		
			keyhash.Add(FRNAME, "frname");
			keyhash.Add (LNAME, "lname");
			keyhash.Add (AGE, "age");
			keyhash.Add (CLUB, "club");
			keyhash.Add (TIME, "time");		
		}
		//-------------
		static public string getTableName(int key) {
			return (string) keyhash[key];
		}
		//-----
		static public int computeValue(string s) {
			int v = 0;
			try {
				v =(int) hash[s];
				return v;
			}
			catch (NullReferenceException ) {
				v = -1;
				return v;
			}	
		} 
		//------
		public bool isLegal() {
			return value >= 0;
		}
		//----------
		public ParseVar(string s) :base(-1, VAR)	{
			value = ParseVar.computeValue (s.ToLower() );
		}
	}
}
