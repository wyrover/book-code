using System;
using System.Windows.Forms;
namespace Interpreter
{
	/// <summary>
	/// Summary description for Verb.
	/// </summary>
	public class Verb:ParseVerb, Command
	{
		protected Data data;
		protected ListBox ptable;
		public Verb(string s, Data dt, ListBox pt) :base(s, dt, pt) 		{
			setData(dt, pt);
		}
		public void setData(Data dt, ListBox pt) {
			data = dt;
			ptable = pt;
		}
		public virtual void Execute() {
			//do nothing
		}
	}
}
