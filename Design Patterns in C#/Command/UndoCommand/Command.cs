using System;

namespace UndoCommand
{
	/// <summary>
	/// defines Command interface
	/// </summary>
	public interface Command 	{
		void Execute();
		void Undo();
		bool isUndo();
	}
}
