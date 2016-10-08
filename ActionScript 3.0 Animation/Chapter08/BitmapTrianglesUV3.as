package
{
	import __AS3__.vec.Vector;
	
	import flash.display.Bitmap;
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.events.MouseEvent;
	
	[SWF(backgroundColor=0xffffff)]
	public class BitmapTrianglesUV3 extends Sprite
	{
		[Embed(source="image.jpg")]
		private var ImageClass:Class;
		
		private var vertices:Vector.<Number> = new Vector.<Number>();
		private var uvtData:Vector.<Number> = new Vector.<Number>();
		private var indices:Vector.<int> = new Vector.<int>();
		private var bitmap:Bitmap;
		private var res:Number = 100;
		private var cols:int = 5;
		private var rows:int = 4;
		
		
		public function BitmapTrianglesUV3()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			bitmap= new ImageClass() as Bitmap;
			makeTriangles();
			
			graphics.beginBitmapFill(bitmap.bitmapData);
			graphics.drawTriangles(vertices, indices, uvtData);
			graphics.endFill();
			
			graphics.lineStyle(0);
			graphics.drawTriangles(vertices, indices);
		}
		
		private function makeTriangles():void
		{
			for(var i:int = 0; i < rows; i++)
			{
				for(var j:int = 0; j < cols; j++)
				{
					vertices.push(j * res, i * res);
					uvtData.push(j / (cols - 1), i / (rows - 1));
					
					if(i < rows - 1 && j < cols - 1)
					{
						// first triangle
						indices.push(i * cols + j, i * cols + j + 1, (i + 1) * cols + j);
						
						// second triangle
						indices.push(i * cols + j + 1, (i + 1) * cols + j + 1, (i + 1) * cols + j);
					}
				}
			}
		}
	}
}