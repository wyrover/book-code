package org.as3lib.kitchensync.action
{
	import flash.events.Event;
	import flash.events.IEventDispatcher;
	
	import org.as3lib.kitchensync.core.*;
	
	/**
	 * An object that causes an event to be dispatched at the time of execution.
	 * An AS3 Event can be dispatched after a delay or as part of a sequence using this class. 
	 * Events are dispatched using the AS3 event dispatching system and can be sent from the 
	 * SynchronizedDispatchEvent itself or from any other IEventDispatcher. The constructor can 
	 * be passed either an Event object or a string for the event type.
	 * Uses weak references by default.
	 */
	public class KSDispatchEvent extends AbstractAction
	{
		public static const SELF:IEventDispatcher = null;
		
		/** 
		 * Set to false if you want to keep a strong reference to the event being dispatched.
		 * Default is true.
		 * @todo - if this is set to false, cleanup event listeners on <code>kill()</code>
		 */
		public function get useWeakReferences():Boolean { return _useWeakReferences; }
		public function set useWeakReferences(useWeakreferences:Boolean):void { _useWeakReferences = useWeakreferences; }
		protected var _useWeakReferences:Boolean = true;
		
		/**
		 * This is the IEventDispatcher that the event will be dispatched from. Default is <code>this</code>. 
		 */ 
		public function get target ():IEventDispatcher { return _target; }
		public function set target (target:IEventDispatcher):void { _target = target; }
		protected var _target:IEventDispatcher;
		
		/**
		 * The event that will be dispatched.
		 */
		public function get event():Event { return _event; }
		public function set event(event:Event):void { _event = event; }
		protected var _event:Event;
		
		/**
		 * Using this will set the event to a generic Event object with type of <code>type</code>.
		 */
		public function set eventType(type:String):void {
			_event = new Event(type);
		}
		
		/**
		 * Constructor.
		 * 
		 * @throws TypeError - If any objects in listeners are not of type Function.
		 * 
		 * @param event - Can be either an Event object or a String. If event is an Event, that object is used.
		 * 				  If event is a string, a new event with that type is automatically created.
		 * @param target - The IEventDispatcher that will dispatch the event. The default is <code>this</code>.
		 * @param delay - time to wait before execution
		 * @param listeners - Any additional objects passed in will be added as listeners (if they're functions)
		 */
		public function KSDispatchEvent(event:*, target:IEventDispatcher = SELF, delay:* = 0, ... listeners) {
			super();
			
			if (event is Event) {
				_event = Event(event);
			} else if (event is String) {
				eventType = String(event);
			} else {
				throw new TypeError ("Invalid event parameter. Must be of type Event or String.");
			}
			
			if (target == null || target == SELF) {
				_target = this;
			} else {
				_target = target;
			}
			
			this.delay = delay;
			
			for (var i:int = 0; i < listeners.length; i++) {
				var func:Function = listeners[i] as Function;
				if (func != null) {
					addEventListenerToTarget(func);
				} else {
					throw new TypeError("All listeners must be of type Function.");
				}
			}
		}
		
		/**
		 * Allows you to add an event listener to the target that is dispatching the event.
		 * Note that useWeakReference will always be true so that the listeners don't need to be removed.
		 */
		public function addEventListenerToTarget(listener:Function, useCapture:Boolean=false, priority:int=0.0):void {
			_target.addEventListener(_event.type, listener, useCapture, priority, _useWeakReferences);
		}
		
		/**
		 * Allows you to remove an event listener from the target that is dispatching the event.
		 */
		public function removeEventListenerFromTarget(listener:Function, useCapture:Boolean=false):void {
			_target.removeEventListener(_event.type, listener, useCapture);
		}
		
		/**
		 * When the delay is reached, the event will be fired from the target.
		 * Note that if duration is > 0, this will continue to fire for every frame until duration is elapsed.
		 */
		override public function update(currentTimestamp:Timestamp):void {
			if (startTimeHasElapsed) {
				_target.dispatchEvent(_event);
				if (durationHasElapsed) {
					complete();
				}
			}
		}
		
		/**
		 * override to clean up references to other objects.
		 */
		override public function kill():void {
			super.kill();
			_target = null;
			_event = null;
		}
		
		override public function clone():IAction {
			var clone:KSDispatchEvent = new KSDispatchEvent(_event, _target, _delay);
			//clone.timeUnit = _timeUnit;
			clone.duration = _duration;
			clone.autoDelete = _autoDelete;
			clone.useWeakReferences = _useWeakReferences;
			return clone;
		}
	}
}