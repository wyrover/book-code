package {
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.geom.Rectangle;
	import flash.media.Microphone;

	[SWF(backgroundColor=0xffffff)]
	public class SoundFlier extends Sprite
	{
		private var _mic:Microphone;
		private var _flier:Sprite;
		private var _background:Bitmap;
		private var _vy:Number = 0;
		
		
		public function SoundFlier()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			makeBackground();
			makeFlier();
			
			_mic = Microphone.getMicrophone();
			_mic.setLoopBack(true);
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function makeBackground():void
		{
			var bmpd:BitmapData = new BitmapData(stage.stageWidth, stage.stageHeight, false);
			_background = new Bitmap(bmpd);
			addChild(_background);
		}
		
		private function makeFlier():void
		{
			_flier = new Sprite();
			_flier.graphics.lineStyle(0);
			_flier.graphics.moveTo(-10, 0);
			_flier.graphics.lineTo(-10, -8);
			_flier.graphics.lineTo(-7, -4);
			_flier.graphics.lineTo(10, 0);
			_flier.graphics.lineTo(-10, 0);
			_flier.x = 100;
			_flier.y = stage.stageHeight - 50;
			addChild(_flier);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_vy += .4;
			_vy -= _mic.activityLevel * .02;
			_flier.y += _vy;
			_vy *= .9;
			_flier.y = Math.min(_flier.y, stage.stageHeight - 20);
			_flier.y = Math.max(_flier.y, 20);
			
			var h:Number = Math.random() * 120;
			_background.bitmapData.fillRect(new Rectangle(stage.stageWidth - 20, 0, 5, h), 0xcccccc);
			
			h = Math.random() * 120;
			_background.bitmapData.fillRect(new Rectangle(stage.stageWidth - 20, stage.stageHeight - h, 5, h), 0xcccccc);
			
			_background.bitmapData.scroll(-5, 0);
		}
	}
}
