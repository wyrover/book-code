package
{
	import __AS3__.vec.Vector;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	
	[SWF(backgroundColor=0xffffff)]
	public class Triangles1 extends Sprite
	{
		public function Triangles1()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			var vertices:Vector.<Number> = new Vector.<Number>();
			vertices.push(100, 100);
			vertices.push(200, 100);
			vertices.push(150, 200);
			
			graphics.lineStyle(0);
			graphics.drawTriangles(vertices);
		}
	}
}