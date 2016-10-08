package org.as3lib.kitchensync.core
{
	import flash.events.Event;

	/**
	 * Event type that is dispatched by the Synchronizer and AbstractSynchronizedActions. 
	 */
	public class KitchenSyncEvent extends Event
	{
		public static const START:String = "actionStart";
		public static const PAUSE:String = "actionPause";
		public static const UNPAUSE:String = "actionUnpause";
		public static const UPDATE:String = "synchronizerUpdate";
		public static const COMPLETE:String = "actionComplete";
		public static const CHILD_START:String = "childActionStart";
		public static const CHILD_COMPLETE:String = "childActionComplete";
		
		private var _timestamp:Timestamp;
		public function get timestamp():Timestamp { return _timestamp }
		
		public function KitchenSyncEvent(type:String, timestamp:Timestamp = null) {
			super(type, false, false);
			_timestamp = timestamp;
		}
		
	}
}