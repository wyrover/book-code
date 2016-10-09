using System;

namespace FFT
{
	/// <summary>
	///Computes the complex multiplications for the FFT
	/// </summary>
	public abstract class Butterfly
	{
			float y;
			public Butterfly() {
			}
			public Butterfly(float angle) {
				y = angle;
			}
			abstract public void Execute(Complex x, Complex y);
	}
}
