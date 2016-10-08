package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import mx.effects.Tween;

	[SWF(backgroundColor=0xffffff)]
	public class FlexTweenSequence extends Sprite
	{
		private var tween1:Tween;
		private var tween2:Tween;
		private var tween3:Tween;
		private var sprite:Sprite;
		
		public function FlexTweenSequence()
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
			
			tween1 = new Tween(this, 100, 800, 3000, -1, onTween1Update, onTween1End);
		}
		
		public function onTween1Update(value:Object):void
		{
			sprite.x = value as Number;
		}
		
		public function onTween1End(value:Object):void
		{
			sprite.x = value as Number;
			tween2 = new Tween(this, 100, 400, 3000, -1, onTween2Update, onTween2End);
		}
		
		public function onTween2Update(value:Object):void
		{
			sprite.y = value as Number;
		}
		
		public function onTween2End(value:Object):void
		{
			sprite.y = value as Number;
			tween3 = new Tween(this, 0, 360, 4000, -1, onTween3Update, onTween3End);
		}
		
		public function onTween3Update(value:Object):void
		{
			sprite.rotation = value as Number;
		}
		
		public function onTween3End(value:Object):void
		{
			sprite.rotation = value as Number;
		}
	}
}
