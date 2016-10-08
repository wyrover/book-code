package org.as3lib.kitchensync.action.tweentarget
{
	import flash.display.FrameLabel;
	import flash.display.MovieClip;
	
	// TODO add option for gotoAndPlay?
	/**
	 * A TweenTarget that controls a timeline animation in a MovieClip.
	 * 
	 * @since 1.5
	 * @author Mims Wright
	 */
	public class TimelineController implements ITweenTarget
	{
		
		/** The current frame of the movieclip. Setting it causes the MC to gotoAndStop() at that frame. */
		public function get currentValue():Number {
			if (target != null) {
				return _target.currentFrame;
			} else {
				throw new Error("Target MovieClip is not set. Cannot return the current value.");
			}
		}
		public function set currentValue(currentValue:Number):void {
			if (target != null) {
				// normalize the frame between 1 and the last frame before setting.
				var frame:int = int(currentValue);
				frame = Math.min(Math.max(frame, 1), _target.totalFrames);
				
				// goto the frame.
				target.gotoAndStop(frame);
			} else {
				throw new Error("You must set the target before changing the current value");
			}
		}
		
		/** The number of the frame to begin at */
		public function get startValue():Number { return _startValue; }
		public function set startValue(startValue:Number):void { _startValue = int(startValue); }
		protected var _startValue:int = 1;
		
		/** The number of the frame to end on */
		public function get endValue():Number { return _endValue; }
		public function set endValue(endValue:Number):void { _endValue = int(endValue); }
		protected var _endValue:int;
		
		/** The target movieclip that will be tweened. */
		public function get target():MovieClip { return _target; }
		public function set target(target:MovieClip):void { 
			_target = target; 
			// TODO make this optional.
			_target.stop();
		} 
		protected var _target:MovieClip;
		
		/**
		 * Constructor.
		 * 
		 * @param target The MovieClip object to animate.
		 * @param startFrame The frame to begin at. Can be of types String, FrameLabel or a numerical type. Defaults to 1.
		 * @param endFrame The frame to end on. Can be of types String, FrameLabel or a numerical type. Defaults to the last frame in the MovieClip.
		 */
		public function TimelineController(target:MovieClip, startFrame:Object = null, endFrame:Object = null) {
			this.target = target;
				
			if (startFrame == null) {
				// startValue will default to the first frame of the movie.
				this.startValue = 1;
			} else {
				this.startValue = evaluateFrameLabel(startFrame);
			}
			
			if (endFrame == null) {
				// endValue will default to the last frame of the movie.
				this.endValue = _target.totalFrames;
			} else {
				this.endValue = evaluateFrameLabel(endFrame);
			}
		}
		
		/**
		 * Determines the frame number based on an unknown object.
		 * 
		 * @param frameLabel An unknown object that can be of type int, uint, Number, FrameLabel, or String. 
		 * @return the frame number as an int 
		 */
		protected function evaluateFrameLabel(frameLabel:Object):int {
			if (frameLabel is FrameLabel) {
				return FrameLabel(frameLabel).frame;
			} else if (frameLabel is String) {
				return getFrameNumberFromString(String(frameLabel));
			} else if (frameLabel is Number || frameLabel is int || frameLabel is uint) {
				return int(frameLabel);
			}
			throw new ArgumentError("The 'frameLabel' parameter must be one of the following types: int, uint, Number, FrameLabel, String");
		}

		// todo Move this to a utility class.
		/**
		 * Returns a frame number based on a string label.
		 * 
		 * @param the frame label you're looking for as a string.
		 * @return int number of the frame or throws an error.
		 */
		protected function getFrameNumberFromString(matchLabel:String):int {
			var labelList:Array = _target.currentLabels;
			var l:int = labelList.length;
			var label:FrameLabel;
			for (var i:int = 0; i < l; i++) {
				label = FrameLabel(labelList[i]);
				if (label.name == matchLabel) {
					return label.frame;
				}
			}
			throw new Error("Invalid label name. The target MovieClip does not contain this label.");
		}

		public function updateTween(percentComplete:Number):Number {
			return currentValue = percentComplete * differenceInValues + _startValue;
		}
		
		/**
		 * The total amount of change between the start and end values. (used internally)
		 */
		public function get differenceInValues():Number { return _endValue - _startValue; }
		
		
		public function reset():void {
			currentValue = _startValue;
		}
		
		public function clone():ITweenTarget {
			return new TimelineController(_target, _startValue, _endValue);
		}
		
		public function toString():String {
			return "TimelineController for " + target + " from frame " + startValue + " to frame " + endValue;
		}
	}
}