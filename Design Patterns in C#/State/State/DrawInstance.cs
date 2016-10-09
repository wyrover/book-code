using System;

namespace State
{
	/// <summary>
	/// Summary description for DrawInstance.
	/// </summary>
	public class DrawInstance :Memento {
		private int intg;
		private Mediator med;
		//-----
		public DrawInstance(int intg, Mediator md) 	{
			this.intg = intg;
			med = md;
		}
		//-----
		public int integ {
			get { return intg;
			}
		}
		//-----
		public void restore() {
			med.removeDrawing(intg);
		}
	}
}
