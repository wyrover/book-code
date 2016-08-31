// Codes for font selection

#define SEL_STOCK 0
#define SEL_CUSTOM 1

// Codes for brushsel

//#define SEL_STOCK 0  // (same as for font selection)
#define SEL_SOLID 1
#define SEL_HATCH 2

// Codes for penSel
//#define SEL_STOCK 0  // (same as for font selection)
#define SEL_COSMETIC  1
#define SEL_GEOMETRIC 2

class CDCValues {
public:
	void setDC(CDC * dc);
	BOOL copyTo(CDCValues * target); 
	CDCValues();
	~CDCValues();
public: // data
	COLORREF BkColor;
	int BkMode;  // OPAQUE | TRANSPARENT
	int align;

	CBrush * brush;

	POINT brushorg;
	POINT penposition;
	int rop;

	CFont * font;

	int textcharextra;

	int graphicsmode;
	int mapmode;

	CPen * pen;

	int fillmode;
	COLORREF textcolor;
	float miterlimit;

	XFORM matrix;
	POINT vext;
	POINT wext;
	POINT vorg;
	POINT worg;

	// The color used to paint the window.  We don't support
	// general brush styles here

	COLORREF wcolor;

	// Values used to track selection in the dialog, but not actually
	// part of the DC state
	//
	// ----- Global -----
	int current_bkmode;
	COLORREF current_bkcolor;

	// ----- Pen -----
	int pensel;
	int stockpen;
	COLORREF pensolid;
	int endcaps;
	int linejoin;
	int lbstyle;
	int penwidth;
	int penstyle;

	// ----- Font -----
	int stockfont;
	int fontsel;

	// ----- Brush -----
	int brushsel;
	int stockbrush; // selection with stock brush dropdown
	int stocksel; // selection within stock dropdown
	int hatchsel; // selection with hatch dropdown
	COLORREF solidsel; // selection within colors dropdown

	// ----- Mapping mode -----
	int ticks;   // value to reset for ticks

	// ----- Last page -----
	int lastpage;

	// ----- Miter angle for demo -----
	int angle;
		};
