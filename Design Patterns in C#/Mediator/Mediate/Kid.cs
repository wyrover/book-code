using System;
using CsharpPats;
using System.Collections ;
namespace Mediate
{
	/// <summary>
	/// Summary description for Kid.
	/// </summary>
	public class Kid
	{
		private string frname, lname, club;
		private int age;
		private float time;
		private Hashtable hash;
		public Kid(string line) {
			hash = new Hashtable ();
			StringTokenizer tok = new StringTokenizer (line);
			string lnum = tok.nextToken ();
			frname = tok.nextToken ();
			//hash.Add (ParseVar.FRNAME , frname);
			lname = tok.nextToken ();
			//hash.Add (ParseVar.LNAME , lname);
			age = Convert.ToInt32 ( tok.nextToken ());
			//hash.Add (ParseVar.AGE , age);
			club = tok.nextToken ();
			//hash.Add (ParseVar.CLUB , club);
			time = Convert.ToSingle (tok.nextToken ());
			//hash.Add (ParseVar.TIME , time);
		}
		//-------
		public object getData(int key) {
			return hash[key];
		}
		//-----
		public bool compare(Kid kd, int key) {
			return compareTo(getData(key) , kd.getData (key));
		}
		//-----
		private bool compareTo(object o1, object o2) {
			if (o1.GetType().Name.Equals  ("String") )
				return compare_To((string)o1, (string) o2);
			else
				return compare_To((float)o1, (float)o2);
		}
		private bool compare_To(String s1, String s2) {
			return s1.CompareTo (s2) >0;
		}
		private bool compare_To(int k1, int k2){ 
			return k1 > k2;
		}
		private bool compare_To(float f1, float f2) {
			return f1 > f2;
		}
		public string getFrname() {
			return frname;
		}
		public string getLname() {
			return lname;
		}
		public int getAge() {
			return age;
		}
		public string getClub() {
			return club;
		}
	}
}
