package {
	import flash.display.Shader;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.Event;

	public class ShaderFillAnim extends Sprite
	{
		[Embed (source="Checkerboard.pbj", 
		        mimeType="application/octet-stream")]
		private var ShaderClass:Class;
		
		private var shader:Shader;
		private var xAngle:Number = 0;
		private var yAngle:Number = 0;
		private var xSpeed:Number = .09;
		private var ySpeed:Number = .07;
		
		public function ShaderFillAnim()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			shader = new Shader(new ShaderClass());
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function onEnterFrame(event:Event):void
		{
			var xres:Number = Math.sin(xAngle += xSpeed) * 50 + 55;
			var yres:Number = Math.sin(yAngle += ySpeed) * 50 + 55; 
			
			shader.data.xres.value = [xres];
			shader.data.yres.value = [yres];
			graphics.clear(); 
			graphics.beginShaderFill(shader);
			graphics.drawRect(20, 20, 400, 400);
			graphics.endFill();
		}
	}
}
