typedef struct {
		BOOL standard;		// true if standard pizza

		short size;		// size of pizza
		LPTSTR name;		// name of person of standard

		BOOL veggie;		// TRUE for veggie pizza only
		DWORD toppings;		// topping bitmap

		// Output parameters only:
		BOOL update_standard;   // TRUE if user asks to change
	       } PIZZA, *LPPIZZA;

#define SIZE_ERR    (-1)   // used internally in dialog handler

#define SIZE_SMALL  0
#define SIZE_MEDIUM 1
#define SIZE_LARGE  2


#define TOPPING_XCHEESE     0x00000001
#define TOPPING_PEPPERONI   0x00000002
#define TOPPING_SAUSAGE     0x00000004
#define TOPPING_ONIONS      0x00000008
#define TOPPING_MUSHROOMS   0x00000010
#define TOPPING_ANCHOVIES   0x00000020
#define TOPPING_PINEAPPLE   0x00000040
#define TOPPING_HOTPEPPERS  0x00000080
#define TOPPING_BLACKOLIVES 0x00000100
#define TOPPING_GREENOLIVES 0x00000200
#define TOPPING_BACON       0x00000400
#define TOPPING_SALAMI      0x00000800

#define TOPPING_MEATS (TOPPING_PEPPERONI | TOPPING_SAUSAGE | \
                       TOPPING_BACON | TOPPING_SALAMI)

#define TOPPING_ALL 0x00000FFF

LPPIZZA getPizzaSelection(HWND hwnd);
BOOL getPizzaOrder(HWND hwnd, LPPIZZA pizza);
BOOL orderPizza(HWND hwnd, LPPIZZA pizza);
void pizzaOrderFailure(HWND hwnd, LPPIZZA pizza);
void releasePizza(LPPIZZA pizza);

