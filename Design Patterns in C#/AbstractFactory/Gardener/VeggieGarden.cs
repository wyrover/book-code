using System;

namespace Gardener
{
	/// <summary>
	/// Summary description for VeggieGarden.
	/// </summary>
	public class VeggieGarden : Garden 	{
		public VeggieGarden() {
			shade = new Plant("Broccoli");
			border = new Plant ("Peas");
			center = new Plant ("Corn");
		}
	}
}
