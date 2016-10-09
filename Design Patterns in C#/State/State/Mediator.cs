using System;
using System.Windows.Forms;
using System.Drawing ;
using System.Collections ;
namespace State
{
	/// <summary>
	/// Summary description for Mediator.
	/// </summary>
	public class Mediator 	{
		private bool startRect;
		private int selectedIndex;
		private RectButton rectb;
		private bool dSelected;
		private ArrayList drawings;
		private ArrayList undoList;
		private RectButton rButton;
		private FillButton filButton;
		private CircleButton circButton;
	    private PickButton arrowButton;
		private PictureBox canvas;
		private int selectedDrawing;
		private StateManager stMgr;
		//-----
		public Mediator(PictureBox pic) 		{
			startRect = false;
			dSelected = false;
			drawings = new ArrayList();
			undoList = new ArrayList();
			stMgr = new StateManager(this);
			canvas = pic;
			selectedDrawing = -1;
		}
		//-----
		public void startRectangle() {
			stMgr.setRect();
			arrowButton.setSelected(false);
			circButton.setSelected(false);
			filButton.setSelected(false);
		}
		//-----
		public void startCircle() {
			stMgr.setCircle();
			rectb.setSelected(false);
			arrowButton.setSelected(false);
			filButton.setSelected(false);
		}
		//------
		public void startFill() {
			stMgr.setFill();
			rectb.setSelected(false);
			circButton.setSelected(false);
			arrowButton.setSelected(false);
			if (selectedDrawing >= 0) {
				stMgr.selectOne(getDrawing(selectedDrawing));
				FillMemento m = new FillMemento(selectedDrawing, this);
				undoList.Add(m);
			}
        repaint();
		}
		//-----
		public void startArrow() {
			stMgr.setArrow();
			rectb.setSelected(false);
			circButton.setSelected(false);
			filButton.setSelected(false);
		}
		//------
		public Drawing getSelected() {
			return (Drawing)drawings[selectedDrawing];
		}
		//-----
		public void setSelected(int index) {
			dSelected = true;
			selectedDrawing = index;
			Drawing d = getDrawing(index);
			d.setSelected(true);
			repaint();
		}
		//-----
		public void fillSelected() {
			if (dSelected) {
            Drawing d = (Drawing)drawings[selectedDrawing];
            d.setFill(true);
            FillMemento m = new FillMemento(selectedDrawing, this);
			undoList.Add(m);
			}
		}
		//-----
		public void undo() {
			rectb.setSelected(false);
			circButton.setSelected(false);
			arrowButton.setSelected(false);
        
			if (undoList.Count > 0) {
				//get last element in undo list
				object obj = undoList[undoList.Count - 1];
				undoList.RemoveAt(undoList.Count - 1);
				//get the Memento
				Memento m = (Memento)obj;
				m.restore();   //and restore the old position
				repaint();
			}
		}
		//-----
		public ArrayList getDrawings() {
			return drawings;
		}
		public Drawing getDrawing(int a ) {
			if(a < drawings.Count )
				return (Drawing) drawings[a];
			else
				return null;
		}
		//-----
		public void addDrawing(Drawing d) {
			drawings.Add (d);
			DrawInstance intc = new DrawInstance(drawings.Count-1, this);
			undoList.Add(intc);
		}
		public void clear() {
			drawings = new ArrayList();
			undoList = new ArrayList();
			dSelected = false;
			selectedDrawing = 0;
			repaint();
		}
		//-----
		public void unpick() {
			 dSelected = false;
			if (selectedDrawing >= 0) {
				Drawing d = getDrawing(selectedDrawing);
				if (d != null) {
					d.setSelected(false);
				}
				selectedDrawing = -1;
				repaint();
			}
		}
		//-----
		private void repaint() {
			canvas.Refresh ();
		}
		public void registerRectButton(RectButton rb) {
			rectb = rb;
		}
		public void registerFillButton(FillButton fb) {
			filButton = fb;
		}
		public void registerArrowButton (PickButton ab) {
			arrowButton = ab;
		}
		public void registerCircleButton(CircleButton cb) {
			circButton = cb;
		}
		//-----
		public void removeDrawing(int a) {
			drawings.RemoveAt (a);
		}
		//-----
		public int findDrawing(int x, int y ) {
			int i = 0;
			bool found = false;
        int index = -1;
			while (i < drawings.Count && ! found) {
				Drawing d = getDrawing(i);
				if (d.contains(x, y)) {
					index = i;
					found = true;
				}
				i++;
			}
        return index;
		}
		//-----
		public void rememberPosition() {
			if (dSelected) {
				Drawing d  = (Drawing)drawings[selectedDrawing];
				Memento m = new DrawMemento (d);
				undoList.Add (m);
			}
		}
		//-----
		public void mouseUp(int x, int y) {
			stMgr.mouseUp (x, y);
		}
		public void mouseDown(int x, int y ) {
			stMgr.mouseDown (x, y);
			repaint();
		}
		public void mouseDrag(int x, int y) {
			stMgr.mouseDrag (x, y);
			repaint();
		}
		//-----
		public void reDraw(Graphics g) {
			
			foreach(object obj in drawings) {
				Drawing d = (Drawing)obj;
				d.draw(g);
			}
		}
	}
}
