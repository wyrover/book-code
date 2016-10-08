package
{
	import flash.display.DisplayObject;
	import flash.display.Sprite;
	import flash.events.Event;
	import flash.geom.Vector3D;
	
	[SWF(width=800, height=800, backgroundColor = 0xffffff)]
	public class Carousel extends Sprite
	{
		private var _holder:Sprite;
		private var _items:Array;
		private var _radius:Number = 200;
		private var _numItems:int = 10;
		
		public function Carousel()
		{
			root.transform.perspectiveProjection.fieldOfView = 130;
			_holder = new Sprite();
			_holder.x = stage.stageWidth / 2;
			_holder.y = stage.stageHeight / 2;
			_holder.z = 0;
			addChild(_holder);
			
			_items = new Array();
			
			_radius = 200;
			for(var i:int = 0; i < _numItems; i++)
			{
				var angle:Number = Math.PI * 2 / _numItems * i;
				var item:Sprite = makeItem();
				item.x = Math.cos(angle) * _radius;
				item.z = Math.sin(angle) * _radius;
				item.rotationY = -360 / _numItems * i + 90;
				_items.push(item);
			}
			sortItems();
			
			addEventListener(Event.ENTER_FRAME, onEnterFrame);
		}
		
		private function makeItem():Sprite
		{
			var item:Sprite = new Sprite();
			item.graphics.beginFill(Math.random() * 0xffffff);
			item.graphics.drawRect(-50, -50, 100, 100);
			_holder.addChild(item);
			return item;
		}
		
		private function sortItems():void
		{
			_items.sort(depthSort);
			for(var i:int = 0; i < _items.length; i++)
			{
				_holder.addChildAt(_items[i] as Sprite, i);
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
			_holder.rotationY += (stage.stageWidth / 2 - mouseX) * .01;
			_holder.y += (mouseY - _holder.y) * .1;
			sortItems();
		}
	}
}
