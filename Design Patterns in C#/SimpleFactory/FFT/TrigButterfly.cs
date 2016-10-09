using System;

namespace FFT
{
	/// <summary>
	/// Summary description for TrigButterfly.
	/// </summary>
	public class TrigButterfly:Butterfly
	{
		float y;
		float oldr1, oldi1;
		float cosy, siny;
		float r2cosy, r2siny, i2cosy, i2siny;
   
		public TrigButterfly(float angle) {
			y = angle;
			cosy = (float) Math.Cos(y);
			siny = (float)Math.Sin(y);
 
		}
		public override void Execute(Complex xi, Complex xj) {
			oldr1 = xi.getReal();
			oldi1 = xi.getImag();
			r2cosy = xj.getReal() * cosy;
			r2siny = xj.getReal() * siny;
			i2cosy = xj.getImag()*cosy;
			i2siny = xj.getImag()*siny;
			xi.setReal(oldr1 + r2cosy +i2siny);
			xi.setImag(oldi1 - r2siny +i2cosy);
			xj.setReal(oldr1 - r2cosy - i2siny);
			xj.setImag(oldi1 + r2siny - i2cosy);  
		}

	}
}
