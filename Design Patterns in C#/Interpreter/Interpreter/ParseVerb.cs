using System;
using System.Windows.Forms ;
using System.Collections ;

namespace Interpreter
{
	/// <summary>
	/// Summary description for ParseVerb.
	/// </summary>
	public class ParseVerb:ParseObject
	{
		protected const int PRINT = 100;
		protected const int SORT = 110;
		protected const int THENBY = 120;
	
		protected ArrayList args;
		protected Data kid;
		protected ListBox pt;
		protected ParseVerb pv;

		public ParseVerb(string s, Data kd, ListBox ls):base(-1, VERB) 		{
			args = new ArrayList ();
			kid = kd;
			pt = ls;
			if(s.ToLower().Equals ("print")) {
				value = PRINT;
			}
			if(s.ToLower().Equals ("sortby")) {
				value = SORT;
			}
		}
		//------
		public ParseVerb getVerb(string s) {
			pv = null;
			if(s.ToLower ().Equals ("print"))
				pv =new Print(s,kid, pt);
			if(s.ToLower ().Equals ("sortby"))
				pv = new Sort (s, kid, pt);
			return pv;
		}
		public void addArgs(MultVar mv) {
			args = mv.getVector ();
		}
		public void addArgs(ParseObject p) {
			args.Add (p);
		}
		public bool isLegal() {
			return (value > 0);
		}
	}
}
