using System;
using System.Collections ;
namespace Interpreter
{
	/// <summary>
	/// Summary description for Stack.
	/// </summary>
	public class Stack 	{
		ArrayList stk;
		public Stack() 		{
			stk = new ArrayList ();
		}
		//------
		public void push(Object obj) {
			stk.Add (obj);
		}
		//-----
		public ParseObject pop() {
			int i =  stk.Count -1;
			ParseObject obj = (ParseObject) stk[i];
			stk.RemoveAt (i);
			return obj;
		}
		//-----
		public ParseObject top() {
			int i =  stk.Count -1;
			return (ParseObject)stk[i];
		}
		//-----
		public bool hasMoreElements() {
			return stk.Count > 0;
		}
		//-----
		public ParseObject nextTop() {
			int i =  stk.Count -2;
			if (i >= 0)
				return (ParseObject)stk[i];
			else
				return null;
		}
		//------
		public void pop2Push(ParseObject p){
			int i = stk.Count ;
			if (i >= 2) {
				pop();
				pop();
				push(p);
			}
			else
				push(p);
		}
	}
}
