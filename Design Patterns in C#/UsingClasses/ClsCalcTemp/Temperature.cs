using System;

namespace CalcTemp
{
	/// <summary>
	/// Summary description for Temperature.
	/// </summary>
	public class Temperature 	{
		private float temp, newTemp;
		//-------------
		public Temperature(string thisTemp) 		{
			temp = Convert.ToSingle(thisTemp);
		}
		//-------------
		public string getConvTemp(bool celsius){
		if (celsius) 
			return getCels();
		else
			return getFahr();
		}
		//-------------
		private string getCels() {
			newTemp= 5*(temp-32)/9;
			return newTemp.ToString() ;
		}
		//-------------
		private string getFahr() {
			newTemp = 9*temp/5 + 32;
			return Convert.ToString(newTemp) ;
		}	
		public string getKelvin() {
			float ttemp =  (float)(5*(temp - 32)/9 +273.16);
			return ttemp.ToString ();
		}
	}
}
