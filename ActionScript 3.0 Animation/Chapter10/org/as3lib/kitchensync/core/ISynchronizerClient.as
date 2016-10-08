package org.as3lib.kitchensync.core
{
	/**
	 * A consumer or the Synchronizer class. This type has the ability to be updated incrementally
	 * with time data.
	 *
	 * @author Mims Wright
	 * @since 1.6
	 */
	public interface ISynchronizerClient
	{
		/**
		 * Update will allow the synchronizer to update the action with the current time.
		 * 
		 * @param currentTimestamp The current time from the synchronizer. 
		 */
		function update(currentTimestamp:Timestamp):void;
	}
}