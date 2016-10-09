using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for VerbMultVarParse.
	/// </summary>
	public class VerbMultvarParse : InterpChain
	{
		public override bool interpret() {
			if(topStack(ParseObject.MULTVAR , ParseObject.VERB )) {
				addArgsToVerb();
				return true;
			}
			else
				return false;
		}
	}
}
