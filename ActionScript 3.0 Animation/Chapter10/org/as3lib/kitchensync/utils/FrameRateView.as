package org.as3lib.kitchensync.utils
{
	import flash.text.TextField;
	import flash.text.TextFieldAutoSize;
	
	import org.as3lib.kitchensync.core.ISynchronizerClient;
	import org.as3lib.kitchensync.core.KitchenSyncEvent;
	import org.as3lib.kitchensync.core.Synchronizer;
	import org.as3lib.kitchensync.core.Timestamp;


	/**
	 * A text field that displays the approximate buffered framerate of the synchronizer.
	 * Use it as you would a TextField.
	 * 
	 * @use <code>
	 * 			public var frameRateView:TextField = new SynchronizerFrameRateView();
	 * 			addChild(frameRateView);  
	 * 		</code>
	 * @see org.as3lib.kitchensync.core.Synchronizer
	 * @author Mims H. Wright
	 * @since 1.5
	 */
	public class FrameRateView extends TextField implements ISynchronizerClient
	{
		/** Number of frames between each update */
		public var updateFrequency:int = 5;
		
		private var _previousTime:int = 0;
		
		/** 
		 * The function used to format the text displayed in the textfield. 
		 * This can be replaced by the user.
		 */
		public var formattingFunction:Function = function (frameRate:int):String {
			return frameRate.toString() + " FPS";
		};
		
		/** 
		 * This array acts as a buffer to hold the past few framerates.
		 */
		private var _frameRateHistory:Array = [];
		/** The number of framerates to hold in the history */
		private var _frameRateHistoryDepth:int = 10;
		public function get actualFrameRate():Number {
			var rate:int = 0;
			for each (var history:int in _frameRateHistory) {
				rate += history;
			}
			rate /= _frameRateHistory.length;
			return Math.round(1000/rate);	
		}
		
		/** Constructor */
		public function FrameRateView()
		{
			super();
			this.autoSize = TextFieldAutoSize.LEFT;
			
			Synchronizer.getInstance().registerClient(this);
		}
		
		/**
		 * Display is updated by the synchronizer pulses.
		 */
		public function update(currentTimestamp:Timestamp):void {	
			_frameRateHistory.unshift(currentTimestamp.currentTime - _previousTime);
			if (_frameRateHistory.length > _frameRateHistoryDepth) {
				_frameRateHistory.pop();
			}
			_previousTime = currentTimestamp.currentTime;
			
			if (currentTimestamp.currentFrame %5 == 0) {
				this.text = formattingFunction(actualFrameRate);
			}
		}
	}
}