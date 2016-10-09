using System;
using CsharpPats;
namespace State
{
	/// <summary>
	/// save the state of a visual rectangle
	/// </summary>
	public class DrawMemento : Memento 	{
		private int x, y, w, h;
		private Rectangle  rect;
		private Drawing visDraw;
		//------
		public DrawMemento(Drawing d) 	{
			 visDraw = d;
			rect = visDraw.getRects ();
			x = rect.x;
			y = rect.y ;  
			w = rect.w;
			h = rect.h;
		}
		//-----
		public void restore() {
			 //restore the state of a drawing object
			rect.x = x;
			rect.y = y;
			rect.h = h;
			rect.w = w;
			visDraw.setRects( rect);
		}
	}
}
