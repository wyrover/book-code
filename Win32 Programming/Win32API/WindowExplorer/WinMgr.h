class CWindowManager {
	public:
		void Add(HWND hwnd);
		void Remove(HWND hwnd);
		POSITION getFirst() { return windowlist.GetHeadPosition(); }
		HWND getNext(POSITION & pos) { 
					return windowlist.GetNext(pos);}
	protected:
		static CList<HWND, HWND> windowlist;
		     };
