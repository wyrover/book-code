using System;
using System.Collections ;
using CsharpPats;
namespace Strategy
{
	/// <summary>
	/// Selects which plot strategy to carry out
	/// </summary>
	public class Context 	{
		float[] x, y;
		PlotStrategy plts;	//strategy selected goes here
		//-----
		public void plot() {
			readFile();		//read in data
			plts.plot (x, y);
		}
		//-----
		//select bar plot
		public void setBarPlot() {
			plts = new BarPlotStrategy ();
		}
		//-----
		//select line plot
		public void setLinePlot() {
			plts = new LinePlotStrategy();
		}
		//-----
		public void readFile() 		{
			ArrayList xc = new ArrayList();
			ArrayList yc = new ArrayList();
                        
        //reads data in from data file
			csFile fl = new csFile("data.txt");
			fl.OpenForRead();
			string sline = fl.readLine ();
			while (sline != null) {
				int i = sline.IndexOf(" ");
				if (i > 0) {
					float tmp = Convert.ToSingle (sline.Substring (0, i));
					xc.Add(tmp);  
					tmp = Convert.ToSingle (sline.Substring(i + 1));
					yc.Add(tmp);
				}
				sline = fl.readLine();
			}
			//copy into arrays from collections
			float[] xp = new float[xc.Count];
			float[] yp = new float[yc.Count];
			for (int i = 0; i< xc.Count; i++) {
				xp[i] = (float)xc[i];
				yp[i] = (float)yc[i];
			}
			x = xp;
			y = yp;
			fl.close();			
		}
	}
}
