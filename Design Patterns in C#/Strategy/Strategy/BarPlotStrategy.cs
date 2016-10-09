using System;

namespace Strategy
{
	/// <summary>
	/// Summary description for BarPlotStrategy.
	/// </summary>
	public class BarPlotStrategy :PlotStrategy
	{
		public override void plot(float[] xp, float[] yp) {
			BarPlot bplot = new BarPlot ();
			bplot.Show ();
			bplot.plot (xp, yp);
		}
	}
}
