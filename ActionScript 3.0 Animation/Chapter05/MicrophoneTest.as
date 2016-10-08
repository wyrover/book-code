package {
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.media.Microphone;

	public class MicrophoneTest extends Sprite
	{
		private var _mic:Microphone;
		private var _bmpd:BitmapData;
		
		public function MicrophoneTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_bmpd = new BitmapData(400, 50, false, 0xffffff);
			addChild(new Bitmap(_bmpd));
			
			_mic = Microphone.getMicrophone();
			_mic.setLoopBack(true);
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			_bmpd.setPixel(298, 50 - _mic.activityLevel / 2, 0);
			_bmpd.scroll(-1, 0);
		}
	}
}
