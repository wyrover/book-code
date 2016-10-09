using System;
using CsharpPats;

namespace SwimmerTokenizer
{
	/// <summary>
	/// Summary description for Swimmer.
	/// </summary>
	public class Swimmer
	{
		private string frName, lName;
		private string club;
		private int Age;
		private int place;
		private FormatTime tms;
	//-----------
		public Swimmer(String dataLine) 		{
			StringTokenizer tok = new StringTokenizer (dataLine);
			place =  Convert.ToInt32 (tok.nextElement());
			frName = tok.nextElement ();
			lName = tok.nextElement ();
			string s = tok.nextElement ();
			Age = Convert.ToInt32 (s);
			club = tok.nextElement ();
			tms = new FormatTime (tok.nextElement ());					
		}
		//-----------
		public string name {
			get{
				return frName+" "+lName;
			}
		}
		//-----------
		public string time {
			get{
				return tms.getTime();
			}
			set  {
				tms = new FormatTime (value);
			}
		}
		//-------------------
		//age property
		public int age {
			get {
				return Age;
			}
			set {
				Age = value;
			}
		}
	}
}
