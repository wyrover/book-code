using System;
using System.Collections ;
namespace Interpreter
{
	/// <summary>
	/// Summary description for MultVar.
	/// </summary>
	public class MultVar  :ParseVar
	{
		private ArrayList multVec;
		
		public MultVar(ParseObject v1, ParseObject v2):base("") 		{
			multVec = new ArrayList ();
			multVec.Add (v1);
			multVec.Add (v2);
			type = MULTVAR;
		}
		public MultVar() :base("") {
			multVec = new ArrayList ();
			type = MULTVAR;
		}
		public void add(ParseObject v) {
			multVec.Add (v);
		}
		public ArrayList getVector() {
			return multVec;
		}
	}
}
