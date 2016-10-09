using System;
using System.Drawing ;

namespace Gardener
{
	/// <summary>
	/// Summary description for AnnualGarden.
	/// </summary>
	public class AnnualGarden : Garden
	{
		public AnnualGarden () {
			shade = new Plant("Coleus");
			border = new Plant ("Alyssum");
			center = new Plant ("Marigold");
		}
	}
}
