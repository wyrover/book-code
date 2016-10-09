using System;

namespace Strategy
{
	/// <summary>
	/// Summary description for LinePlotStrategy.
	/// </summary>
	public class LinePlotStrategy : PlotStrategy 	{
		public override void plot(float[] x, float[] y) {
			LinePlot lplt = new LinePlot();
			lplt.Show ();
			lplt.plot (x, y);
		}
	}
}
