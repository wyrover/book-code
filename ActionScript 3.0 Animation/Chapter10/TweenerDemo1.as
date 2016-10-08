package {
	import caurina.transitions.Tweener;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;

	[SWF(backgroundColor=0xffffff)]
	public class TweenerDemo1 extends Sprite
	{
		private var sprite:Sprite;
		
		public function TweenerDemo1()
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
		}
	}
}
