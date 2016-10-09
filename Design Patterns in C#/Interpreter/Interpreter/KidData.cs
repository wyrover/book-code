using System;
using System.Collections ;
using CsharpPats;
namespace Interpreter
{
	/// <summary>
	/// Summary description for KidData.
	/// </summary>
	public class KidData 	{
		private ArrayList kids;
		public KidData(string filename) 		{
			kids = new ArrayList ();
			csFile fl = new csFile (filename);
			fl.OpenForRead ();
			string line = fl.readLine ();
			while(line != null) {
				Kid kd = new Kid (line);
				kids.Add (kd);
				line = fl.readLine ();
			}
			fl.close ();
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
		public ArrayList getKidData(int key) {
			ArrayList ara = new ArrayList ();
			for(int i =0; i< kids.Count ; i++ ) {
				Kid kd = (Kid)kids[i];
				ara.Add (kd.getData (key));
			}
			return ara;
		}
		public int getTableKey(string tbName) {
			return ParseVar.computeValue (tbName.ToLower ());
		}
		public string getTableName(int key) {
			return ParseVar.getTableName (key);
		}
	}
}
