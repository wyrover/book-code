package org.as3lib.kitchensync.utils
{
	
	/**
	 * Parses a time string into milliseconds.
	 */ 
	public class TimeStringParser_en implements ITimeStringParser
	{
		public static const MILLISECONDS_VALUE:Number = 1;
		public static const SECONDS_VALUE:Number = 1000;
		public static const MINUTES_VALUE:Number = 60000;
		public static const HOURS_VALUE:Number = 3600000;
		public static const DAYS_VALUE:Number = 86400000;
		
		protected static const NUMBER_UNIT_PAIR_SEARCH:RegExp = /(\d+(\.\d+)?)\s*[a-z]+\s*,?\s*/g;
		protected static const NUMBER_SEARCH:RegExp = /\d+(\.\d+)?/g;
		protected static const LETTER_SEARCH:RegExp = /[a-z]+/;
		
		protected static const FRAMES_SEARCH:RegExp = /([^a-z]|^)(f|fr|frames?)/;
		protected static const MILLISECONDS_SEARCH:RegExp = /([^a-z]|^)(ms|msecs?|milliseconds?)/;
		protected static const SECONDS_SEARCH:RegExp = /([^a-z]|^)(s|secs?|seconds?)/;
		protected static const MINUTES_SEARCH:RegExp = /([^a-z]|^)(m|mins?|minutes?)/;
		protected static const HOURS_SEARCH:RegExp = /(h|hrs?|hours?)/;
		protected static const DAYS_SEARCH:RegExp = /(d|days?)/;
		protected static const TIMECODE_FORMAT_SEARCH:RegExp = /(\d\d?)?(:\d\d)+(;\d\d)?/;
		protected static const TIMECODE_SEGMENT_SEARCH:RegExp = /(^\d\d?)|(:\d\d)/g;
		protected static const TIMECODE_FRAME_SEARCH:RegExp = /;(\d\d)/;
		protected static const TIMECODE_DIGIT_SEARCH:RegExp = /(\d\d?)/;
		
		
		public function TimeStringParser_en() {
			super();	
		}

		/**
		 * Parses a string into milliseconds.
		 * Times can use multiple units. Each unit should be separated by a comma or a space.
		 * Units will only be detected if they are placed after the time value. 
		 * All times will be returned in milliseconds.
		 * If no time unit is specified, the result will use null for the time unit and
		 * the synchronized action will use its default.
		 * These are all valid options:
		 * "1 hour, 2 minutes, 3 seconds, 4 milliseconds"
		 * "1h2m3s4ms"
		 * "5sec,12fr"
		 * "01:23:45;15" (1h, 23m, 45s, 15f - frames are based on stage's framerate)
		 * ":03" (3s)
		 * "300 frames"
		 * "1.25s"
		 * "5 milliseconds, 15mins, 6 hrs"
		 * "0.25 days"
		 * 
		 * @param timeString - a string representing some ammount of time.
		 * @return An int containing the time in milliseconds
		 */
		public function parseTimeString(timeString:String):int
		{
			// define an object to hold the results
			var result:Number = 0;
			
			// if the string is empty, throw an error. 
			if (!timeString) { throw new SyntaxError("The input object containes no data."); return null;}
			
			// if string contains only a number value, return it and don't specify a time unit
			if (!isNaN(Number(timeString))) {
				result = Number(timeString);
				return result;
			}
			
			// make time string not case sensitive
			timeString = timeString.toLocaleLowerCase();
			
			// Process timecode from time string if it extists.
			if (timeString.search(TIMECODE_FORMAT_SEARCH) >= 0) {
				var ms:int = 0;
				
				//trace("Converting timecode -", timeString, ".......");
				
				// Extract the times from the timecode if there are any.
				var timeMatch:Array = timeString.match(TIMECODE_SEGMENT_SEARCH);
				if (timeMatch && timeMatch.length >= 1) {
					timeMatch = timeMatch.reverse();

					// Timecodes with more than 4 segments aren't supported
					if (timeMatch.length > 4) {
						throw new SyntaxError("The timecode wasn't formatted correctly. It has too many segments.");
					}
					
					
					// check for seconds
					if (timeMatch[0]) {
						
						ms = timeMatch[0].toString().match(TIMECODE_DIGIT_SEARCH)[0] * SECONDS_VALUE;
						result += ms;
						//trace("s +" + ms);
					}
					// check for minutes
					if (timeMatch[1]) {
						ms = timeMatch[1].toString().match(TIMECODE_DIGIT_SEARCH)[0] * MINUTES_VALUE;
						result += ms;
						//trace("m +" + ms);
					}
					// check for hours
					if (timeMatch[2]) {
						ms = timeMatch[2].toString().match(TIMECODE_DIGIT_SEARCH)[0] * HOURS_VALUE;
						result += ms;
						//trace("h +" + ms);
					}
					// check for days
					if (timeMatch[3]) {
						ms = timeMatch[3].toString().match(TIMECODE_DIGIT_SEARCH)[0] * DAYS_VALUE;
						result += ms;
						//trace("d +" + ms);
					}
				}
				
				// Extract the frames from the timecode if there are any.
				var frameMatch:Array = timeString.match(TIMECODE_FRAME_SEARCH);
				if (frameMatch && frameMatch.length >= 1) {
					ms = TimestampUtil.framesToMilliseconds(frameMatch[0].toString().match(TIMECODE_DIGIT_SEARCH)[0]);
					result += ms;
					//trace ("f +" + ms);
				}
				
				//trace ("  = " + result + "ms");
				return result;
			}
			
			// separate by number / unit pairs separated by spaces or commas.
			var unitValuePairs:Array = timeString.match(NUMBER_UNIT_PAIR_SEARCH);
			// if there are no pairs, the data is malformed.
			if (unitValuePairs.length < 1) {
				throw new SyntaxError("The input object contains malformed data: " + timeString);
				return null;
			}
			
			// for each unit value pair...
			for each (var pair:String in unitValuePairs) {
				
				// separate the number from the text unit identifier
				var time:Number = Number(pair.match(NUMBER_SEARCH)[0]);
				var timeUnit:String = pair.match(LETTER_SEARCH)[0];
				
				//based on the time unit, convert the time value to milliseconds
				if (timeUnit.search(FRAMES_SEARCH) >= 0) {
					// Convert frames to milliseconds
					time = TimestampUtil.framesToMilliseconds(time);
				} else {
					if (timeUnit.search(MILLISECONDS_SEARCH) >= 0) {
						time *= MILLISECONDS_VALUE;
					} else if (timeUnit.search(SECONDS_SEARCH) >= 0) {
						time *= SECONDS_VALUE;
					} else if (timeUnit.search(MINUTES_SEARCH) >= 0) {
						time *= MINUTES_VALUE;
					} else if (timeUnit.search(HOURS_SEARCH) >= 0) {
						time *= HOURS_VALUE;
					} else if (timeUnit.search(DAYS_SEARCH) >= 0) {
						time *= DAYS_VALUE;
					} else {
						// this is probably a different type of unit.
						throw new SyntaxError("The input object contains malformed data.");
						continue;
					}
				}
				time = Math.round(time);
				result += time;
			} 
			// return the result as an integer
			return int(result);
		}
	}
}