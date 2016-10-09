using System;
using CsharpPats;

namespace Memento
{
	/// <summary>
	/// Command interface
	/// </summary>
	public interface Command
	{
		void Execute();
	}
}
