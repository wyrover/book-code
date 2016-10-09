using System;
using System.Collections ;
namespace Interpreter
{
	/// <summary>
	/// Summary description for VerbAction.
	/// </summary>
	public class VerbAction : InterpChain
	{
		private ArrayList actionList;
		public VerbAction(ArrayList ar) {
			actionList = ar;
		}
		//-----
		public override bool interpret() {
			try {
				if(stk.top().getType() == ParseVerb.VERB ) {
					actionList.Add (stk.pop() );
					return true;
				}
				else
					return false;
			}
			catch(NullReferenceException) {
				return false;
			}
		}
		
	}
}
