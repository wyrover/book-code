using System;

namespace FFT
{
	/// <summary>
	/// Summary description for Complex.
	/// </summary>
	public class Complex
	{
			float real;
			float imag;
	   //---------------------------------
			public Complex(float r, float i) {
				real = r;                              
				imag = i;
			}
		//---------------------------------
		public void setReal(float r) {
			real = r;
		}
		//---------------------------------
		public void setImag(float i) {
			imag= i;
		}
		//---------------------------------
		public float getReal() {
			return real;
		}
		//---------------------------------
		public float getImag() {
			return imag;
		}

		
	}
}
