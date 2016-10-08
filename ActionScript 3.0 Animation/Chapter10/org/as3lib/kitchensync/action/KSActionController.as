package org.as3lib.kitchensync.action
{
	/**
	 * Executes a function within another AbstractSynchronizedAction when executed. This allows you to easily
	 * control other actions within a sequence. The accepted commands can be found in ActionControllerCommands.
	 * 
	 * @see ActionControllerCommands
	 */
	public class KSActionController extends KSFunction
	{
		/**
		 * The action that the ActionController will send commands to.
		 */
		protected var _target:IAction;
		public function set target (target:IAction):void { _target = target; }
		public function get target ():IAction { return _target; }
		
		/**
		 * Constructor.
		 * 
		 * @param target - the AbstractSynchronizedAction that will receive the commands from the controller.
		 * @param command - the function that the SynchronizedAction will perform when the ActionController executes.
		 * @param delay - the number of frames to delay the action.
		 */ 
		public function KSActionController (target:IAction, command:ActionControllerCommand = null, delay:* = 0) {
			super(delay, null);
			if (target) {
				_target = target;
			} else {
				throw new ArgumentError ("target AbstractSynchronizedAction must not be null.");
			}
			if (command == null) {  command = ActionControllerCommand.DEFAULT; }
			switch (command) {
				case ActionControllerCommand.START:
					_func = function ():void { _target.start(); };
				break;
				case ActionControllerCommand.PAUSE:
					_func = function ():void { _target.pause(); };
				break;
				case ActionControllerCommand.UNPAUSE:
					_func = function ():void { _target.unpause(); };
				break;
				case ActionControllerCommand.STOP:
					_func = function ():void { _target.stop(); };
				break;
				case ActionControllerCommand.KILL:
					_func = function ():void { _target.kill(); this.kill(); };
				break;
				case ActionControllerCommand.RESET:
					// RESET only appllies to Tweens so use stop() if the object isn't a Tween.
					_func = function ():void { 
						if (_target is KSTween) { 
							KSTween(_target).reset();
						} else { 
							_target.stop(); 
						} 
					};
				break;
				
				default:
					throw new ArgumentError ("Please provide an appropriate value for the command argument. Use one of the enumeration values from ActionControllerCommands.");
					kill();
				break;
			}
		}
		
		override public function kill():void {
			super.kill();
			_target = null;
		}
	}
}