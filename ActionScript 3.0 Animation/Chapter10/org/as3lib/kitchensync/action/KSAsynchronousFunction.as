package org.as3lib.kitchensync.action
{
	import flash.events.*;
	
	import org.as3lib.kitchensync.core.*;
	
	/**
	 * An action for calling an asynchronous function that is expected to dipatch an event when
	 * it completes.
	 * 
	 * @author Mims Wright
	 * @since 1.5
	 */
	public class KSAsynchronousFunction extends KSFunction
	{
		/** a reference to the event dispatcher */
		protected var _completeEventDispatcher:IEventDispatcher;
		/** the type of event to listen for */
		protected var _completeEventType:String;
		
		/**
		 * Constructor.
		 * 
		 * @param delay The time to wait before calling the function.
		 * @param func The function to call
		 * @param completeEventDispatcher The IEventDispatcher that will fire the event signaling that the 
		 * 								  function is complete.
		 * @param completeEventType The type (name) of the event that will be fired when complete.
		 * @param args All additional parameters will be passed as arguments to the function when it is called. 
		 */
		public function KSAsynchronousFunction(delay:*, func:Function, completeEventDispatcher:IEventDispatcher, completeEventType:String, ...args)
		{
			super(delay, func);
			this._args = args;
			
			_completeEventDispatcher = completeEventDispatcher;
			_completeEventType = completeEventType;
			
			_completeEventDispatcher.addEventListener(_completeEventType, onFunctionComplete, false, 0, true);
		}
		
		/**
		 * Executes the function when the delay has elapsed.
		 */
		override public function update(currentTimestamp:Timestamp):void {
			if (startTimeHasElapsed) {
				invoke();
				unregister();
			}
		}
		
		/**
		 * Event listener that is called when the asyncronous function is completed.
		 */
		protected function onFunctionComplete(event:Event):void {
			_completeEventDispatcher.removeEventListener(_completeEventType, onFunctionComplete);
			complete();
		}
		
		override public function clone():IAction {
			var clone:KSAsynchronousFunction = new KSAsynchronousFunction(_delay, _func, _completeEventDispatcher, _completeEventType);
			clone._args = _args;
			clone.duration = _duration;
			clone.autoDelete = _autoDelete;
			return clone;
		}
		
		override public function kill():void {
			super.kill();
			_completeEventDispatcher = null;
		}
		
	}
}