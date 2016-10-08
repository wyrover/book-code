package {
	import com.gskinner.motion.GTween;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import mx.effects.easing.Circular;
	import mx.effects.easing.Quadratic;

	[SWF(backgroundColor=0xffffff)]
	public class GTweenSequence extends Sprite
	{
		private var sprite:Sprite;
		
		public function GTweenSequence()
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
			
			var tween1:GTween = new GTween(sprite, 3, {x:800}, {ease:Quadratic.easeInOut});
			var tween2:GTween = new GTween(sprite, 3, {y:400}, {ease:Quadratic.easeInOut});
			var tween3:GTween = new GTween(sprite, 3, {rotation:180}, {ease:Quadratic.easeInOut});
			
			tween2.pause();
			tween3.pause();
			
			tween1.nextTween = tween2;
			tween2.nextTween = tween3;
		}
	}
}
