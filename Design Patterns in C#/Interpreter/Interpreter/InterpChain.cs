using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for InterpChain.
	/// </summary>
	public abstract class InterpChain:Chain	{
		private Chain nextChain;
		protected Stack stk;
		private bool hasChain;
		//-----
		public InterpChain() 	{
			stk = new Stack ();	
			hasChain = false;
		}
		//-----
		public void addToChain(Chain c) {
			nextChain = c;
			hasChain = true;
		}
		//-----
		public abstract bool interpret();
		//-----
		public void sendToChain(Stack stack) {
			stk = stack;
			if(! interpret()  ) {		//interpret stack
				nextChain.sendToChain (stk);	//pass along
			}
		}
		//-----
		public bool topStack(int c1, int c2) { 
			ParseObject p1, p2;
			p1 = stk.top ();
			p2 = stk.nextTop ();
			try{
				return (p1.getType() == c1 && p2.getType() == c2);
				}
			catch(NullReferenceException) {
				return false;
			}
		}
		//-----
		public void addArgsToVerb() {
			ParseObject p = (ParseObject) stk.pop();
			ParseVerb v =  (ParseVerb) stk.pop();
			v.addArgs (p);
			stk.push (v);
		}
		//-----
		public Chain getChain() {
			return nextChain;
		}
	}
}
