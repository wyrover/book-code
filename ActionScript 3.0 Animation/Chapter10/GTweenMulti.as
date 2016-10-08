package {
	import com.gskinner.motion.GTween;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	import mx.effects.easing.Quadratic;

	[SWF(backgroundColor=0xffffff)]
	public class GTweenMulti extends Sprite
	{
		
		public function GTweenMulti()
		{
			stage.scaleMode = StageScaleMode.NO_SCALE;
			stage.align = StageAlign.TOP_LEFT;
			
			var s1:Sprite = makeSprite(100, 100, 0xff0000);
			var s2:Sprite = makeSprite(100, 200, 0x00ff00);
			var s3:Sprite = makeSprite(100, 300, 0x0000ff);
			var s4:Sprite = makeSprite(100, 400, 0xffff00);
			
			var tween1:GTween = new GTween(s1, 3, {x:800}, {ease:Quadratic.easeInOut});
			var tween2:GTween = tween1.clone(s2);
			var tween3:GTween = tween1.clone(s3);
			var tween4:GTween = tween1.clone(s4);
		}
		
		private function makeSprite(xpos:Number, ypos:Number, color:uint):Sprite
		{
			var s:Sprite = new Sprite();
			s.graphics.beginFill(color);
			s.graphics.drawRect(-50, -25, 100, 50);
			s.graphics.endFill();
			s.x = xpos;
			s.y = ypos;
			addChild(s);
			return s;
		}
	}
}
