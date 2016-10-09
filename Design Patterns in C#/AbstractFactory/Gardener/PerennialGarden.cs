using System;

namespace Gardener
{
	/// <summary>
	/// Summary description for PerennialGarden.
	/// </summary>
	public class PerennialGarden : Garden
	{
		public PerennialGarden() {
			shade = new Plant("Astilbe");
			border = new Plant ("Dicentrum");
			center = new Plant ("Sedum");
		}
	}
}
