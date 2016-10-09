using System;

namespace BasicBridge
{
	/// <summary>
	/// Summary description for VisList.
	/// </summary>
	public interface VisList 	{
		//add a line to the display
		void addLine(Product p);
		//remove a line from the display
		void removeLine(int num);
	}
}
