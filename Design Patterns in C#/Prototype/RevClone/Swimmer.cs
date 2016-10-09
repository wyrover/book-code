using System;
using CsharpPats;
namespace DeepSexClone
{
	/// <summary>
	/// Summary description for Swimmer.
	/// </summary>
	public class Swimmer:IComparable 	{
		private string name;		 //name
		private string lname, frname;//split names
		private int age;			//age
		private string club;		//club initials
		private float time;			//time achieved
		private bool female;		//sex
	//---------
		public Swimmer(string line) {
			StringTokenizer tok = new StringTokenizer(line,",");
			splitName(tok);
			age = Convert.ToInt32 (tok.nextToken());
			club = tok.nextToken();
			time = Convert.ToSingle (tok.nextToken());
			string sx = tok.nextToken().ToUpper ().Trim ();
			female = sx.Equals ("F");
		}
		//---------
		private void splitName(StringTokenizer tok) {
			name = tok.nextToken();
			int i = name.IndexOf (" ");
			if (i > 0 ) {
				frname = name.Substring (0, i);
				lname = name.Substring (i + 1).Trim ();
			}
		}
		//---------
		public int CompareTo(object swo) {
			Swimmer sw = (Swimmer)swo;
			return lname.CompareTo (sw.getLName() );
		}
		//---------
		public bool isFemale() {
			return female;
		}
		//---------
		public int getAge() {
			return age;
		}
		//---------
		public float getTime() {
			return time;
		}
		//---------
		public string getName() {
			return name;
		}
		//---------
		public string getClub() {
			return club;
		}
		//---------
		public string getLName() {
			return lname;
		}
	}
}
