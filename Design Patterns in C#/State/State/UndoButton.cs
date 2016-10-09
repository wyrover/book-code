using System;

namespace State
{
	/// <summary>
	/// Summary description for UndoButton.
	/// </summary>
	public class UndoButton :ComdToolBarButton
	{
		public UndoButton(Mediator md):base("Undo", md) {
		 }
		public override void Execute() {
			med.undo();
		}
	}
}
