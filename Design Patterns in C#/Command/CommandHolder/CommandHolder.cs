using System;

namespace CHolder
{
	/// <summary>
	/// Summary description for CommandHolder.
	/// </summary>
	public interface CommandHolder 	{
		Command getCommand();
		void setCommand(Command cmd);
	}
}
