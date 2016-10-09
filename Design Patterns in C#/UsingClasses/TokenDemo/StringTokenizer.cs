using System;

namespace CsharpPats
{
	//String Tokenizer class
	public class StringTokenizer 	{
		private string data, delimiter;
		private string[] tokens;	//token array
		private int index;		//index to next token
		//----------
		public StringTokenizer(string dataLine) 		{
			init(dataLine, " ");
		}
		//----------
		//sets up initial values and splits string
		private void init(String dataLine, string delim) {
			delimiter = delim;
			data = dataLine;
			tokens = data.Split (delimiter.ToCharArray() );
			index = 0;
		}
		//----------
		public StringTokenizer(string dataLine, string delim) {
			init(dataLine, delim);	
		}
		//----------
		public bool hasMoreElements() {
			return (index < (tokens.Length));
		}
		//----------
		public string nextElement() {
			//get the next token
			if( index < tokens.Length )
				return tokens[index++];
			else
				return "";	//or none
		}
	}
}
