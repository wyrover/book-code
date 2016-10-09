using System;

namespace Interpreter
{
	/// <summary>
	/// Summary description for ParseObject.
	/// </summary>
	public class ParseObject
	{
		public const int VERB=1000;
		public const int VAR=1010;
		public const int MULTVAR=1020;

		protected int value, type;
		public ParseObject(int val, int typ) 	{
			value = val;
			type = typ;
		}
		public int getValue() {
			return value;
		}
		public int getType() {
			return type;
		}
	}
}
