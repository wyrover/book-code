package org.as3lib.kitchensync.action
{
	import flash.events.IEventDispatcher;
	
	import org.as3lib.kitchensync.core.ISynchronizerClient;
	
	/**
	 * An action is a type that represents any kind of happening that takes place at
	 * a certain time. 
	 * 
	 * @since 1.6
	 * @author Mims Wright
	 */
	public interface IAction extends ISynchronizerClient, IEventDispatcher
	{
		/**
		 * duration is the length of time that the action will run.
		 * Uses * to allow use of time string parser.
		 */
		function get duration():int;
		function set duration(duration:*):void;
		
		/**
		 * delay is the time that will pass after the start() method is called
		 * before the action begins.
		 * Uses * to allow use of time string parser.
		 */
		function get delay():int;
		function set delay(delay:*):void; 
		
		/** Should return true if the action is running (or paused). */
		function get isRunning():Boolean;
		
		/** Should return true if the action is paused. */
		function get isPaused():Boolean;
		
		/**
		 * Starts the timer for this action.
		 * 
		 * @return The action that was just started for convenience.
		 */
		function start():IAction;
		
		/**
		 * Stops the action from running and resets the timer.
		 */
		function stop():void;
		
		
		/**
		 * Causes the action to be paused. The action temporarily ignores update events from the Synchronizer 
		 * and the onUpdate() handler will not be called. When unpause() is called,
		 * the action will continue at the point where it was paused.
		 * The pause() method affects the start time even if the delay time hasn't expired yet. 
		 */
		function pause():void;
		
		/**
		 * Resumes the action at the point where it was paused.
		 */
		function unpause():void;
		
		/** Returns an exact copy of the action. */
		function clone():IAction;
		
		/**
		 * Unregisters the action and removes any refrerences to objects that it may be holding onto.
		 */
		 function kill():void;
	}
}