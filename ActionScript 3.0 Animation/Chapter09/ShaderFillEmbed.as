package {
	import flash.display.Shader;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.geom.Matrix;

	public class ShaderFillEmbed extends Sprite
	{
		[Embed (source="Checkerboard.pbj", 
		        mimeType="application/octet-stream")]
		private var ShaderClass:Class;
		
		public function ShaderFillEmbed()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var shader:Shader = new Shader(new ShaderClass());
		
			var angle:Number = Math.PI / 4;
			var cos:Number = Math.cos(angle);
			var sin:Number = Math.sin(angle);
			graphics.beginShaderFill(shader, new Matrix(cos, sin, -sin, cos));
			graphics.drawRect(20, 20, 400, 400);
			graphics.endFill();
		}
	}
}
