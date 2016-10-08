package
{
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.text.TextField;
	import flash.text.TextFormat;
	
	[SWF(width=800, height=800, backgroundColor=0xffffff)]
	public class Container3D extends Sprite
	{
		private var _sprite:Sprite;
		private var _n:Number = 0;
		
		public function Container3D()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_sprite = new Sprite();
			_sprite.y = stage.stageHeight / 2;
			
			for(var i:int = 0; i < 100; i++)
			{
				var tf:TextField = new TextField();
				tf.defaultTextFormat = new TextFormat("Arial", 40);
				tf.text = String.fromCharCode(65 + Math.floor(Math.random() * 25));
				tf.selectable = false;
				tf.x = Math.random() * 300 - 150;
				tf.y = Math.random() * 300 - 150;
				tf.z = Math.random() * 1000;
				_sprite.addChild(tf);
			}
			
			addChild(_sprite);
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_sprite.x = stage.stageWidth / 2 + Math.cos(_n) * 200;
			_n += .05;
		}
	}
}