package org.as3lib.kitchensync.action
{
	import flash.utils.getQualifiedClassName;
	
	import org.as3lib.kitchensync.core.*;
	
	/**
	 * A group of actions that execute one at a time in the order that they were added to the group.
	 * 
	 * */
	public class KSSequenceGroup extends AbstractActionGroup
	{
		protected const NO_CURRENT_ACTION_INDEX:int = -1;
		
		protected var _currentAction:IAction = null;
		protected var _currentActionIndex:int = NO_CURRENT_ACTION_INDEX;
		
		public function get childrenAreRunning():Boolean { return _currentActionIndex != NO_CURRENT_ACTION_INDEX; }
		public function get currentAction():IAction { return _currentAction; }
		
		/**
		 * Constructor.
		 * 
		 * @throws TypeError - if any children are not of type AbstractSynchronizedAction.
		 * 
		 * @params children - a list of AbstractSynchronizedActions that will be added as children of the group.
		 */
		public function KSSequenceGroup (... children) {
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
		
		/**
		 * Listens for updates to synchronize the start time of the sequence.
		 * The first action in the sequence is called by using the startNextAction() method.
		 * After the Sequence starts running, it no longer needs to listen to updates so it unregisters.
		 */
		override public function update(currentTimestamp:Timestamp):void {
			if (startTimeHasElapsed && !childrenAreRunning) {
				startNextAction();
				// Sequence no longer needs to listen for events from Synchronizer
				// since it now receives all cues from its children.
				unregister();
			}
		}
		
		/**
		 * Starts playing the next action in the sequence. Listens for the COMPLETE event for each child
		 * and runs onChildFinished() when each child completes. The action that is currently playing
		 * will be stored in _currentAction which is publicly accessible. 
		 * 
		 * @return The currently playing action.
		 */
		protected function startNextAction():IAction {
			_currentActionIndex++;
			_currentAction = getChildAtIndex(_currentActionIndex);
			_currentAction.addEventListener(KitchenSyncEvent.COMPLETE, onChildFinished);
			_currentAction.addEventListener(KitchenSyncEvent.START, onChildStart);
			_currentAction.start();
			return _currentAction;
		}
		
		/**
		 * Called when child actions are completed. After each is finished, checks to see if the sequence is 
		 * complete. If not, it starts the next child.
		 * Also remove reference to child action so it can be garbage collected.
		 * 
		 * @param event - The SynchronizerEvent.COMPLETE from the _currentAction
		 */
		 // todo - Add a reference to the completed child to the CHILD_COMPLETE event.
		override protected function onChildFinished (event:KitchenSyncEvent):void {
			super.onChildFinished(event);
			_currentAction.removeEventListener(KitchenSyncEvent.COMPLETE, onChildFinished);
			_currentAction.removeEventListener(KitchenSyncEvent.START, onChildStart);
			//_currentAction = null;
			if (!checkForComplete()) {
				startNextAction();
			} else {
				complete();
			}
		}
		
		/**
		 * Checks to see if all of the children have completed. If so, calls the complete method.
		 * 
		 * @return true if complete, otherwise false.
		 */
		protected function checkForComplete():Boolean{
			if (_childActions && (_childActions.length > 0 ) && (_currentActionIndex >= _childActions.length-1)) { 
				return true;
			}
			return false;
		}
		
		/**
		 * override to reset the _currentActionIndex.
		 */
		override protected function complete():void {
			_currentActionIndex = NO_CURRENT_ACTION_INDEX;
			super.complete();
		}
		
		/**
		 * Override start to automatically quit if there are no children.
		 */
		override public function start():IAction {
			super.start();
			if (childActions && childActions.length < 1) { complete(); }
			return this;
		}
		
		override public function clone():IAction {
			var clone:KSSequenceGroup = new KSSequenceGroup();
			for (var i:int = 0; i < _childActions.length; i++) {
				var action:IAction = getChildAtIndex(i).clone();
				clone.addActionAtIndex(action, i);
			}
			clone.delay = _delay;
			clone.autoDelete = _autoDelete;
			return clone;
		}
		
		override public function toString():String {
			var string:String = "KSSequenceGroup[";
			for each (var action:IAction in _childActions) {
				string += Object(action).toString() + ", ";
			}  
			string += "]";
			return  string;
		}
	}
}