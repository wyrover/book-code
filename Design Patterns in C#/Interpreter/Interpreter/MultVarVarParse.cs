using System;
using System.Collections ;
namespace Interpreter
{
	/// <summary>
	/// Summary description for MultVarVarParse.
	/// </summary>
	public class MultVarVarParse :InterpChain
	{
		public override bool interpret() {
			//reduce Var to Multvar
			if(topStack(ParseObject.MULTVAR , ParseObject.VAR )) {
				MultVar mv = new MultVar ();
				MultVar mvo = (MultVar)stk.pop();
				ParseVar v = (ParseVar)stk.pop();
				mv.add (v);
				ArrayList mvec = mvo.getVector ();
				for(int i=0; i < mvec.Count ; i++) {
					mv.add ((ParseVar)mvec[i]);
				}
				stk.push (mv);
				return true;
			}
			else
				return false;
		}
	}
}
