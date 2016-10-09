using System;

namespace UndoCommand
    {
	/// <summary>
	/// Summary description for CommandHolder.
	/// </summary>
	public interface CommandHolder 	{
		Command getCommand();
		void setCommand(Command cmd);
	}
}
