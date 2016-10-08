package org.as3lib.kitchensync
{
	import flash.display.DisplayObject;
	import flash.errors.IllegalOperationError;
	
	import org.as3lib.kitchensync.core.Synchronizer;
	

	/**
	 * Gateway to the library. Initializes the entire system.
	 * 
	 * @use KitchenSync.initialize(this, versionNumber);
	 * @since 1.2
	 * @author Mims H. Wright
	 * @see org.as3lib.kitchensync.core.Synchronizer
	 */ 
	public final class KitchenSync
	{
		/** 
		 * The current version of the library. Use this to verify that the library is the
		 * version that your software expects. 
		 */ 
		public static const VERSION:String = "1.6"
		private static var _initialized:Boolean = false;
		
		/**
		 * Initializes the timing core for KitchenSync. Must be called before using any actions.
		 * 
		 * @param frameRateSeed must be a DisplayObject that is added to the display list.
		 * @param versionCheck a string for the version you think you're using. e.g. 1.2 This is recommended
		 * 					   but not required. It will throw an error if you're using the wrong version of KS. 
		 */
		public static function initialize(frameRateSeed:DisplayObject, versionCheck:String = VERSION):void
		{	
			
			if (_initialized) {
				// todo make this error optional.
				throw new IllegalOperationError("KitchenSync has already been initialized.");
			}
			if (versionCheck != VERSION) {
				throw new Error ("Version check failed. Please update to the correct version or to continue using this version (at your own risk) put the initialize() method inside a try{} block.");
			}
			var synchronizer:Synchronizer;
			synchronizer = Synchronizer.getInstance();
			synchronizer.frameRateSeed = frameRateSeed;
			_initialized = true;
		}
		
		public function KitchenSync () {
			throw Error ("There is no need to instantiate this class. use KitchenSync.initialize() instead");
		}
	}
}

