using System;

namespace State
{
	/// <summary>
	/// Summary description for FillMemento.
	/// </summary>
	public class FillMemento : Memento 	{
		private int index;
		private Mediator med;
		//-----
		public FillMemento(int dindex, Mediator md) {
			index = dindex;	
			med = md;
		}
		//-----
		public void restore() {
			Drawing d = med.getDrawing(index);
			d.setFill(false);
		}
	}
}
