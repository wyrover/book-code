using System;
using System.Drawing ;
using CsharpPats;
namespace State
{
	/// <summary>
	/// interface defining Drawing object
	/// </summary>
	public interface Drawing 	{
		void setSelected(bool b);
		void draw(Graphics g);
		void move(int xpt, int ypt );
		bool contains(int x,int y);
	    void setFill(bool b);
		CsharpPats.Rectangle getRects();
		void setRects(CsharpPats.Rectangle rect);
	}
}
