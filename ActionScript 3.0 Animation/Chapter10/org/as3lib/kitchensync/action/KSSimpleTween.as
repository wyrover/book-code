package org.as3lib.kitchensync.action
{	
	import flash.events.EventDispatcher;
	
	import org.as3lib.kitchensync.core.KitchenSyncEvent;
	import org.as3lib.kitchensync.core.Synchronizer;
	import org.as3lib.kitchensync.core.Timestamp;
	import org.as3lib.kitchensync.easing.EasingUtil;
	import org.as3lib.kitchensync.easing.Linear;

	
	/**
	 * A simplified version of the KSTween class with NO bells or whistles. Designed for optimized performance,
	 * file size, and minimal memory use. 
	 * 
	 * @use
	 * <code>
	 * var target:Object = targetObject; // this is the object whose property you want to control.
	 * var property:String = "x"; // A string representing the name of the property to control. 
	 * var tween:IAction = new SimpleTween(target, property, 0, 500, 3000, 1000, Cubic.easeOut);
	 * tween.start();
	 * 
	 * //Note, this class has 0 references to other classes except when running so it will be garbage collected
	 * //when it is finished playing if there are no other references to it. To create an auto-deleting version,
	 * //use the following syntax:
	 * new SimpleTween(target, property, 0, 500, 3000, 1000, Cubic.easeOut).start();
	 * </code>
	 * 
	 * @see org.as3lib.kitchensync.action.KSTween;
	 * @see org.as3lib.kitchensync.easing;
	 * @author Mims Wright
	 * @since 1.6
	 */
	public class KSSimpleTween extends EventDispatcher implements IAction, ITween
	{
		/** duration of tween, not including delay, in milliseconds. */ 
		public function get duration():int { return _duration; }
		public function set duration(duration:*):void { _duration = int(duration); }
		protected var _duration:int = 0;
		
		/** delay before the animation begins in milliseconds. */
		public function get delay():int { return _delay; }
		public function set delay(delay:*):void { _delay = int(delay); }
		protected var _delay:int = 0;
		
		/** target object whose properties will be affected. */
		public var target:Object;
		
		/** property of the target object to affect as a string. */
		public var property:String;
		
		/** the starting value of the tween. */
		public var startValue:Number;
		
		/** the ending value of the tween. */
		public var endValue:Number;
		
		/** a cached value for the difference between the start and end. */
		protected var _delta:Number;
		
		/** the easing function to use with the tween */
		public var easingFunction:Function;
		
		/** The time at which the tween was started. */
		protected var _startTime:int;
		
		/** The time at which the tween was last paused. */
		protected var _pauseTime:int;
		
		/** Set to true internally if when the start() method is called (false when stopped). */
		protected var _running:Boolean = false;
		/** Set to true internally when the puase() mehtod is called (false when unpaused) */
		protected var _paused:Boolean = false;
		
		/** True when the action is running (or paused) */
		public function get isRunning():Boolean { return _running; }
		
		/** True when the action is paused */ 
		public function get isPaused():Boolean { return _paused; }
		
		/**
		 * Constuctor.
		 * 
		 * @param target - the object whose property will be changed.
		 * @param property - the name of the property to change. The property must be a Number, int or uint such as a Sprite object's "alpha"
		 * @param startValue - the value to tween the property to. After the tween is done, this will be the value of the property.
		 * @param endValue - the starting value of the tween.
		 * @param duration - the time in milliseconds that this tween will take to execute.
		 * @param delay - the time to wait in milliseconds before starting the tween.
		 * @param easingFunction - the function to use to interpolate the values between fromValue and toValue.
		 */
		public function KSSimpleTween(target:Object, property:String, startValue:Number, endValue:Number, duration:int, delay:int, easingFunction:Function = null) {
			this.target = target;
			this.property = property;
			this.startValue = startValue;
			this.endValue = endValue;
			_duration = duration;
			_delay = delay;
			if (easingFunction == null) { easingFunction = Linear.ease; }
			this.easingFunction = easingFunction;
		}

		/** Called when a pulse is sent from the Synchronizer */
		public function update(currentTimestamp:Timestamp):void {
			var currentTime:int = currentTimestamp.currentTime;
			var timeElapsed:int = currentTime - _startTime;
			
			// timeElapsed shouldn't exceed the duration.
			timeElapsed = Math.min(timeElapsed, _duration);
			
			// if the delay is passed,
			if (timeElapsed >= 0) {
				
				// invoke the easing function.
				var result:Number =  EasingUtil.call(easingFunction, timeElapsed, _duration); 
				
				target[property] = result * (endValue - startValue) + startValue;
				
				// if the tween's duration is complete.
				if (timeElapsed >= _duration) {
					// end the tween.
					complete();
				}
			}
		}
		
		/**
		 * Starts the tween.
		 * @return IAction - returns self (for convenience)
		 */
		public function start():IAction {
			if (!_running) {
				// get the current timestamp
				var currentTimestamp:Timestamp = Synchronizer.getInstance().currentTimestamp;
				// cache the delta
				_delta = endValue - startValue;
				// record the start time
				_startTime = currentTimestamp.currentTime + _delay;
				// register the class.
				Synchronizer.getInstance().registerClient(this);
				_running = true;
				// force the first update.
				update(currentTimestamp);
				
				dispatchEvent(new KitchenSyncEvent(KitchenSyncEvent.START, currentTimestamp));
			}
			return this;
		}
		
		/**
		 * Stops the tween. All progress will be lost and the tween will restart from the startValue
		 * if start() is called again.
		 */
		public function stop():void {
			Synchronizer.getInstance().unregisterClient(this);
		}
		
		/** Called internally when the tween is completed. */
		protected function complete():void {
			stop();
			dispatchEvent(new KitchenSyncEvent(KitchenSyncEvent.COMPLETE, Synchronizer.getInstance().currentTimestamp));
		}
		
		
		public function pause():void {
			if (!_running && !_paused) {
				var currentTimestamp:Timestamp = Synchronizer.getInstance().currentTimestamp;
				_pauseTime = currentTimestamp.currentTime;
				_paused = true;
				Synchronizer.getInstance().unregisterClient(this);
				dispatchEvent(new KitchenSyncEvent(KitchenSyncEvent.PAUSE, currentTimestamp));
			}
		}
		
		/**
		 * Resumes the action at the point where it was paused.
		 */
		public function unpause():void {
			if (_running && _paused) {
				Synchronizer.getInstance().registerClient(this);
				_paused = false;
				var currentTimestamp:Timestamp = Synchronizer.getInstance().currentTimestamp;
				var timeSincePause:int = currentTimestamp.currentTime - _pauseTime;
				_startTime = _startTime + timeSincePause; 
				dispatchEvent(new KitchenSyncEvent(KitchenSyncEvent.UNPAUSE, currentTimestamp));
			}
		}
		
		
		public function kill():void {
			target = null;
			easingFunction = null;
		}
		
		public function clone():IAction {
			return new KSSimpleTween(target, property, startValue, endValue, duration, delay, easingFunction);
		}
		
		override public function toString():String {
			return "SimpleTween :" + this.target.toString() + "[" + this.property + "]";
		}
	}
}