using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for VarVarParse.
	/// </summary>
	public class VarVarParse : InterpChain 	{
		public override bool interpret() {
			if(topStack(ParseVar.VAR , ParseVar.VAR )) {
				//reduces VAR VAR to MULTVAR
				ParseVar v1 = (ParseVar) stk.pop();
				ParseVar v2 = (ParseVar) stk.pop();
				MultVar mv = new MultVar (v2, v1);
				stk.push (mv);
				return true;
			}
			else
				return false;
		}
	}
}
