package {
	import flash.display.Shader;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.filters.ShaderFilter;
	import flash.text.TextField;
	import flash.text.TextFormat;

	[SWF(backgroundColor=0xFFFFFF)]
	public class ShaderAsFilter extends Sprite
	{
		[Embed (source="TwirlFlash.pbj", 
		        mimeType="application/octet-stream")]
		private var ShaderClass:Class;
		
		public function ShaderAsFilter()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var shader:Shader = new Shader(new ShaderClass());
			shader.data.center.value = [200, 200]; 
			shader.data.twist.value = [-1];
			shader.data.radius.value = [200];
			
			var tf:TextField = new TextField();
			tf.width = 400;
			tf.height = 400;
			tf.wordWrap = true;
			tf.multiline = true;
			tf.border = true;
			tf.defaultTextFormat = new TextFormat("Arial", 24);
			addChild(tf);
			
			for(var i:int = 0; i < 340; i++)
			{
				tf.appendText(String.fromCharCode(65 + Math.random() * 25));
			}
			tf.filters = [new ShaderFilter(shader)];
		}
	}
}
