using System;
using CsharpPats;

namespace Seeding
{
	/// <summary>
	/// Summary description for Swimmer.
	/// </summary>
	public class Swimmer
	{
		private string firstName, lastName;
		private int age;
		private string club;
		private float time;
   
		private int heat, lane;
		//--------------------------------------
		public Swimmer(String dataline)  {
			StringTokenizer st = new StringTokenizer(dataline, " ");
			string lineNumber = st.nextToken();    //ignore and discard
			firstName = st.nextToken();
			lastName = st.nextToken();
			age = Convert.ToInt32 (st.nextToken().Trim());
			club = st.nextToken().Trim();
   
			string stime = st.nextToken().Trim();
			int i = stime.IndexOf(":");
			if (i > 0) {
				stime = stime.Substring(0, i) + stime.Substring (i+1);
			}
			time = Convert.ToSingle (  stime);
   
		}
   
		//-------------------------------
		public void setLane(int ln) {
			lane = ln;
		}
		//-------------------------------
		public int getLane() {
			return lane;
		}
		//-------------------------------
		public void setHeat(int ht) {
			heat = ht;
		}
		//-------------------------------
		public int getHeat() {
			return heat;
		}
		//-------------------------------
		public int getAge() {
			return age;
		}
		//-------------------------------
		public float getTime() {
			return time;
		}
		//-------------------------------
		public string getName() {
			return firstName+" "+lastName;
		}
		//-------------------------------
		public string getClub() {
			return club;
		}

	}
}
