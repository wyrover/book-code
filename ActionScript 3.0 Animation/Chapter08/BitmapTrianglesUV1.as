package
{
    import __AS3__.vec.Vector;
    
    import flash.display.Bitmap;
    import flash.display.Sprite;
    import flash.display.StageAlign;
    import flash.display.StageScaleMode;
    
    [SWF(backgroundColor=0xffffff)]
    public class BitmapTrianglesUV1 extends Sprite
    {
        [Embed(source="image.jpg")]
        private var ImageClass:Class;
        
        public function BitmapTrianglesUV1()
        {
            stage.align = StageAlign.TOP_LEFT;
            stage.scaleMode = StageScaleMode.NO_SCALE;
            var vertices:Vector.<Number> = new Vector.<Number>();
            vertices.push(100, 100);
            vertices.push(200, 100);
            vertices.push(200, 200);
            vertices.push(100, 200);
            
            var uvtData:Vector.<Number> = new Vector.<Number>();
            uvtData.push(0, 0);
            uvtData.push(1, 0);
            uvtData.push(1, 1);
            uvtData.push(0, 1);
            
            var indices:Vector.<int> = new Vector.<int>();
            indices.push(0, 1, 2);
            indices.push(2, 3, 0);
            
            var bitmap:Bitmap = new ImageClass() as Bitmap;
            graphics.beginBitmapFill(bitmap.bitmapData);
            graphics.drawTriangles(vertices, indices, uvtData);
            graphics.endFill();
        }
    }
}