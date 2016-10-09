using System;

namespace Visitor
{
	/// <summary>
	/// Summary description for Visitor.
	/// </summary>
	public abstract class Visitor 	{
		public abstract void visit(Employee emp);
		public abstract void visit(Boss bos);
	}
}
