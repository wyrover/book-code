package org.as3lib.kitchensync.action
{	
	/**
	 * A set of commands to be used in conjunction with the ActionController class.
	 * 
	 * @see ActionController
	 */
	public class ActionControllerCommand
	{
		/** start() command. */
		public static const START:ActionControllerCommand = new ActionControllerCommand("start");
		
		/** pause() command. */
		public static const PAUSE:ActionControllerCommand = new ActionControllerCommand("pause");

		/** unpause() command. */
		public static const UNPAUSE:ActionControllerCommand = new ActionControllerCommand("unpause");

		/** stop() command. */
		public static const STOP:ActionControllerCommand = new ActionControllerCommand("stop");

		/** kill() command. */
		public static const KILL:ActionControllerCommand = new ActionControllerCommand("kill");

		/** reset() command. For Tween objects only. */
		public static const RESET:ActionControllerCommand = new ActionControllerCommand("reset");
		
		/** The default command will be used if nothing is specified. */
		public static var DEFAULT:ActionControllerCommand = START;
		
		// The string equivelant of the Command
		private var _string:String;
		
		public function ActionControllerCommand(string:String = "") {
			_string = string;
		} 
		
		public function toString():String {
			if (!_string) { return super.toString(); }
			return _string;
		}
	}
}