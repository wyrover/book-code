package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import mx.effects.Tween;
	import mx.effects.easing.Quintic;

	[SWF(backgroundColor=0xffffff)]
	public class FlexTweenXY extends Sprite
	{
		private var tween:Tween;
		private var sprite:Sprite;
		
		public function FlexTweenXY()
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
			
			tween = new Tween(this, [100, 100], [800, 400], 1000);
		}
		
		public function onTweenUpdate(value:Object):void
		{
			sprite.x = value[0];
			sprite.y = value[1];
		}
		
		public function onTweenEnd(value:Object):void
		{
			sprite.x = value[0];
			sprite.y = value[1];
		}
	}
}
