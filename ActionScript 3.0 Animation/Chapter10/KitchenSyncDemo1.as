package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import org.as3lib.kitchensync.KitchenSync;
	import org.as3lib.kitchensync.action.KSTween;
	import org.as3lib.kitchensync.easing.Quartic;

	[SWF(backgroundColor=0xffffff)]
	public class KitchenSyncDemo1 extends Sprite
	{
		private var sprite:Sprite;
		
		public function KitchenSyncDemo1()
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
			tween1.start();
			tween2.start();
		}
	}
}
