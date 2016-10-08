package {
	
	import flash.display.*;
	
	import gs.TweenGroup;
	import gs.TweenLite;

	[SWF(backgroundColor=0xffffff)]
	public class TweenGroupDemo3 extends Sprite
	{
		private var sprite:Sprite;
		private var tween1:TweenLite;
		private var tween2:TweenLite;
		private var tween3:TweenLite;
		
		public function TweenGroupDemo3()
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
			tween2 = new TweenLite(sprite, 3, {y:400, overwrite:0});
			tween3 = new TweenLite(sprite, 3, {rotation:360, overwrite:0});
			var tg:TweenGroup = new TweenGroup([tween1, tween2, tween3]);
			tg.align = TweenGroup.ALIGN_SEQUENCE;
		}
	}
}
