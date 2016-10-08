package org.as3lib.utils
{
    import flash.utils.ByteArray;

    public class Cloner
    {
    	/**
    	 * Creates a duplicate of the source object by storing it in a ByteArray and reading it back in, 
    	 * The object will return untyped so you may need to cast it to the desired class. 
    	 * To ensure your object retains its type, you should register the class 
    	 * using flash.net.registerClassAlias() before cloning.
    	 * AMF rules apply. Check out the documentation for more info.
    	 * 
    	 * @example <listing version="3.0">
    	 * 		registerClassAlias("com.example.Example", Example);
    	 * 		var example:Example = new Example();
    	 * 		var clone:Example = Cloner.clone(example) as Example;
    	 * 		</listing>
    	 * 
    	 * @see flash.net#registerClassAlias()
    	 * @param source - the object to duplicate.
    	 * @return * - the duplicate of the source object.
    	 */
        public static function clone(source:Object):* {
            var copy:ByteArray = new ByteArray();
            copy.writeObject(source);
            copy.position = 0;
            var clonedObject:* = copy.readObject();
            return clonedObject;
        }
    }
    
}