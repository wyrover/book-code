package {
	import caurina.transitions.Tweener;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;

	[SWF(backgroundColor=0xffffff)]
	public class TweenerDelay extends Sprite
	{
		private var sprite:Sprite;
		
		public function TweenerDelay()
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
			
			Tweener.addTween(sprite, {x:800, time:3, transition:"easeInOutCubic"});
			Tweener.addTween(sprite, {y:400, time:3, transition:"easeInOutCubic", delay:3});
			Tweener.addTween(sprite, {rotation:360, time:4, transition:"easeInOutCubic", delay:6});
		}
	}
}
