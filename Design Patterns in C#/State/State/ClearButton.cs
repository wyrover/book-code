using System;

namespace State
{
	/// <summary>
	/// Summary description for ClearButton.
	/// </summary>
	public class ClearButton:ComdToolBarButton
	{
		public ClearButton(Mediator md):base("Clear", md)	{}
		public override void Execute() {
			med.clear();
		}
	}
}
