package {
	import com.gskinner.motion.GTween;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import mx.effects.easing.Circular;

	[SWF(backgroundColor=0xffffff)]
	public class GTweenDemo extends Sprite
	{
		private var sprite:Sprite;
		
		public function GTweenDemo()
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
			
			var tween:GTween = new GTween(sprite, 3, {x:800, y:400}, {ease:Circular.easeInOut});
			tween.ease = Circular.easeInOut;
		}
	}
}
