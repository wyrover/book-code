using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for VarMultVarParse.
	/// </summary>
	public class VarMultvarParse : InterpChain
	{
		public override bool interpret() {
			if(topStack(ParseVar.VAR , ParseVar.MULTVAR )) {
				//reduce Var MultVar to MultVar
				ParseVar v  =(ParseVar) stk.pop();
				MultVar mv = (MultVar) stk.pop ();
				mv.add (v);
				stk.push (mv);
				return true;
			}
			else
				return false;
		}
	}
}
