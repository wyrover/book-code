package {
    import flash.display.Bitmap;
    import flash.display.BitmapData;
    import flash.display.Sprite;
    import flash.display.StageAlign;
    import flash.display.StageScaleMode;
    import flash.events.Event;
    import flash.filters.BlurFilter;
    import flash.filters.ConvolutionFilter;
    import flash.geom.Matrix;
    import flash.geom.Point;
    import flash.media.Camera;
    import flash.media.Video;

    public class EdgeTracking extends Sprite
    {
        private var _cam:Camera;
        private var _vid:Video;
        private var _bmpd:BitmapData;
        private var _cbRect:Sprite;
        private var _flakes:Array;
        
        public function EdgeTracking()
        {
            stage.align = StageAlign.TOP_LEFT;
            stage.scaleMode = StageScaleMode.NO_SCALE;
            
            _cam = Camera.getCamera();
            _cam.setMode(320, 240, 15);
            _vid = new Video(320, 240);
            _vid.attachCamera(_cam);
            _vid.filters = [new ConvolutionFilter(1, 3, [0, 4, -4]),
                            new BlurFilter()];
                            
            var vid2:Video = new Video(320, 240);
            vid2.attachCamera(_cam);
            vid2.scaleX = -1;
            vid2.x = 320;
            addChild(vid2);
            
            _bmpd = new BitmapData(320, 240, false);
//            addChild(new Bitmap(_bmpd));
            
            _flakes = new Array();
            
            addEventListener(Event.ENTER_FRAME, onEnterFrame);
        }
        
        private function onEnterFrame(event:Event):void
        {
            _bmpd.draw(_vid, new Matrix(-1, 0, 0, 1, _bmpd.width, 0));
            _bmpd.threshold(_bmpd, _bmpd.rect, new Point(), "<", 0x00220000, 0xff000000, 0x00ff0000, true);
            
            var snow:Snow = new Snow();
            snow.x = Math.random() * _bmpd.width;
            addChild(snow);
            _flakes.push(snow);
            
            for(var i:int = _flakes.length - 1; i >= 0; i--)
            {
            	snow = _flakes[i] as Snow;
            	if(_bmpd.getPixel(snow.x, snow.y) <= 0)
            	{
	            	snow.update();
	            	if(snow.y > _bmpd.height)
	            	{
	            		removeChild(snow);
	            		_flakes.splice(i, 1);
	            	}
	            }
            }
        }
    }
}