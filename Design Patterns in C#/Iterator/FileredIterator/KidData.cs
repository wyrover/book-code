using System;
using System.Collections ;
using CsharpPats;
namespace FileredIterator
{
	/// <summary>
	/// Summary description for KidData.
	/// </summary>
	public class KidData :IEnumerator	{
		private ArrayList kids;
		private int index;
		private Hashtable clubs;
		public KidData(string filename) 		{
			kids = new ArrayList ();
			clubs = new Hashtable ();
			csFile fl = new csFile (filename);
			fl.OpenForRead ();
			string line = fl.readLine ();
			while(line != null) {
				Kid kd = new Kid (line);
				string club = kd.getClub ();
				if(! clubs.Contains (club ) ) {
					clubs.Add (club, club);
				}
				kids.Add (kd);
				line = fl.readLine ();
			}
			fl.close ();
			index = 0;
		}
		//------
		public IDictionaryEnumerator getClubs() {
			return clubs.GetEnumerator ();
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
		//------
		public Kid[] getData() {
			Kid[] kds = new Kid [kids.Count ];
			for(int i=0; i< kids.Count ; i++ ) {
				kds[i] = (Kid)kids[i];
			}
			return kds;
		}
		//-----
		public int size() {
			return kids.Count ;
		}
		//-----
		public Kid getKid(int i) {
			if(i>=0 && i< kids.Count )
				return (Kid)kids[i];
			else
				return null;
		}
		//-----
		public KidIterator getIterator() {
			return new KidIterator (kids);
		}
		//----
		public FilteredIterator getFilteredIterator(string club) {
			return new FilteredIterator (kids, club);
		}
		
	}
}
