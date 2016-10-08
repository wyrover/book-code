package org.as3lib.kitchensync.action
{
	import flash.utils.Dictionary;
	import flash.utils.getQualifiedClassName;
	
	import org.as3lib.kitchensync.core.KitchenSyncEvent;
	import org.as3lib.kitchensync.core.Timestamp;
	import org.as3lib.kitchensync.utils.TimestampUtil;
	
	/**
	 * A parallel group where all children END at the same time instead of starting
	 * at the same time. Instantaneous items will play at the end. 
	 * 
	 * @author Mims Wright
	 * @since 1.6
	 */
	public class KSSimultaneousEndGroup extends KSParallelGroup {
		private var _longestItemsTotalDuration:int;
		private var _childStartTimes:Dictionary;
		
		/** Constructor. */
		public function KSSimultaneousEndGroup (... children) {
			super();
			for (var i:int = 0; i < children.length; i++) {
				if (children[i] is IAction) {
					var action:IAction = IAction(children[i]);
					addAction(action); 
				} else {
					throw new TypeError ("All children must be of type IAction. Make sure you are not calling start() on the objects you've added to the group. Found " + getQualifiedClassName(children[i]) + " where IAction was expected.");
				}
			}
		}
		
		override public function update(currentTimestamp:Timestamp):void {
			if (startTimeHasElapsed) {
				var childAction:IAction;
				// if the group isn't already running...
				if (!childrenAreRunning) {
					// reset the number of running children.
					_runningChildren = 0;				
					// cache the longest duration
					_longestItemsTotalDuration = getLongestItemsTotalDuration();
					// reset the dictionary.
					_childStartTimes = new Dictionary(true);
					
					// for all child actions
					childAction = null;
					for each (childAction in childActions) {
						// add the start time to a dictionary.
						_childStartTimes[childAction] = calculateStartTime(childAction, _longestItemsTotalDuration);
						
						// add a listener to each action so that the completion of the entire group can be tracked.
						childAction.addEventListener(KitchenSyncEvent.START, onChildStart);
						childAction.addEventListener(KitchenSyncEvent.COMPLETE, onChildFinished);
						
						//trace("final start time: " + _childStartTimes[childAction]);
					}
					// once this has started, it doesn't need updates anymore.
					//unregister();
				}
				
				childAction = null;
				// Once it's running, for all child actions
				for each (childAction in childActions) {
					// check to see if start time has elapsed for the child.
					if (isChildStartTimeElapsed(childAction, currentTimestamp)) {
						// if so, start the child.
						childAction.start();
						// add one running child.
						_runningChildren++;
					}
				}
				// if all children are running, unregister the group.
				if (_runningChildren >= _childActions.length) { unregister(); }
			}
		}
		
		/**
		 * Determines if the child should start yet.
		 */
		 private function isChildStartTimeElapsed(childAction:IAction, currentTimestamp:Timestamp):Boolean {
		 	if (childAction.isRunning) { return false; }
		 	var startTime:int = _childStartTimes[childAction];
		 	//trace(startTime, "<=", currentTimestamp.currentTime);
			if (sync) {
				if (startTime <= currentTimestamp.currentTime) { return true; }
			} else {
				if (TimestampUtil.millisecondsToFrames(startTime) <= currentTimestamp.currentFrame) { return true; }
			}
			return false;
		 }
		
		/**
		 * Calculates the start time for an action using the following formula:
		 * S: start time for item.
		 * G: start time for the group.
		 * L: longest item's delay and duration.
		 * O: item's offset
		 * D: item's duration
		 * S = G + L - O - D
		 * 
		 * @param action The action to get the start time for.
		 * @param longestItemsTotalDuration The delay + duration of the longest item.
		 */
		 private function calculateStartTime(action:IAction, longestItemsTotalDuration:int):int {
		 	return _startTime.currentTime + longestItemsTotalDuration - action.delay - action.duration;
		 }
		
		/**
		 * Returns the combined duration and delay for the item(s) with the longest combined delay and duration.
		 * 
		 * @return int The longest time of any child action.
		 */
		private function getLongestItemsTotalDuration():int {
			var longestTime:int = 0;
			var currentTime:int = 0;
			if (childActions.length > 0) {
				for each (var action:IAction in childActions) {
					// get combined delay and duration.
					currentTime = action.delay + action.duration;
					// if it's the longest, save the value. 
					longestTime = currentTime > longestTime ? currentTime : longestTime;
				}	
			}
			return longestTime;
		}
		
	}
}