using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for Chain.
	/// </summary>
	public interface Chain {
		void addToChain(Chain c);
		void sendToChain(Stack stk);
		Chain getChain();
		
	}
}
