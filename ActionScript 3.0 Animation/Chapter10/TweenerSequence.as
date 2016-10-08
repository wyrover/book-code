package {
	import caurina.transitions.Tweener;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;

	[SWF(backgroundColor=0xffffff)]
	public class TweenerSequence extends Sprite
	{
		private var sprite:Sprite;
		
		public function TweenerSequence()
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
			
			Tweener.addTween(sprite, {x:800, time:3, transition:"easeInOutCubic", onComplete:tween1End});
		}
		
		private function tween1End():void
		{
			Tweener.addTween(sprite, {y:400, time:3, transition:"easeInOutCubic", onComplete:tween2End});
		}
		
		private function tween2End():void
		{
			Tweener.addTween(sprite, {rotation:360, time:4, transition:"easeInOutCubic"});
		}
	}
}
