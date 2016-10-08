package org.as3lib.kitchensync.action
{
	import org.as3lib.kitchensync.KitchenSyncDefaults;
	import org.as3lib.kitchensync.action.tweentarget.*;
	
	public class KitchenSyncObjectParser implements ITweenObjectParser {
		
		public static const TARGET:String 			= "target";
		public static const TARGETS:String 			= "targets";
		public static const DURATION:String			= "duration";
		public static const DELAY:String 			= "delay";
		public static const EASING_FUNCTION:String	= "easingFunction";
		public static const EASING_MOD1:String		= "easingMod1";
		public static const EASING_MOD2:String		= "easingMod2";
		public static const PROPERTIES:String		= "properties";
		public static const SYNC:String				= "sync";
		public static const SNAP_TO_WHOLE:String	= "snapToWholeNumbers"; 
		public static const SNAP_TO_VALUE:String	= "snapToValueOnComplete";
		
		private static const PROP_DELIMITER:RegExp = /\s*,\s*/;
		private static const PROP_VALUE_DELIMITER:RegExp = /\s*(=|:)\s*/;
		private static const VALUE_RANGE_DELIMITER:RegExp = /\s*[~]\s*/;
		private static const START_VALUE_MARKER:RegExp = /(_start|Start|_from|From)/;
		private static const END_VALUE_MARKER:RegExp = /(_end|End|_to|To)/;
		
		public function get keywords():Array
		{
			return _keywords;
		}
		
		protected var _keywords:Array = [
			TARGET, TARGETS, PROPERTIES,
			DURATION, DELAY,
			EASING_FUNCTION, EASING_MOD1, EASING_MOD2,
			SYNC, SNAP_TO_VALUE, SNAP_TO_WHOLE
		];
		
		public function parseObject(parameters:Object):ITween {
			// set up initial tween.
			var tween:KSTween = new KSTween();
			
			tween.duration = parseDuration(parameters);
			tween.delay = parseDelay(parameters);
			tween.easingFunction = parseEasingFunction(parameters);
			tween.sync = parseSync(parameters);
			
			
			var mod1:* = parseMod1(parameters);
			var mod2:* = parseMod2(parameters);

			if (mod1) { tween.easingMod1 = mod1; }
			if (mod2) { tween.easingMod2 = mod2; }
			
			var targets:Array = parseTargets(parameters);
			//trace("targets:",targets);
			var properties:Array = [];
			var explicitProperties:Array = parseProperties(parameters);
			var stringProperties:Array = parsePropertiesString(parameters);
			properties = properties.concat(explicitProperties, stringProperties);
			trace("properties (" + properties.length + "):",properties);
			
			var i:int, j:int;
			var target:Object;
			var propertyData:PropertyData;
			var tweenTarget:ITweenTarget;
			
			for (i = 0; i < targets.length; i ++) {
				target = targets[i];
				for (j = 0; j < properties.length; j++) {
					propertyData = properties[j] as PropertyData;
					if (propertyData != null) {
						tweenTarget = new TargetProperty(
										target, 
										propertyData.propertyName,
										propertyData.startValue,
										propertyData.endValue
									  );
														
						tween.addTweenTarget(tweenTarget);
					}
				}
			} 
			
			return tween;
		}
		
		private function parseTargets(parameters:Object):Array {
			// extract raw data.
			var targets:Array, target:Object;
			
			targets = getFirstDefinedValue(parameters, "targets") as Array;
			// determine which value to use.
			if (targets != null && targets.length > 0) {
				return targets;
			} 
			
			target = getFirstDefinedValue(parameters, "target") as Object;
			if (target != null) {
				return [target];
			}
			throw new SyntaxError("At least one target must be defined");
		}
		
		private function parseSync(parameters:Object):Boolean {
			var sync:Object = getFirstDefinedValue(parameters, "sync") as Boolean;
			if (sync != null) {
				return sync as Boolean;
			}
			return KitchenSyncDefaults.sync;
		}
		
		private function parseEasingFunction(parameters:Object):Function {
			var easingFunction:Function = getFirstDefinedValue(parameters, "easingFunction", "easing", "ease") as Function;
			if (easingFunction != null) {
				return easingFunction;
			}
			return null;
		}
		
		private function parseDuration(parameters:Object):* {
			var duration:* = getFirstDefinedValue(parameters, "duration");
			if (isNaN(duration)) {
				if (duration == null) {
					// default to 1 second
					return 1000;
				}
			} 
			return duration;
		}

		private function parseDelay(parameters:Object):* {
			var delay:* = getFirstDefinedValue(parameters, "delay", "offset");
			return delay;
		}
		
		
		private function parseProperties(paramerters:Object):Array {
			var resultsArray:Array = new Array();
			var property:String;
			var startValue:Number;
			var endValue:Number;
			
			for (var key:String in paramerters) {
				if (keywords.indexOf(key) < 0) {
					var data:PropertyData = new PropertyData();
					// check for ~ notation 
					var string:String = paramerters[key].toString();				
					if (string.search("~") >= 0) {
						var values:Array = string.split("~");
						data.propertyName = key;
						data.startValue = values[0];
						data.endValue = values[1];
					} else {
						// check for _start / _end notation.
						var startStringIndex:int = key.search(START_VALUE_MARKER);
						var endStringIndex:int = key.search(END_VALUE_MARKER);
						if (startStringIndex >= 0) {
							data.propertyName = key.slice(0, startStringIndex);
							data.startValue = paramerters[key] as Number; 
							data.endValue = getFirstDefinedValue(paramerters, 	data.propertyName + "_end", 
																				data.propertyName + "End",
																				data.propertyName + "_to",
																				data.propertyName + "To") as Number;
							if (isNaN(data.endValue)) {
								data.endValue = KSTween.VALUE_AT_START_OF_TWEEN;
							}
							
							resultsArray.push(data);
							continue;
						} else {
							// ignore it.
							continue;
						}
					} 
					// check results for keywords.
					switch (data.propertyName) {
						case "scale":
							resultsArray.push(new PropertyData("scaleX", data.startValue, data.endValue));
							resultsArray.push(new PropertyData("scaleY", data.startValue, data.endValue));
						break;
						
						default:
							// push results onto array to later be converted into tween targets.
							resultsArray.push(data);
					}
				}
			}
			return resultsArray;
		}
		
		private function parsePropertiesString(parameters:Object):Array {
			var property:PropertyData;
			var resultsArray:Array = [];
			
			var allPropsString:String = getFirstDefinedValue(parameters, "properties", "property") as String;
			if (allPropsString == null) { return null; }
			
			var properties:Array = allPropsString.split(PROP_DELIMITER);
			var singleProperty:String;
			var values:String;
			/* 
			for (var key:String in parameters) {
				if (keywords.indexOf(key) < 0) {
					// if key isn't a keyword
					var startStringIndex:int = key.search(START_VALUE_MARKER);
					if (startStringIndex >= 0) {
						var propertyName:String = key.slice(0, startStringIndex);
						var startValue:Number = parameters[key] as Number; 
						var endValue:Number = getFirstDefinedValue(parameters, propertyName + "_end", propertyName + "End") as Number;
						resultsArray.push(new PropertyData(propertyName, startValue, endValue));
					} else {
						var propertyName:String = key;
						var values:String = parameters[key].toString();
						var startValue:Number = values.split(VALUE_RANGE_DELIMITER)[0];
						var endValue:Number = values.split(VALUE_RANGE_DELIMITER)[1];
					}
				}
			} */
			
			for each (singleProperty in properties) {
				property = new PropertyData();
				property.propertyName = singleProperty.split(PROP_VALUE_DELIMITER)[0];
				values = singleProperty.split(PROP_VALUE_DELIMITER)[2]; 
				if (values.search(VALUE_RANGE_DELIMITER) < 0) {
					property.startValue = KSTween.VALUE_AT_START_OF_TWEEN;
					property.endValue = Number(values);
				} else {
					property.startValue = Number(values.split(VALUE_RANGE_DELIMITER)[0]);
					property.endValue = Number(values.split(VALUE_RANGE_DELIMITER)[1]);
				}
				
				switch (property.propertyName) {
					case "scale":
						resultsArray.push(new PropertyData("scaleX", property.startValue, property.endValue));
						resultsArray.push(new PropertyData("scaleY", property.startValue, property.endValue));
					break;
					
					default:
						resultsArray.push(property);
				}
			}
			return resultsArray;
		}
		
		private static function parseMod1(parameters:Object):Number {
			var mod1:Number = getFirstDefinedValue(parameters, "easingMod1", "mod1") as Number;
			 return mod1;
		}
		private static function parseMod2(parameters:Object):Number {
			var mod2:Number = getFirstDefinedValue(parameters, "easingMod2", "mod2") as Number;
			 return mod2;
		}
		
		/** 
		 * Returns the first defined variable within an object.
		 * Used when you need to access a value that may have multiple aliases.
		 * The values will be checked in order that they are entered.
		 * 
		 * @param object The object to check for values.
		 * @param keys The remaining parameters will be strings of property names to check for valid values.
		 * @return the value of the first defined property or null if all were undefined.
		 */
		private static function getFirstDefinedValue(object:Object, ... keys):* {
			var key:String, value:*, i:int;
			for (i = 0; i < keys.length; i++) {
				key = keys[i];
				value = object[key] as Object;
				if (value != null && value != undefined)  {
					return value;
				}
			}
			return null;
		}
	}
}
	
internal class PropertyData {
	public var propertyName:String;
	public var startValue:Number;
	public var endValue:Number;
	
	public function PropertyData(propertyName:String = "", startValue:Number = NaN, endValue:Number = NaN) {
		this.propertyName = propertyName;
		this.startValue = startValue;
		this.endValue = endValue;
	}
	
	public function toString():String { return propertyName + ":" + startValue + "~" + endValue }
}