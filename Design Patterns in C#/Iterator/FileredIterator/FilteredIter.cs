using System;
using System.Collections ;
namespace FileredIterator
{
	/// <summary>
	/// Summary description for KidIterator.
	/// </summary>
	public class FilteredIterator : IEnumerator 	{
		private ArrayList kids;
		private int index;
		private string club;
		public FilteredIterator(ArrayList kidz, string club) 	{
			kids = kidz;
			index = 0;
			this.club = club;
		}
		//------
		public bool MoveNext() {
			bool more = index < kids.Count-1 ;
			if(more) {
				Kid kd = (Kid)kids[++index];
				more = index < kids.Count;
				while(more && ! kd.getClub().Equals (club)) {
					kd = (Kid)kids[index++];
					more = index < kids.Count ;
				}
			}
			return more;
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
