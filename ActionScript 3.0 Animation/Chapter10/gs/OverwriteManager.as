/*
VERSION: 0.99
DATE: 10/10/2008
ACTIONSCRIPT VERSION: 3.0 (AS2 version is available)
UPDATES & DOCUMENTATION AT: http://blog.greensock.com/overwritemanager/
DESCRIPTION:
	OverwriteManager is included in all TweenLite/TweenFilterLite/TweenMax downloads and allows you to control how tweens with 
	overlapping properties are handled. Without OverwriteManager, tweens of the same object are always completely overwritten 
	unless you set overwrite:0 (previously overwrite:false which still works by the way).
		
		TweenLite.to(mc, 1, {x:100, y:200});
		TweenLite.to(mc, 1, {alpha:0.5 delay:2}); //Without OverwriteManager, this tween immediately overwrites the previous one
		
	So even though there are no overlapping properties in the previous example, the 2nd tween would overwrite the first. 
	The primary reason for this has to do with speed and file size. But if you're willing to sacrifice	a little speed and file size, 
	OverwriteManager can work with the tweening classes to automatically sense when there are overlapping properties and then only 
	overwrite the individual properties in the other tween(s). Don't worry, you'd probably never notice even a slight speed decrease 
	unless hundreds of tweens are beginning simultaneously with overlapping properties.
		
	I kept OverwriteManager as a separate, optional class primarily because of file size concerns. I know, you're probably thinking 
	"what the heck? It's like 1Kb! What's the big deal?". Well, there are thousands of developers using TweenLite because of its
	extremely small footprint and blistering speed. Even 1Kb would represent a 33% increase in file size, and some developers have no 
	use for the capabilities of this class. 
	
	So OverwriteManager is an optional enhancement to TweenLite and TweenFilterLite, but it is automatically included with TweenMax
	without any additional steps required on your part. That also means that if you use TweenMax anywhere in your project, 
	OverwriteManager will automatically get initted and will therefore affect TweenLite and TweenFilterLite, making their
	default mode "AUTO" instead of "ALL". 
	

USAGE:
	OverwriteManager has three modes: NONE, ALL, and AUTO. By default, it uses AUTO. Here's what they do:
		
		- NONE (0): No tweens are overwritten. This is the fastest mode, but you need to be careful not to create any tweens with
					overlapping properties, otherwise they'll conflict with each other.
						
		- ALL (1): Similar to the default behavior of TweenLite/TweenFilterLite/TweenMax where all tweens of the same object are completely
				   overwritten immediately when the tween is created. 
						
						TweenLite.to(mc, 1, {x:100, y:200});
						TweenLite.to(mc, 1, {x:300, delay:2}); //immediately overwrites the previous tween
							
		- AUTO (2): Searches for and overwrites only individual overlapping properties in tweens that are running at the time the tween begins. 
						
						TweenLite.to(mc, 1, {x:100, y:200});
						TweenLite.to(mc, 1, {x:300}); //only overwrites the "x" property in the previous tween
							
		- CONCURRENT (3): Overwrites all tweens of the same object that are active at the time the tween begins.
						
						TweenLite.to(mc, 1, {x:100, y:200});
						TweenLite.to(mc, 1, {x:300, delay:2}); //does NOT overwrite the previous tween because the first tween will have finished by the time this one begins.
		
		
	To add OverwriteManager's capabilities to TweenLite and TweenFilterLite, you must init() the class once (typically on the first frame of your file) like so:
			
		OverwriteManager.init();
		
	You do NOT need to add this line if you're using TweenMax because it automatically does it internally.
	

EXAMPLES: 

	To start OverwriteManager in AUTO mode (the default) and then do a simple TweenLite tween, simply do:
		
		import gs.*;
		
		OverwriteManager.init();
		TweenLite.to(mc, 2, {x:"300"});
		
	You can also define overwrite behavior in individual tweens, like so:
	
		import gs.*;
		
		OverwriteManager.init();
		TweenLite.to(mc, 2, {x:"300", y:"100"});
		TweenLite.to(mc, 1, {alpha:0.5, overwrite:1}); //or simply the constant OverwriteManager.ALL
		TweenLite.to(mc, 3, {x:200, rotation:30, overwrite:2}); //or simply the constant OverwriteManager.AUTO
		
	But normally, you'll just control the overwriting directly through the OverwriteManager with its mode property, like this:
		
		import gs.*;
		
		OverwriteManager.init(OverwriteManager.ALL);
		
		//-OR-//
		
		OverwriteManager.init();
		OverwriteManager.mode = OverwriteManager.ALL;
		
	The mode can be changed anytime.
		
	

NOTES:
	- This class adds about 1Kb to your SWF.

CODED BY: Jack Doyle, jack@greensock.com
Copyright 2008, GreenSock (This work is subject to the terms in http://www.greensock.com/terms_of_use.html.)
*/

package gs {
	import flash.errors.*;
	import flash.utils.*;
	
	public class OverwriteManager {
		public static const version:Number = 0.99;
		public static const NONE:int = 0;
		public static const ALL:int = 1;
		public static const AUTO:int = 2;
		public static const CONCURRENT:int = 3;
		public static var mode:int;
		public static var enabled:Boolean;
		
		public static function init($mode:int=2):int {
			if (TweenLite.version < 9.12) {
				trace("TweenLite warning: Your TweenLite class needs to be updated to work with OverwriteManager (or you may need to clear your ASO files). Please download and install the latest version from http://www.tweenlite.com.");
			}
			TweenLite.overwriteManager = OverwriteManager;
			mode = $mode;
			enabled = true;
			return mode;
		}
		
		public static function manageOverwrites($tween:TweenLite, $targetTweens:Array):void {
			var vars:Object = $tween.vars;
			var m:int = (vars.overwrite == undefined) ? mode : int(vars.overwrite);
			if (m < 2 || $targetTweens == null) {
				return;
			}
			
			var startTime:Number = $tween.startTime, a:Array = [], i:int, tween:TweenLite;
			for (i = $targetTweens.length - 1; i > -1; i--) {
				tween = $targetTweens[i];
				if (tween != $tween && tween.startTime <= startTime && tween.startTime + (tween.duration * 1000 / tween.combinedTimeScale) > startTime) {
					a[a.length] = tween;
				}
			}
			if (a.length == 0) {
				return;
			}
			
			if (m == AUTO) {
				if (vars.isTV == true) {
					vars = vars.exposedProps; //Enables use of the TweenLiteVars, TweenFilterLiteVars, and TweenMaxVars utility classes.
				}
				var v:Object = {}, p:String;
				for (p in vars) {
					if (p == "ease" || p == "delay" || p == "overwrite" || p == "onComplete" || p == "onCompleteParams" || p == "runBackwards" || p == "persist" || p == "onUpdate" || p == "onUpdateParams" || p == "timeScale" || p == "onStart" || p == "onStartParams" || p == "renderOnStart" || p == "proxiedEase" || p == "easeParams" || p == "onCompleteAll" || p == "onCompleteAllParams" || p == "yoyo" || p == "loop" || p == "onCompleteListener" || p == "onStartListener" || p == "onUpdateListener") {
						//ignore
					} else {
						v[p] = 1;
					}
				}
				
				for (i = a.length - 1; i > -1; i--) {
					a[i].killVars(v);
				}
			} else {
				for (i = a.length - 1; i > -1; i--) {
					a[i].enabled = false; //flags for garbage collection
				}
			}
		}
		
		public static function killVars($killVars:Object, $vars:Object, $tweens:Array, $subTweens:Array, $filters:Array):void {
			var i:int;
			for (i = $subTweens.length - 1; i > -1; i--) {
				if ($killVars[$subTweens[i].name] != undefined) {
					$subTweens.splice(i, 1);
				}
			}
			for (i = $tweens.length - 1; i > -1; i--) {
				if ($killVars[$tweens[i][4]] != undefined) {
					$tweens.splice(i, 1);
				}
			}
			for (i = $filters.length - 1; i > -1; i--) {
				if ($killVars[$filters[i].name] != undefined) {
					$filters.splice(i, 1);
				}
			}
			for (var p:String in $killVars) {
				delete $vars[p];
			}
		}

	}
}