package {
	import flash.display.Bitmap;
	import flash.display.Shader;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.filters.ShaderFilter;

	public class ShaderFillImage extends Sprite
	{
		[Embed (source="TwirlFlash.pbj", 
		        mimeType="application/octet-stream")]
		private var ShaderClass:Class;
		
		[Embed (source="john_davey.jpg")]
		private var JohnDavey:Class;
		
		public function ShaderFillImage()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var shader:Shader = new Shader(new ShaderClass());
			shader.data.center.value = [400, 300];
			shader.data.twist.value = [-6];
			shader.data.radius.value = [300];
			
			var image:Bitmap = new JohnDavey();
			image.filters = [new ShaderFilter(shader)];
//			shader.data.src.input = image.bitmapData;
//			
//			graphics.beginShaderFill(shader);
//			graphics.drawRect(20, 20, 800, 600);
//			graphics.endFill();
		}
	}
}
