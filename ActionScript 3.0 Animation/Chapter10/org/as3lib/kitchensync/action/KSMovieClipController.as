package org.as3lib.kitchensync.action
{
	import flash.display.MovieClip;
	
	/**
	 * Tells a target MovieClip to play a specified frame.
	 *
	 */
	 // todo test
	public class KSMovieClipController extends KSFunction
	{
		protected var _target:MovieClip;
		public function get target ():MovieClip { return _target; }
		public function set target (target:MovieClip):void { _target = target; }
		
		/**
		 * A frame identifier could be a string or a frame number to play
		 */
		protected var _frameIdentifier:*;
		public function get frameIdentifier ():* { return _frameIdentifier; }
		public function set frameIdentifier (frameIdentifier:*):void { _frameIdentifier = frameIdentifier ;}
		
		/**
		 * Constructor.
		 * 
		 * @param delay - the number of frames to delay the action
		 * @param target - the MovieClip whose frames you are going to
		 * @param frameIdentifier - a String or uint to go to
		 */
		public function KSMovieClipController(delay:*, target:MovieClip, frameIdentifier:*)
		{
			super(delay, target.gotoAndPlay, frameIdentifier);
		}
		
		override public function clone():IAction {
			var clone:KSMovieClipController = new KSMovieClipController(delay, target, frameIdentifier);
			clone._args = _args;
			clone._result = _result;
			clone.duration = _duration;
			clone.autoDelete = _autoDelete;
			clone._frameIdentifier = _frameIdentifier;
			clone._target = _target;
			return clone;
		}
	}
}