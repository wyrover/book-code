package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.ActivityEvent;
	import flash.media.Microphone;

	public class Clapper extends Sprite
	{
		private var _mic:Microphone;
		private var _on:Boolean = false;
		
		public function Clapper()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			update();
			
			_mic = Microphone.getMicrophone();
			_mic.setSilenceLevel(25, 500);
			_mic.setLoopBack();
			_mic.addEventListener(ActivityEvent.ACTIVITY, onActivity);
		}
		
		private function onActivity(event:ActivityEvent):void
		{
			if(event.activating)
			{
				_on = !_on;
				update();
			}
		}
		
		private function update():void
		{
			graphics.clear();
			if(_on)
			{
				graphics.beginFill(0xffffff);
			}
			else
			{
				graphics.beginFill(0);
			}
			graphics.drawRect(0, 0, stage.stageWidth, stage.stageHeight);
		}
	}
}
