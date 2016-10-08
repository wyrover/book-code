package org.as3lib.kitchensync.action
{
	import flash.utils.getQualifiedClassName;
	
	import org.as3lib.kitchensync.core.KitchenSyncEvent;
	
	/**
	 * A sequence of actions that plays back in a random order.
	 * 
	 * @author Mims Wright
	 * @since 1.6
	 */
	// Todo implement.
	public class KSRandomGroup extends KSSequenceGroup
	{
		public function KSRandomGroup(... children)
		{
			for (var i:int = 0; i < children.length; i++) {
				if (children[i] is IAction) {
					var action:IAction = IAction(children[i]);
					addAction(action); 
				} else {
					throw new TypeError ("All children must be of type IAction. Make sure you are not calling start() on the objects you've added to the group. Found " + getQualifiedClassName(children[i]) + " where IAction was expected.");
				}
			}
		}
		
		override protected function startNextAction():IAction {
			_currentAction = getNextRandomAction();
			_currentAction.addEventListener(KitchenSyncEvent.COMPLETE, onChildFinished);
			_currentAction.addEventListener(KitchenSyncEvent.START, onChildStart);
			_currentAction.start();
			return _currentAction;
		}
		
		protected function getNextRandomAction():IAction {
			// TODO make random
			throw new Error("Sorry, this class isn't fully functional yet.");
			return getChildAtIndex(_currentActionIndex++);
		}
		
	}
}