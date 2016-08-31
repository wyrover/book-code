
class menuitem {
	public:
		menuitem();
		~menuitem();
	public:
		DWORD flags;  // flags, see below
		HMENU hmenu;  // Menu handle of menu
		CString name; // name of menu (if relevant)
		int id;	      // ID of menu or command
		int uid;      // ID of user-defined menu
		
			};

// These flags are referenced by the ItemData field of the dropdown to describe
// special properties of the various menus.  This dropdown holds all the
// menu handles for the application.

#define M_READONLY 0x01  // menu is protected, may not be modified in any way
#define M_NODELETE 0x02  // menu is protected, may not be deleted
#define M_SYSMENU  0x04  // menu is a GetSysMenu menu
#define M_USER     0x08  // menu is a user-defined working menu
#define M_POPUP    0x10  // menu was created by CreatePopupMenu or by
			 // removal from an existing menu
#define M_MAIN	   0x20	 // menu is top-level main menu
#define M_UNASSIGNED 0x40 // menu has no assignment (new CreateMenu)
#define M_CONTEXT  0x80  // menu is a context menu
#define M_COMMAND  0x100 // menu item is a command item
#define M_EMPTY    0x200 // menu item is placeholder, not yet filled in
#define M_NOREMOVE 0x400 // menu item is protected, may not be removed or deleted
#define M_NOPARENT 0x800 // menu has no logical parent (but may have structural parent)
