package {
    import flash.display.Bitmap;
    import flash.display.BitmapData;
    import flash.display.Sprite;
    import flash.display.StageAlign;
    import flash.display.StageScaleMode;
    import flash.events.Event;
    import flash.events.MouseEvent;
    import flash.filters.BlurFilter;
    import flash.geom.Matrix;
    import flash.geom.Point;
    import flash.geom.Rectangle;
    import flash.media.Camera;
    import flash.media.Video;

    public class ColorTracking extends Sprite
    {
        private var _cam:Camera;
        private var _vid:Video;
        private var _bmpd:BitmapData;
        private var _cbRect:Sprite;
        private var _color:uint = 0xffffff;
        private var _red:Array;
        private var _green:Array;
        private var _blue:Array;
        private var _ball:Sprite;
        
        public function ColorTracking()
        {
            stage.align = StageAlign.TOP_LEFT;
            stage.scaleMode = StageScaleMode.NO_SCALE;
            
            _cam = Camera.getCamera();
            _cam.setMode(320, 240, 15);
            _vid = new Video(320, 240);
            _vid.attachCamera(_cam);
            _vid.filters = [new BlurFilter(10, 10, 1)];
            
            _bmpd = new BitmapData(320, 240, false);
            addChild(new Bitmap(_bmpd));
            
            _cbRect = new Sprite();
            addChild(_cbRect);
            
            _ball = new Sprite();
            _ball.graphics.beginFill(0x0000ff);
            _ball.graphics.drawCircle(0, 0, 40);
            _ball.graphics.endFill();
            addChild(_ball);
            
            makePaletteArrays();
            
            addEventListener(Event.ENTER_FRAME, onEnterFrame);
            stage.addEventListener(MouseEvent.CLICK, onClick);
            
        }
        
        private function makePaletteArrays():void
        {
        	_red = new Array();
        	_green = new Array();
        	_blue = new Array();
        	var levels:int = 4;
       		var div:int = 256 / levels;
        	for(var i:int = 0; i < 256; i++)
        	{
        		var value:Number = Math.floor(i / div ) * div; 
        		_red[i] = value << 16;
        		_green[i] = value << 8;
        		_blue[i] = value
        	}
        }
        
        private function onClick(event:MouseEvent):void
        {
        	_color = _bmpd.getPixel(mouseX, mouseY);
        }
        
        private function onEnterFrame(event:Event):void
        {
            _bmpd.draw(_vid, new Matrix(-1, 0, 0, 1, _bmpd.width, 0));
            _bmpd.paletteMap(_bmpd, _bmpd.rect, new Point(), _red, _green, _blue);
            
            var rect:Rectangle = _bmpd.getColorBoundsRect(0xffffff, _color, true);
            _cbRect.graphics.clear();
            _cbRect.graphics.lineStyle(1, 0xff0000);
            _cbRect.graphics.drawRect(rect.x, rect.y, rect.width, rect.height);
            
            if(!rect.isEmpty())
            {
	            var xCenter:Number = rect.x + rect.width / 2;
	            var yCenter:Number = rect.y + rect.height / 2;
	            _ball.x = xCenter / _bmpd.width * stage.stageWidth;
	            _ball.y = yCenter / _bmpd.height * stage.stageHeight; 
	        }
        }
    }
}