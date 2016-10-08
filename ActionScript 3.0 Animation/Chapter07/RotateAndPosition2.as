package
{
	import flash.display.DisplayObject;
	import flash.display.Shape;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Vector3D;
	
	[SWF(width=800, height=800, backgroundColor = 0xffffff)]
	public class RotateAndPosition2 extends Sprite
	{
		private var _holder:Sprite;
		private var _shapes:Array;
		
		public function RotateAndPosition2()
		{
			_holder = new Sprite();
			_holder.x = stage.stageWidth / 2;
			_holder.y = stage.stageHeight / 2;
			_holder.z = 0;
			addChild(_holder);
			
			var shape1:Shape = makeShape();
			shape1.z = 200;
			
			var shape2:Shape = makeShape();
			shape2.z = -200;
			
			var shape3:Shape = makeShape();
			shape3.x = 200;
			shape3.rotationY = 90;
			
			var shape4:Shape = makeShape();
			shape4.x = -200;
			shape4.rotationY = -90;
			
			var shape5:Shape = makeShape();
			shape5.y = 200;
			shape5.rotationX = 90;
			
			var shape6:Shape = makeShape();
			shape6.y = -200;
			shape6.rotationX = 90;
			
			_shapes = [shape1, shape2, shape3, shape4, shape5, shape6];
			sortShapes();
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function makeShape():Shape
		{
			var shape:Shape = new Shape();
			shape.graphics.beginFill(Math.random() * 0xffffff);
			shape.graphics.drawRect(-100, -100, 200, 200);
			_holder.addChild(shape);
			return shape;
		}
		
		private function sortShapes():void
		{
			_shapes.sort(depthSort);
			for(var i:int = 0; i < _shapes.length; i++)
			{
				_holder.addChildAt(_shapes[i] as Shape, i);
			}
		}
		
		private function depthSort(objA:DisplayObject, objB:DisplayObject):int
		{
			var posA:Vector3D = objA.transform.matrix3D.position;
			posA = _holder.transform.matrix3D.deltaTransformVector(posA);
			var posB:Vector3D = objB.transform.matrix3D.position;
			posB = _holder.transform.matrix3D.deltaTransformVector(posB);
			return posB.z - posA.z;
		}
		
		private function onEnterFrame(event:Event):void
		{
			_holder.rotationY += .5;
			_holder.rotationX += 2;
			sortShapes();
		}
	}
}
