using System;
using System.Collections;
using CsharpPats;

namespace Seeding
{
	/// <summary>
	/// Summary description for Event.
	/// </summary>
	public abstract class Event 	{
		protected int numLanes;
		protected ArrayList swimmers;

		public Event(string filename, int lanes) {
			numLanes = lanes;
			swimmers = new ArrayList();
			//read in swimmers from file
			 csFile f = new csFile(filename);
			f.OpenForRead ();
			string s = f.readLine();
			while (s != null) {
				Swimmer sw = new Swimmer(s);
				swimmers.Add (sw);
				s = f.readLine();
			}
			f.close();
		}
		public abstract Seeding getSeeding();
		public abstract bool isPrelim();
		public abstract bool isFinal();
		public abstract bool isTimedFinal();
	}
}
