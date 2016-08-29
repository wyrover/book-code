class CBitmapData {
  public:
	CBitmapData(HBITMAP hbm);	
	virtual ~CBitmapData();
	LPBITMAPINFO getBITMAPINFO() {return pbmi; };
	LPVOID getBits() { return bits; }
	friend operator==(const CBitmapData & p1, const CBitmapData & p2);
	friend operator!=(const CBitmapData & p1, const CBitmapData & p2);
  private:
	LPBITMAPINFO pbmi;
	LPVOID bits;
	HBITMAP hbm;
		  };
operator==(const CBitmapData & p1, const CBitmapData & p2);
__inline operator!=(const CBitmapData & p1, const CBitmapData & p2) 
			{ return !(p1 == p2); }
