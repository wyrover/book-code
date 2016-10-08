package {
	import fl.transitions.Tween;
	import fl.transitions.TweenEvent;
	import fl.transitions.easing.Regular;
	import fl.transitions.easing.Strong;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;

	[SWF(backgroundColor=0xffffff)]
	public class FlashTweenSequence extends Sprite
	{
		private var tween1:Tween;
		private var tween2:Tween;
		private var tween3:Tween;
		private var sprite:Sprite;
		
		public function FlashTweenSequence()
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
			
			tween1 = new Tween(sprite, "x", Regular.easeInOut, 100, 800, 3, true);
			tween1.addEventListener(TweenEvent.MOTION_FINISH, onTween1Finish);
		}
		
		private function onTween1Finish(event:TweenEvent):void
		{
			tween2 = new Tween(sprite, "y", Regular.easeInOut, 100, 400, 3, true);
			tween2.addEventListener(TweenEvent.MOTION_FINISH, onTween2Finish);
		}
		
		private function onTween2Finish(event:TweenEvent):void
		{
			tween3 = new Tween(sprite, "rotation", Strong.easeInOut, 0, 360, 4, true);
		}
	}
}
