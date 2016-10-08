package org.as3lib.kitchensync.core
{
	/**
	 * Timestamp encapsulates an instant as a frame int and a real time so that either may be used for 
	 * calculating synchronicity.
	 * 
	 */
	 // todo rename currentTime and currentFrame to frames and milliseconds
	public class Timestamp
	{
		private var _currentTime:int = 0;
		public function get currentTime():int { return _currentTime; }
		
		private var _currentFrame:int = 0;
		public function get currentFrame():int { return _currentFrame; }
		
		public function Timestamp(frame:int = 0, currentTime:int = 0) {
			setTime(frame, currentTime);
		}
		
		internal function setTime(frame:int = 0, currentTime:int = 0):void {
			_currentFrame = frame;
			_currentTime = currentTime;
		}
		
		public function clone():Timestamp {
			return new Timestamp(currentFrame, currentTime);
		}
		
		public function toString():String {
			return _currentTime + " msec; " + _currentFrame + " frames";
		}
	}
}