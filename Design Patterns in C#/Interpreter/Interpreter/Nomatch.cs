using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for Nomatch.
	/// </summary>
	public class Nomatch :InterpChain
	{
		public override bool interpret() {
			while(stk.hasMoreElements ())
				stk.pop ();		//clear off all of stack
			return true;
		}
	}
}
