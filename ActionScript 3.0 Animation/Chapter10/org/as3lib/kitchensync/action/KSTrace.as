package org.as3lib.kitchensync.action
{
	/**
	 * Traces a message at the specified time.
	 */
	public class KSTrace extends KSFunction
	{
		/** The message to trace */
		public function get message():String { return _args[0]; }
		public function set message(message:String):void { _args[0] = message; }
		
		/**
		 * Constructor.
		 * @param message - the message to be displayed in the trace window.
		 * @param delay - the time to wait before tracing in frames.
		 */
		public function KSTrace(message:*, delay:* = 0)
		{
			super(delay, trace, message.toString());
		}
		
		override public function clone():IAction { 
			var clone:KSTrace = new KSTrace(message, _delay);
			clone.autoDelete = _autoDelete;
			return clone;
		}
		
	}
}