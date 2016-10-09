using System;

namespace FormatValue
{
	/// <summary>
	/// Summary description for FormatTime.
	/// </summary>
	public class FormatTime 	{
		private bool errflag;
		private float t;
		private int mins; 
		private float secs;
		static public int NT = 10000;
		static public int DQ = 20000;
		private int minVal=1000;
	//---------------------------
		public FormatTime(string entry)		{
			errflag = false;
			minVal = 1000;
			t = 0;
			if (! testCharVals(entry)) {
				int i = entry.IndexOf (":");
				if (i >= 0 ) {
					mins = Convert.ToInt32 (entry.Substring (0, i));
					secs = Convert.ToSingle (entry.Substring (i+1));
					if(secs >= 60.0F ) {
						errflag = true;
						t = NT;
					}
					t = mins *100 + secs;
				}
				else {

					float fmins = Convert.ToSingle (entry) / 100;
					mins = (int)fmins;
					secs = Convert.ToSingle (entry) - 100 * mins;
					if (secs >= 60) {
						errflag = true;
						t = NT;
					}
					else
						t = Convert.ToSingle(entry);
				}
			}
		}
		//---------------------------
		public string getTime() {
			if(t == NT )
				return "NT";
			if (t == DQ)
				return "DQ";
			if(t > 60)
				return getMinTime();
			else
				return getSecTime();

		}
		//---------------------------
		private bool testCharVals(string entry) {
			string stime = entry.ToLower ();
			bool tchar = false;
			int j = stime.IndexOf ("n");
			if (j >= 0) {
				t = NT;
				tchar = true;
			}
			j = stime.IndexOf ("d");
			if (j >= 0 ) {
				t= DQ;
				tchar = true;
			}
			return tchar;
		}
		//---------------------------
		private string getSecTime() {
			string ssecs = Convert.ToString (secs);
			return	secs.ToString ("f");
		}
		//---------------------------
		private string getMinTime() {
			string stime = Convert.ToString (mins)+":"+getSecTime();
			return stime;
		}
		/----------------------------
		public void setSingle(float tm) {
			t = tm;
			if((tm > minVal) && (tm < NT)) {
			t = tm / 100.0f;
			}
		}
		
	}
}
