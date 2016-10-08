package org.as3lib.kitchensync.easing
{
	 /**
	 * <p>Stepped produces values between 0.0 and 1.0 which are rounded up or down to nearby values
	 * so that they jump from one sample to the next creating a stairstep motion. It gives
	 * the impression of low-tech computer graphics.</p>
	 * <p>The easing classes provide static methods for interpolating the change between two values over time.
	 * Each class handles the interpolation, or easing, differently. Each class typically contains three methods
	 *  - <code>easeIn()</code>, <code>easeOut()</code> and <code>easeInOut()</code> - which vary the rate of change
	 *  of the values. Most of the easing functions produce values as a percentage - a number between 0.0 and 1.0</p>
	 * 
 	 * @author Mims H. Wright
	 */
	public class Stepped
	{
		/**
	    * Produces values that are cropped to a threshold based on the number of steps specified.
	    * For example, if there are 3 steps, the values will jump from 0 to 0.5 to 1.0
	    * If 10 steps are specified, the values will jump from 0 to 0.1 to 0.2, ..., to 1.0
	    * 
	    * @throws Error If steps is less than 1.
	    * 
	    * @param timeElapsed The time since the tween began in milliseconds or frames.
	    * @param duration The duration of the tween, in milliseconds or frames.
	    * @param steps The number of steps to take. Must be an integer greater than 0. 
	    *              Generally, if steps > duration, you're better off using the Linear.ease().
	    * @return percentage complete - between 0.0 and 1.0
	    */  
		public static function ease(timeElapsed:Number, duration:Number, steps:Number = 10):Number{
			if (steps < 1) { throw new Error("There must be at least 1 step."); }
			return Math.floor((timeElapsed / duration) * steps)/steps;
		}
		
		/**
		 * Creates a stepped easing function based on another easing function that uses the same method as the 
		 * ease() method. 
		 * 
		 * @example The following code will create a new Tween object which will move with Cubic motion over
		 * 			the course of 15 steps.
		 * 			<listing version="3.0">
		 * 				var steppedCubic:Tween = new Tween(foo, "bar", 0, 100, 30, Stepped.getSteppedFunction(Cubic.easeInOut, 15));
		 * 			</listing>
		 * 
		 * @param easingFunciton The original easing function to use to create ne new function.
 	     * @param steps The number of steps to take. Must be an integer greater than 0. 
	     *              Generally, if steps > duration, you're better off using the original function.
	     * @return A new easing function that adds stepping to the original easingFunction.
	     */
		public static function getSteppedFunction(easingFunction:Function, steps:int = 10):Function {
			var func:Function;
			func = function (timeElapsed:Number, duration:Number, mod1:Number = NaN, mod2:Number = NaN):Number {
				var value:Number = EasingUtil.call(easingFunction, timeElapsed, duration, mod1, mod2);
				return Math.round((value) * steps)/steps;
			}
			return func;
		}
	}
}