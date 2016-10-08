package org.as3lib.kitchensync.core
{
	import flash.display.DisplayObject;
	import flash.display.Stage;
	import flash.events.Event;
	import flash.events.EventDispatcher;
	import flash.events.TimerEvent;
	import flash.utils.Dictionary;
	import flash.utils.Timer;
	import flash.utils.getTimer;
	
	import org.as3lib.kitchensync.KitchenSyncDefaults;
	
	
	[Event(name="synchronizerUpdate", type="org.as3lib.kitchensync.KitchenSyncEvent")]
	
	/**
	 * Synchronizer acts as the main time keeper for the animation engine. 
	 * 
	 * @author Mims H. Wright
	 * @since 0.1
	 */
	public final class Synchronizer extends EventDispatcher
	{
		
		private static var _instance:Synchronizer = null;
		private var _stage:Stage = null;
		
		private var _timer:Timer;
		private var _timerDelay:int = 10;
		public function get timerDelay():int { return _timerDelay; }
				
		private var _frames:Number = 0;
		private var _currentTime:Number = 0;
		//private var _active:Boolean = true;
		//public function get active ():Boolean { return _active; }
		//public function set active (active:Boolean):void { _active = active; }
		
		/** A list of clients that are registered to listen for updates. */
		private var _clients:Dictionary = new Dictionary(KitchenSyncDefaults.syncrhonizerUsesWeakReferences);
		
		/** The frameRate (as defined in the stage) */
		public function get frameRate():int {return _stage.frameRate; }
		
		
		/**
		 * Constructor. SingletonEnforcer prevents this class from being instantiated without
		 * using the initialize() method.
		 * 
		 * @param enforcer - a SingletonEnforcer can only be created internally.
		 */
		public function Synchronizer(enforcer:SingletonEnforcer) { 
			super(); 
		}
		
		/**
		 * Sets the framerate seed for the synchronizer.
		 * @param frameRateSeed must be a DisplayObject that is added to the display list.
		 * @since 1.2
		 */ 
		public function set frameRateSeed(frameRateSeed:DisplayObject):void {
			if (frameRateSeed && frameRateSeed.stage) {
				_stage = frameRateSeed.stage;
				_stage.addEventListener(Event.ENTER_FRAME, _instance.onEnterFrame, false, int.MAX_VALUE, false);
				_timerDelay = Math.floor(1/frameRate*1000);
				_timer = new Timer(_timerDelay);
				_timer.addEventListener(TimerEvent.TIMER, onTimer);
				_timer.start();
			} else {
				throw new ArgumentError("frameRateSeed must be a DisplayObject that is part of the Display List.");
			}
		}
		
		
		/**
		 * Returns an instance to the single instance of the class. 
		 * 
		 * @return a reference to the only instance of the Synchronizer.
		 */
		public static function getInstance():Synchronizer {
			if (_instance == null) {
				_instance = new Synchronizer(new SingletonEnforcer()); 
			}
			return _instance;
		}
		
		/**
		 * Adds a Syncrhonizer client to the list that will be updated when the dispatchUpdate() method is called.
		 * 
		 * @param client The client that will receive the update.
		 */ 
		public function registerClient(client:ISynchronizerClient):void {
			_clients[client] = client;
		}
		
		/**
		 * Removes a Syncrhonizer client from the list.
		 * 
		 * @param client The client that will be unregistered.
		 */ 
		public function unregisterClient(client:ISynchronizerClient):void {
			delete _clients[client];
		}
		
		
		/**
		 * Triggered by every passing frame of the Stage. Rebroadcasts the event with additional
		 * information about the time at which it occurred. 
		 */
		private function onEnterFrame(event:Event = null):void {
			_frames++;
			//dispatchUpdate();
		}
		
		private function onTimer(event:TimerEvent):void {
			dispatchUpdate();
		}
		
		/**
		 * Dispatches events to children for them to update.
		 */
		private function dispatchUpdate():void {
			_currentTime = getTimer();
			
			// cache the timestamp so that it's only generated once. 
			var currentTimestampCache:Timestamp = currentTimestamp; 
			
			// update registered clients.
			for each (var client:ISynchronizerClient in _clients) {
				client.update(currentTimestampCache);
			}
			
			// dispatch event to event listeners.
			dispatchEvent(new KitchenSyncEvent(KitchenSyncEvent.UPDATE, currentTimestampCache));
			
			//trace(currentTimestamp);			
		}
		
		/**
		 * Returns the current time as a timestamp object.
		 */
		public function get currentTimestamp():Timestamp {
			return new Timestamp(_frames, _currentTime);
		}
		
	}
}
class SingletonEnforcer {}