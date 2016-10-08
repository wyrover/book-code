package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import org.as3lib.kitchensync.KitchenSync;
	import org.as3lib.kitchensync.action.KSParallelGroup;
	import org.as3lib.kitchensync.action.KSTween;
	import org.as3lib.kitchensync.easing.Quartic;

	[SWF(backgroundColor=0xffffff)]
	public class KitchenSyncMultiple extends Sprite
	{
		private var sprite:Sprite;
		
		public function KitchenSyncMultiple()
		{
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
			var s1:Sprite = makeSprite(100, 100, 0xff0000);
			var s2:Sprite = makeSprite(100, 200, 0x00ff00);
			var s3:Sprite = makeSprite(100, 300, 0x0000ff);
			var s4:Sprite = makeSprite(100, 400, 0xffff00);
			
			KitchenSync.initialize(this);
			var tween1:KSTween = new KSTween(s1, "x", 100, 800, 3000, 0, Quartic.easeInOut);
			var tween2:KSTween = tween1.cloneWithTarget(s2, "x");
			var tween3:KSTween = tween1.cloneWithTarget(s3, "x");
			var tween4:KSTween = tween1.cloneWithTarget(s4, "x");
			
			var pg:KSParallelGroup = new KSParallelGroup(tween1, tween2, tween3, tween4);
			pg.start();
		}
		
		private function makeSprite(xpos:Number, ypos:Number, color:uint):Sprite
		{
			var s:Sprite = new Sprite();
			s.graphics.beginFill(color);
			s.graphics.drawRect(-50, -25, 100, 50);
			s.graphics.endFill();
			s.x = xpos;
			s.y = ypos;
			addChild(s);
			return s;
		}
	}
}
