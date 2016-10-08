package org.as3lib.kitchensync.utils
{
	import org.as3lib.kitchensync.core.Synchronizer;
	import org.as3lib.kitchensync.core.Timestamp;
	
	/**
	 * Provides helpful utility functions for working with Timestamp objects.
	 * 
	 * @see Timestamp
	 * @see Synchronizer
	 */
	public class TimestampUtil
	{	
		/**
		 * Adds the two timestamps and returns a new one that is the sum of the two times.
		 * 
		 * @param timestampA
		 * @param timestampB
		 * @return the sum of timestampA and timestampB
		 */
		public static function add(timestampA:Timestamp, timestampB:Timestamp):Timestamp {
			return new Timestamp(timestampA.currentFrame + timestampB.currentFrame, timestampA.currentTime + timestampB.currentTime);
		}

		/**
		 * Subtracts timestampB from timestampA and returns the difference of the two times.
		 * 
		 * @param timestampA - the timstamp to subtract from.
		 * @param timestampB - the ammount of time to subtract from timestampA.
		 * @return the difference between timestampA and timestampB
		 */
		public static function subtract(timestampA:Timestamp, timestampB:Timestamp):Timestamp {
			return new Timestamp(timestampA.currentFrame - timestampB.currentFrame, timestampA.currentTime - timestampB.currentTime);
		}
		
		/**
		 * Convert frames to milliseconds based on the framerate of the Synchronizer.
		 * 
		 * @see #millisecondsToFrames()
		 * 
		 * @param frames - the number in frames to convert
		 * @return the result in milliseconds
		 */
		public static function framesToMilliseconds(frames:int):int {
			return Math.round ( frames / Synchronizer.getInstance().frameRate * 1000 );
		}
		
		/**
		 * Convert milliseconds to frames based on the framerate of the Synchronizer.
		 * 
		 * @see #framesToMilliseconds()
		 * 
		 * @param milliseconds - the number in milliseconds to convert
		 * @return the result rounded to the nearest frame
		 */
		public static function millisecondsToFrames(milliseconds:int):int {
			return Math.round ( milliseconds / 1000 * Synchronizer.getInstance().frameRate );
		}
		
		public static function getTimestampFromMilliseconds(milliseconds:int):Timestamp {
			return new Timestamp (millisecondsToFrames(milliseconds), milliseconds);
		}
		
		public static function getTimestampFromFrames(frames:int):Timestamp {
			return new Timestamp (frames, framesToMilliseconds(frames));
		}
	}
}