using System;

namespace CsharpPats
{
	//String Tokenizer class
	public class StringTokenizer 	{
		private string data, delimiter;
		private string[] tokens;
		private int index;
		public StringTokenizer(string dataLine) 		{
			init(dataLine, " ");
				}
		private void init(String dataLine, string delim) {
			delimiter = delim;
			data = dataLine;
			tokens = data.Split (delimiter.ToCharArray() );
			index = 0;
		}
		public StringTokenizer(string dataLine, string delim) {
   		init(dataLine, delim);	
		}
		public bool hasMoreElements() {
			return (index < (tokens.Length));
		}
		public string nextToken() {
			return nextElement();
		}
		public string nextElement() {
			string s = tokens[index++].Trim ();
			while((s.Length <=0) && (index<tokens.Length ))
				s = tokens[index++].Trim ();
		return s;
		}
	}
}
