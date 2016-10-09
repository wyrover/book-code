using System;
using System.Windows.Forms;
using System.Collections ;
using System.Drawing;
namespace Memento
{
	/// <summary>
	/// Mediates events between buttonsb
	/// </summary>
	public class Mediator
	{
		private bool startRect;
		private bool rectSelected;
		private ArrayList drawings;
		private PictureBox canvas;
		private int selectedIndex;
		private CareTaker caretakr;
		private RectButton rect;
		public Mediator(PictureBox p) 		{
			startRect = false;
			rectSelected = false;
        
			drawings = new ArrayList();
			caretakr = new CareTaker(drawings);
			canvas = p;
		}
		//------
		public void startRectangle() {
			startRect = true;
		}
		//-----
		public void createRect(int x, int y) { 
			unpick();         //make sure no rectangle is selected
			if (startRect) {  //if rect button is depressed
				int count = drawings.Count;
				caretakr.Add(count);  //Save previous drawing list size
				VisRectangle v = new VisRectangle(x, y);    //create a rectangle
				drawings.Add(v);            //add new element to list
				startRect = false;          //done with this rectangle
				rect.setSelected(false);    //unclick button
				canvas.Refresh();
			}
			else
				pickRect(x, y);   //if not pressed look for rect to select
		}
		//-----
		public void registerRectButton(RectButton rb) {
			rect = rb;
		}
		//-----
		public void unpick() {
			if (rectSelected && (selectedIndex >= 0) && (selectedIndex < drawings.Count)) {
				VisRectangle vis = (VisRectangle) drawings[selectedIndex];
				vis.setSelected(false);
				selectedIndex = -1;
				rectSelected = false;
				canvas.Refresh();
			}
		}
		//-----
		public void pickRect(int x, int y) {
			//save current selected rectangle
			//to avoid double save of undo
			int lastPick = -1;     
			if (selectedIndex >= 0) {
				lastPick = selectedIndex;
			}
			unpick();  //undo any selection
			//see if one is being selected
			for (int i = 0; i< drawings.Count; i++) {
				VisRectangle v = (VisRectangle)drawings[i];
				if (v.contains(x, y)) { //did click inside a rectangle
					selectedIndex = i;     //save it
					rectSelected = true;
					if (selectedIndex != lastPick) { //but don't save twice
						caretakr.rememberPosition(v);
					}
					v.setSelected(true);    //turn on handles
					repaint();          //and redraw
				}			
			}
		}
		//-----
		public void clear() {
			 drawings = new ArrayList();
			caretakr.clear(drawings);
			rectSelected = false;
			selectedIndex = 0;
			repaint();
		}
	//-----
		private void repaint() {
			canvas.Refresh ();
		}
		//-----
		public void undo() {
			caretakr.undo ();
			repaint();
		}
		//-----
		public void reDraw(Graphics g) {
			for(int i=0; i < drawings.Count ; i++ ) {
				VisRectangle v = (VisRectangle)drawings[i];
				v.draw (g);
			}
		}
		//-----
		public void drag(int x, int y) {
			if (rectSelected) {
				VisRectangle v = (VisRectangle)drawings[selectedIndex];
				if(v.contains (x, y)) {
					v.move (x, y);
					repaint();
				}
			}
		}
	}
}
