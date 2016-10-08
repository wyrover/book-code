package gs.easing {
	public class Quad {
		public static function easeIn (t:Number, b:Number, c:Number, d:Number):Number {
			return c*(t/=d)*t + b;
		}
		public static function easeOut (t:Number, b:Number, c:Number, d:Number):Number {
			return -c *(t/=d)*(t-2) + b;
		}
		public static function easeInOut (t:Number, b:Number, c:Number, d:Number):Number {
			if ((t/=d/2) < 1) return c/2*t*t + b;
			return -c/2 * ((--t)*(t-2) - 1) + b;
		}
	}
}