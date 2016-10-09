using System;

namespace Memento
{
	/// <summary>
	/// Summary description for Memento.
	/// </summary>
	public class Memento 	{
		private int x, y, w, h;
		private CsharpPats.Rectangle rect;
		private VisRectangle visRect;
		//------
    	public Memento(VisRectangle vrect) 		{
			visRect = vrect;
			rect = visRect.rects ;
			x = rect.x ;
			y = rect.y;
			w = rect.w;
			h = rect.h;
		}
		//------
		public void restore() {
			rect.x = x;
			rect.y = y;
			rect.h = h;
			rect.w = w;
			visRect.rects = rect;
		}
	}
}
