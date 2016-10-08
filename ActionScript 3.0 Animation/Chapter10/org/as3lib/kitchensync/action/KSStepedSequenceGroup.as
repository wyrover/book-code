package org.as3lib.kitchensync.action
{
	import flash.utils.getQualifiedClassName;
	
	import org.as3lib.kitchensync.core.KitchenSyncEvent;
	
	/**
	 * A sequence that plays each child then pauses until it is started again. Perfect for
	 * slideshow or powerpoint-like applications.
	 * 
	 * @uthor Mims Wright
	 * @since 1.6
	 */
	public class KSStepedSequenceGroup extends KSSequenceGroup
	{
		/**
		 * Used internally to determine whether the next item should play or whether the 
		 * children should simply be unpaused when the unpause() method is called.
		 */
		protected var _waitingForInput:Boolean = false;
		
		/**
		 * Constructor. Identical to KSSequenceGroup.
		 * @param children Optional children to add upon instantiation.
		 */
		public function KSStepedSequenceGroup(...children)
		{
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
		 * Override the onChildFinished handler to pause the sequence after each child completes.
		 */
		override protected function onChildFinished(event:KitchenSyncEvent):void {
			//super.onChildFinished(event);
			_currentAction.removeEventListener(KitchenSyncEvent.COMPLETE, onChildFinished);
			_currentAction.removeEventListener(KitchenSyncEvent.START, onChildStart);
			//_currentAction = null;
			if (!checkForComplete()) {
				_waitingForInput = true;
				pause();
			} else {
				complete();
			}
		}
		
		/**
		 * Override unpause to see if the sequence was paused internally or externally.
		 */
		override public function unpause():void {
			super.unpause();
			if (_waitingForInput) {
				_waitingForInput = false;
				// Start the next action only if unpause() is called after the group was paused internally.
				startNextAction();
			}
		}
	}
}