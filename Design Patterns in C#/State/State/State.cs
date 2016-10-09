using System;

namespace State
{
	/// <summary>
	///empty base class containing State methods to override
	/// </summary>
	public class State     {
		//keeps state of each button
		protected Mediator med;
		public State(Mediator md) {
			med = md;	//save reference to mediator
		}
		public virtual void mouseDown(int x, int y) {}
		public virtual void mouseUp(int x, int y) {	}
		public virtual void mouseDrag(int x, int y) {}
		public virtual void selectOne(Drawing d) {}
	}
}
