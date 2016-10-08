package org.as3lib.kitchensync.action
{
	import flash.errors.IllegalOperationError;
	
	import org.as3lib.kitchensync.core.*;
	
	/**
	 * Packages up a function to be run at a specified time or as part of a sequence.
	 * Setting _duration to anything but zero will cause the function to repeat every frame until the duration is reached.
	 */
	public class KSFunction extends AbstractAction
	{
		/**
		 * The function that will be called by the action.
		 */
		public function set func(func:Function):void { _func = func; }
		protected var _func:Function;
		
		override public function set duration(duration:*):void { 
			throw new IllegalOperationError("Duration cannot be set for KSFunction"); 
		}
		
		/**
		 * Arguments that will be passed into the function.
		 */
		protected var _args:Array;
		
		/**
		 * The result of the function (if the function generates one.
		 */		
		public function get result():* { return _result; }
		protected var _result:*;
		
		/**
		 * Constructor.
		 * 
		 * @param delay - the number of frames to delay the action
		 * @param func - the function or method to call at the specified time
		 * @param args - the rest of the parameters become arguments passed to the function at the time it's called.
		 */
		public function KSFunction(delay:*, func:Function, ... args)
		{
			super();
			this.delay = delay;
			_duration = 0;
			_func = func;
			_args = args;
		}
		
		/**
		 * Calls the function with the arguments specified. The result of the function is stored in the 
		 * results property of the SynchronizedFunction object.
		 * 
		 * @returns the results from the function call.
		 */
		public function invoke():* {
			_result = _func.apply(this, _args);
			return _result;
		}
		
		/**
		 * Executes the function when the delay has elapsed.
		 * If the duration is > 0, it will repeat until the duration has elapsed.
		 */
		override public function update(currentTimestamp:Timestamp):void {
			if (startTimeHasElapsed) {
				invoke();
				if (durationHasElapsed) {
					complete();
				}
			}
		}
		
		override public function clone():IAction {
			var clone:KSFunction = new KSFunction(_delay, _func);
			clone._args = _args;
			clone._result = _result;
			clone.autoDelete = _autoDelete;
			return clone;
		}
		
		override public function kill():void {
			super.kill();
			_func = null;
			_args = null;
			_result = null;
		} 
	}
}