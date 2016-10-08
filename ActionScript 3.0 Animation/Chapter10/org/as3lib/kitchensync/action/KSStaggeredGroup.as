package org.as3lib.kitchensync.action
{
	import flash.utils.getQualifiedClassName;
	
	import org.as3lib.kitchensync.core.*;
	import org.as3lib.kitchensync.utils.*;
	
	/**
	 * A group of actions that executes in sequence seperated by a specified gap.
	 */
	public class KSStaggeredGroup extends KSParallelGroup
	{	
		/**
		 * Time to wait between the execution of each of the children.
		 * Accepts integer or parsable time string.
		 * 
		 * @see org.as3lib.kitchensync.ITimeStringParser;
		 */
		public function get stagger():int { return _stagger;}
		public function set stagger(stagger:*):void { 
			if (!isNaN(Number(stagger))) {
				_stagger = stagger; 
			} else {
				var timeString:String = stagger.toString();
				_stagger = timeStringParser.parseTimeString(timeString);
			}
			if (_stagger <= 0) {
				throw new RangeError("Stagger amount must be an integer greater than 0.");
			}
		}
		protected var _stagger:int;
		
		protected var _lastStartTime:Timestamp;
		protected var _lastStartIndex:int;
		
		
		/**
		 * Constructor.
		 * 
		 * 
		 * @params stagger - The amount of time to stagger between each action starting. 
		 *					 The first one will not stagger (but will use the delay for the Staggered object)
		 * 					 Accepts an integer or a parseable string.
		 * @params children - a list of AbstractSynchronizedActions that will be added as children of the group.
		 * 
		 * @throws TypeError - if any children are not of type AbstractSynchronizedAction.
		 */
		public function KSStaggeredGroup (stagger:*, ... children) {
			super();
			for (var i:int = 0; i < children.length; i++) {
				if (children[i] is IAction) {
					var action:IAction = IAction(children[i]);
					addAction(action); 
				} else {
					throw new TypeError ("All children must be of type IAction. Make sure you are not calling start() on the objects you've added to the group. Found " + getQualifiedClassName(children[i]) + " where IAction was expected.");
				}
			}
			this.stagger = stagger;
		}
		
		
		override public function start():IAction {
			var action:IAction = super.start();
			_lastStartIndex = -1;
			return action;
		}
		
		/**
		 * When the first update occurs, all of the child actions are started simultaniously.
		 */
		override public function update(currentTimestamp:Timestamp):void { 
			if (startTimeHasElapsed) {
				if (!_lastStartTime) {
					_runningChildren = _childActions.length;
				}
				if (!_lastStartTime || currentTimestamp.currentFrame - _lastStartTime.currentFrame > TimestampUtil.millisecondsToFrames(_stagger)) {
					_lastStartTime = currentTimestamp;
					var currentTime:int = currentTimestamp.currentFrame - TimestampUtil.millisecondsToFrames(delay) - _startTime.currentFrame;
					var currentStartIndex:int = Math.floor(currentTime / TimestampUtil.millisecondsToFrames(_stagger));
					
					// for all of the indexes since the last index.
					for (var i:int = _lastStartIndex + 1; i <= currentStartIndex; i++) {
						if (i < childActions.length) {
							var childAction:IAction = IAction(_childActions[i]);
							// add a listener to each action so that the completion of the entire group can be tracked.
							childAction.addEventListener(KitchenSyncEvent.COMPLETE, onChildFinished);
							childAction.addEventListener(KitchenSyncEvent.START, onChildStart);
							// start the child action
							childAction.start();
						}
					} 
					
					_lastStartIndex = currentStartIndex;
					
					
					// if this is the last child, unregister
					if (currentStartIndex == _childActions.length - 1) {
						unregister();
					}
				}
			}
		}
		
		override public function clone():IAction {
			var clone:KSStaggeredGroup = new KSStaggeredGroup(_stagger);
			for (var i:int = 0; i < _childActions.length; i++) {
				var action:IAction = getChildAtIndex(i).clone();
				clone.addActionAtIndex(action, i);
			}
			clone.delay = _delay;
			clone.autoDelete = _autoDelete;
			return clone;
		}
		
		override public function toString():String {
			return "Staggered Group containing " + _childActions.length + " children";
		}
	}
}