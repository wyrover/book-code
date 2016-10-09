using System;
using System.IO ;

namespace SwimmerTokenizer
{
	/// <summary>
	/// A simple interface to file IO methods
	/// </summary>
	public class csFile {
		private File fl;
		private StreamReader ts;
		public csFile(string filename) 	
		{
			ts = File.OpenText  (filename);
			
		}
		public string readLine() {
			return ts.ReadLine ();
		}
		public void close() {
			ts.Close ();
		}
	}
}
