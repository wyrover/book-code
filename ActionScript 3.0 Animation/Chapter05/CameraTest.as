package {
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.media.Camera;
	import flash.media.Video;

	public class CameraTest extends Sprite
	{
		private var _cam:Camera;
		private var _vid:Video;
		
		public function CameraTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			_cam = Camera.getCamera();
			_cam.setMode(640, 480, 15);
			_vid = new Video(640, 480);
			_vid.attachCamera(_cam);
			addChild(_vid);
		}
	}
}
