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

    public class EdgeTracking1 extends Sprite
    {
        private var _cam:Camera;
        private var _vid:Video;
        private var _bmpd:BitmapData;
        
        public function EdgeTracking1()
        {
            stage.align = StageAlign.TOP_LEFT;
            stage.scaleMode = StageScaleMode.NO_SCALE;
            
            _cam = Camera.getCamera();
            _cam.setMode(320, 240, 15);
            _vid = new Video(320, 240);
            _vid.attachCamera(_cam);
            _vid.filters = [new ConvolutionFilter(1, 3, [0, 4, -4]),
                            new BlurFilter()];
                            
            _bmpd = new BitmapData(320, 240, false);
            addChild(new Bitmap(_bmpd));
            
            addEventListener(Event.ENTER_FRAME, onEnterFrame);
        }
        
        private function onEnterFrame(event:Event):void
        {
            _bmpd.draw(_vid, new Matrix(-1, 0, 0, 1, _bmpd.width, 0));
        }
    }
}