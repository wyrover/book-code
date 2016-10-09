using System;
using System.Collections ;
namespace Mediate
{
	/// <summary>
	/// Summary description for KidIterator.
	/// </summary>
	public class KidIterator : IEnumerator 	{
		private ArrayList kids;
		private int index;
		public KidIterator(ArrayList kidz) 	{
			kids = kidz;
			index = 0;
		}
		//------
		public bool MoveNext() {
			index++;
			return index < kids.Count ;
		}
		//------
		public object Current {
			get {
				return kids[index];
			}
		}
		//------
		public void Reset() {
			index = 0;
		}
	}
}
