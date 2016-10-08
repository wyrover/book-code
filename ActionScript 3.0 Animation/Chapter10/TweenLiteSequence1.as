package {
	
	import flash.display.*;
	
	import gs.TweenLite;
	import gs.easing.Elastic;

	[SWF(backgroundColor=0xffffff)]
	public class TweenLiteSequence1 extends Sprite
	{
		private var sprite:Sprite;
		private var tween1:TweenLite;
		private var tween2:TweenLite;
		private var tween3:TweenLite;
		
		public function TweenLiteSequence1()
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
			
			tween1 = new TweenLite(sprite, 3, {x:800});
			tween2 = new TweenLite(sprite, 3, {y:400, delay:3, overwrite:0});
			tween3 = new TweenLite(sprite, 3, {rotation:360, delay:6, overwrite:0});
		}
	}
}
