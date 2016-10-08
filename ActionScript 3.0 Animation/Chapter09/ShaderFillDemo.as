package {
	import flash.display.Shader;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;
	import flash.net.URLLoader;
	import flash.net.URLLoaderDataFormat;
	import flash.net.URLRequest;

	public class ShaderFillDemo extends Sprite
	{
		
		private var loader:URLLoader;
		private var shaderURL:String = "Checkerboard.pbj";
		
		public function ShaderFillDemo()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			loader = new URLLoader();
			loader.addEventListener(Event.COMPLETE, onLoadComplete);
			loader.dataFormat = URLLoaderDataFormat.BINARY;
			loader.load(new URLRequest(shaderURL));
		}
		
		private function onLoadComplete(event:Event):void
		{
			var shader:Shader = new Shader(loader.data);
			graphics.beginShaderFill(shader);
			graphics.drawRect(0, 0, 400, 400);
			graphics.endFill();
		}
	}
}
