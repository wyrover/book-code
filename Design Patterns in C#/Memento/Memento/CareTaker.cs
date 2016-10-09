using System;
using System.Collections ;
namespace Memento
{
	/// <summary>
	/// Summary description for CareTaker.
	/// </summary>
	public class CareTaker
	{
		private ArrayList drawings, undoList;
		public CareTaker(ArrayList dcol)
		{
			clear(dcol);
		}
		public void rememberPosition(VisRectangle vr) {
			Memento mem = new Memento (vr);
			undoList.Add (mem);
		}
		public void clear(ArrayList drw) {
			drawings = drw;
			undoList = new ArrayList();
		}
		public void Add(int intg) {
			undoList.Add (intg);
		}
		public void removeDrawing() {
			drawings.RemoveAt (drawings.Count -1);
		}
		public void remove(Memento mem) {
			mem.restore ();
		}
		public void remove(int intg) {
			removeDrawing();
		}
		public void undo() {
			if(undoList.Count > 0) {
				int last = undoList.Count -1;
				object obj = undoList[last];
				try{
					Memento mem = (Memento)obj;
					remove(mem);
				}
				catch (Exception) {
					removeDrawing();
				}
				undoList.RemoveAt (last);
			}
		}
	}
}
