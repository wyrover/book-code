package org.as3lib.kitchensync.easing
{
	/**
	 * <p>Random produces random values between 0.0 and 1.0. Since there is no accelleration, there is only one
	 * ease() function.</p>
	 * <p>The easing classes provide static methods for interpolating the change between two values over time.
	 * Each class handles the interpolation, or easing, differently. Each class typically contains three methods
	 *  - <code>easeIn()</code>, <code>easeOut()</code> and <code>easeInOut()</code> - which vary the rate of change
	 *  of the values. Most of the easing functions produce values as a percentage - a number between 0.0 and 1.0</p>
	 * 
 	 * @author Mims H. Wright
	 */
	public class Random
	{
		/**
		 * If set to true, random will snap to 1.0 if duration has elapsed.
		 */
		public static var snapping:Boolean = true;
		
		/**
	    * Produces a random value between 0.0 and 1.0. If snapping is set to true, 
	    * always returns 1.0 when duration has elapsed.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param minimum The lowest value to generate
	    * @param maximum The highest value to generate
	    * @return percentage complete - between 0.0 and 1.0
	    */  
		public static function ease(timeElapsed:Number, duration:Number, minimum:Number = 0.0, maximum:Number = 1.0):Number{
			if (timeElapsed >= duration && snapping == true) {
				return 1;
			}
			return Math.random() * (maximum + minimum) - minimum;
		}
	}
}