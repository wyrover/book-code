package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import org.as3lib.kitchensync.KitchenSync;
	import org.as3lib.kitchensync.action.KSSequenceGroup;
	import org.as3lib.kitchensync.action.KSTween;
	import org.as3lib.kitchensync.easing.Quartic;

	[SWF(backgroundColor=0xffffff)]
	public class KitchenSyncSequence extends Sprite
	{
		private var sprite:Sprite;
		
		public function KitchenSyncSequence()
		{
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
			sprite = new Sprite();
			sprite.graphics.beginFill(0xff0000);
			sprite.graphics.drawRect(-50, -25, 100, 50);
			sprite.graphics.endFill();
			sprite.x = 100;
			sprite.y = 100;
			addChild(sprite);
			
			KitchenSync.initialize(this);
			var tween1:KSTween = new KSTween(sprite, "x", 100, 800, 3000, 0, Quartic.easeInOut);
			var tween2:KSTween = new KSTween(sprite, "y", 100, 400, 3000, 0, Quartic.easeInOut);
			var tween3:KSTween = new KSTween(sprite, "rotation", 0, 360, 3000, 0, Quartic.easeInOut);
			
			var sg:KSSequenceGroup = new KSSequenceGroup(tween1, tween2, tween3);
			sg.start();
		}
	}
}
