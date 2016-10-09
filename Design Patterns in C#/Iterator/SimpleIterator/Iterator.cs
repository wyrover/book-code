using System;

namespace SimpleIterator
{
	/// <summary>
	/// Summary description for Iterator.
	/// </summary>
	public interface Iterator 	{
		object First();
		object Next();
		bool isDone();
		object currentItem();
	}
}
