package {
	
	import flash.display.*;
	
	import gs.TweenLite;
	import gs.easing.Elastic;

	[SWF(backgroundColor=0xffffff)]
	public class TweenLiteDemo1 extends Sprite
	{
		private var sprite:Sprite;
		private var tween:TweenLite;
		
		public function TweenLiteDemo1()
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
			
			tween = new TweenLite(sprite, 3, {x:800, y:400, rotation:360});
			tween.ease = Elastic.easeInOut;
		}
	}
}
