package {
	import flash.display.Bitmap;
	import flash.display.Shader;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;

	public class ShaderBlendMode extends Sprite
	{
		[Embed (source="ChannelBlend.pbj", 
		        mimeType="application/octet-stream")]
		private var ShaderClass:Class;
		
		[Embed (source="input1.jpg")]
		private var input1:Class;
		
		[Embed (source="input2.jpg")]
		private var input2:Class;
		
		public function ShaderBlendMode()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var back:Bitmap = new input2();
			addChild(back);			
			
			var fore:Bitmap = new input1();
			addChild(fore);			
			
			var shader:Shader = new Shader(new ShaderClass());
			shader.data.amt.value = [1.0, 0.9, 0.0];
			fore.blendShader = shader;
		}
	}
}
