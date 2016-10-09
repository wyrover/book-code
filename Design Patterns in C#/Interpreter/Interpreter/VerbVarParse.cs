using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for VerbVarParse.
	/// </summary>
	public class VerbVarParse : InterpChain
	{
		public override bool interpret() {
			if(topStack(ParseObject.VAR , ParseObject.VERB )) {
				addArgsToVerb();
				return true;
			}
			else
				return false;
		}
	}
}
