package
{
	import com.friendsofed.isometric.IsoUtils;
	import com.friendsofed.isometric.Point3D;
	
	import flash.display.Sprite;
	import flash.display.StageAlign;
	import flash.display.StageScaleMode;
	import flash.geom.Point;

	public class IsoTransformTest extends Sprite
	{
		public function IsoTransformTest()
		{
			stage.align = StageAlign.TOP_LEFT;
			stage.scaleMode = StageScaleMode.NO_SCALE;
			
			var p0:Point3D = new Point3D(0, 0, 0);
			var p1:Point3D = new Point3D(100, 0, 0);
			var p2:Point3D = new Point3D(100, 0, 100);
			var p3:Point3D = new Point3D(0, 0, 100);
			
			var sp0:Point = IsoUtils.isoToScreen(p0);
			var sp1:Point = IsoUtils.isoToScreen(p1);
			var sp2:Point = IsoUtils.isoToScreen(p2);
			var sp3:Point = IsoUtils.isoToScreen(p3);
			
			var tile:Sprite = new Sprite();
			tile.x = 200;
			tile.y = 200;
			addChild(tile);
			
			tile.graphics.lineStyle(0);
			tile.graphics.moveTo(sp0.x, sp0.y);
			tile.graphics.lineTo(sp1.x, sp1.y);
			tile.graphics.lineTo(sp2.x, sp2.y);
			tile.graphics.lineTo(sp3.x, sp3.y);
			tile.graphics.lineTo(sp0.x, sp0.y);
		}
		
	}
}