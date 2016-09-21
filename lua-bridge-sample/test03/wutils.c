#define WINDOWS_LEAN_AND_MEAN
#include <windows.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#define MAX_KEY MAX_PATH

#define eq(s1,s2) (strcmp(s1,s2)==0)

typedef int Ref;

/// make a reference to a Lua object.
// @param L the state
// @param idx the index of the value on the stack.
// @return a reference (an integer value)
// @function make_ref
Ref make_ref(lua_State *L, int idx) {
  lua_pushvalue(L,idx);
  return luaL_ref(L,LUA_REGISTRYINDEX);
}

/// release a reference to a Lua value.
// @param L the state
// @param ref the reference
// @function release_ref
void release_ref(lua_State *L, Ref ref) {
  luaL_unref(L,LUA_REGISTRYINDEX,ref);
}

/// push a referenced value on the stack.
// @param L the state
// @param ref the reference
// @return 1; the value is on the stack
// @function push_ref
int push_ref(lua_State *L, Ref ref) {
  lua_rawgeti(L,LUA_REGISTRYINDEX,ref);
  return 1;
}

const char *last_error(int err) {
  static char errbuff[256];
  int sz;
  if (err == 0) {
    err = GetLastError();
  }
  sz = FormatMessage(
    FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
    NULL,err,
    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
    errbuff, 256, NULL
  );
  errbuff[sz-2] = '\0'; // strip the \r\n
  return errbuff;
}

/// push a error message.
// @param L the state
// @param msg a message string
// @return 2; `nil` and the message on the stack
// @function push_error_msg
int push_error_msg(lua_State *L, const char *msg) {
  lua_pushnil(L);
  lua_pushstring(L,msg);
  return 2;
}

/// push the last Windows error.
// @param L the state
// @return 2; 'nil' and the message
// @function push_error
int push_error(lua_State *L) {
  return push_error_msg(L,last_error(0));
}

/// push a particular Windows error.
// @param L the state
// @param err the error code
// @return 2; 'nil' and the message
// @function push_error
int push_error_code(lua_State *L, int err) {
  return push_error_msg(L,last_error(err));
}


/// push a true value.
// @param L the state
// @return 1; a boolean true value
// @function push_ok
int push_ok(lua_State *L) {
  lua_pushboolean(L,1);
  return 1;
}

/// push error if needed.
// @param L the state
// @param bval 0 or 1
// @return 1; a boolean true value, or 2, `nil`, last Windows error
// @function push_bool
int push_bool(lua_State *L, int bval) {
  if (bval) {
    return push_ok(L);
  } else {
    return push_error(L);
  }
}

/// throw a Lua error, dumping to the debug monitor.
// @param L the state
// @param msg a message
// @function throw_error
void throw_error(lua_State *L, const char *msg) {
  OutputDebugString(last_error(0));
  OutputDebugString(msg);
  lua_pushstring(L,msg);
  lua_error(L);
}

BOOL call_lua_direct(lua_State *L, Ref ref, int idx, const char *text, int discard) {
  BOOL res,ipush = 1;
  if (idx < 0)
    lua_pushvalue(L,idx);
  else if (idx > 0)
    lua_pushinteger(L,idx);
  else
    ipush = 0;
  push_ref(L,ref);
  if (idx != 0)
    lua_pushvalue(L,-2);
  if (text != NULL) {
    lua_pushstring(L,text);
    ++ipush;
    free((char*)text);
  }
  lua_call(L, ipush, 1);
  res = lua_toboolean(L,-1);
  if (discard) {
    release_ref(L,ref);
  }
  return res;
}

// Calling back to Lua /////
// For console applications, we just use a mutex to ensure that Lua will not
// be re-entered, but if use_gui() is called, we use a message window to
// make sure that the callback happens on the main GUI thread.

typedef struct {
  lua_State *L;
  Ref ref;
  int idx;
  const char *text;
  int discard;
} LuaCallParms;

#define MY_INTERNAL_LUA_MESSAGE WM_USER+42

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  WNDPROC lpPrevWndProc;
  if (uMsg == MY_INTERNAL_LUA_MESSAGE) {
    BOOL res;
    LuaCallParms *P  = (LuaCallParms*)lParam;
    res = call_lua_direct(P->L,P->ref,P->idx,P->text,P->discard);
    free(P);
    return res;
  }

  lpPrevWndProc = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_USERDATA);
  if (lpPrevWndProc)
    return CallWindowProc(lpPrevWndProc, hwnd, uMsg, wParam, lParam);

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static BOOL s_use_mutex = TRUE;
static HWND hMessageWin = NULL;

/// start a message window for GUI Lua dispatching.
// @function make_message_windowS
void make_message_window() {
  if (hMessageWin == NULL) {
    LONG_PTR subclassedProc;
    s_use_mutex = FALSE;
    hMessageWin = CreateWindow(
      "STATIC", "winapi_Spawner_Dispatcher",
      0, 0, 0, 0, 0, 0, 0, GetModuleHandle(NULL), 0
    );
    subclassedProc = SetWindowLongPtr(hMessageWin, GWLP_WNDPROC, (LONG_PTR)WndProc);
    SetWindowLongPtr(hMessageWin, GWLP_USERDATA, subclassedProc);
  }
}

static HANDLE hMutex = NULL;

  //~ fprintf(stderr,"lock waiting %d\n",(int)GetCurrentThreadId());

void lock_mutex() {
  WaitForSingleObject(hMutex,INFINITE);
}

void setup_mutex() {
  hMutex = CreateMutex(NULL,TRUE,NULL);
}

void release_mutex() {
  ReleaseMutex(hMutex);
}

// this is a useful function to call a Lua function within an exclusive
// mutex lock. There are two parameters:
//
// - the first can be zero, negative or postive. If zero, nothing happens. If
// negative, it's assumed to be an index to a value on the stack; if positive,
// assumed to be an integer value.
// - the second can be NULL or some text. If NULL, nothing is pushed.
//

/// call a Lua function.
// This ensures that only one Lua function can be entered at any time, controlled
// by a mutex. If in 'GUI mode' then the Lua function is furthermore called
// from the GUI state.
// @param L the state
// @param ref a reference to the function
// @param idx a stack index: if greater than zero, pass value to function
// @param text a string: if not NULL, pass this string to the function
// @param discard if 1, then remove the reference after calling
// @function call_lua
BOOL call_lua(lua_State *L, Ref ref, int idx, const char *text, int discard) {
  BOOL res;
  if (text) {
    size_t len = strlen(text);
    char *mtext = (char *)malloc(len+1);
    memcpy(mtext,text,len+1);
    text = mtext;
  }
  if (s_use_mutex) {
    lock_mutex();
    res = call_lua_direct(L,ref,idx,text,discard);
    release_mutex();
  } else {
    LuaCallParms *parms = (LuaCallParms*)malloc(sizeof(LuaCallParms));
    parms->L = L;
    parms->ref = ref;
    parms->idx = idx;
    parms->text = text;
    parms->discard = discard;
    PostMessage(hMessageWin,MY_INTERNAL_LUA_MESSAGE,0,(LPARAM)parms);
    res = FALSE; // for now
  }

  return res;
}

static int current_encoding = CP_ACP;

/// set the encoding.
// Will be one of `CP_ACP` or `CP_UTF8`
// @param e
// @function set_encoding
void set_encoding(int e) {
  current_encoding = e;
}

/// get the encoding.
// @return current encoding
// @function set_encoding
int get_encoding() {
  return current_encoding;
}

/// convert text to UTF-16 depending on encoding.
// @param text the input multi-byte text
// @param wbuf the output wide char text
// @param bufsz the size of the output buffer.
// @return a pointer to `wbuf`
// @function wstring_buff
LPWSTR wstring_buff(LPCSTR text, LPWSTR wbuf, int bufsz) {
  int res = MultiByteToWideChar(
    current_encoding, 0,
    text,-1,
    wbuf,bufsz);
  if (res != 0) {
    return wbuf;
  } else {
    return NULL; // how to indicate error, hm??
  }
}

/// push a wide string on the Lua stack with given size.
// This converts to the current encoding first.
// @param L the State
// @param us the wide string
// @param len size of wide string
// @return 1; the encoded string or 2, `nil` and the error message
// @function push_wstring_l
int push_wstring_l(lua_State *L, LPCWSTR us, int len) {
  int osz = 3*len;
  char *obuff = malloc(osz);
  int res = WideCharToMultiByte(
    current_encoding, 0,
    us,len,
    obuff,osz,
    NULL,NULL);
  if (res == 0) {
    free(obuff);
    return push_error(L);
  } else {
    lua_pushlstring(L,obuff,res);
    free(obuff);
    return 1;
  }
}

/// push a wide string on the Lua stack.
// @param L the state
// @param us the wide string
// @return 1; the encoded string or 2, `nil` and the error message
// @function push_wstring
int push_wstring(lua_State *L,LPCWSTR us) {
  int len = wcslen(us);
  return push_wstring_l(L,us,len);
}

static HKEY predefined_keys(LPCSTR key) {
  #define check(predef) if (eq(key,#predef)) return predef;
  check(HKEY_CLASSES_ROOT);
  check(HKEY_CURRENT_CONFIG);
  check(HKEY_CURRENT_USER);
  check(HKEY_LOCAL_MACHINE);
  check(HKEY_USERS);
  #undef check
  return NULL;
}

#define SLASH '\\'

/// split a full registry path into predefined key and subkey path.
// @param path the input full path
// @param keypath the output path
// @return the predefined HKEY
// @function split_registry_key
HKEY split_registry_key(LPCSTR path, char *keypath) {
  char key[MAX_KEY];
  LPCSTR slash = strchr(path,SLASH);
  int i = (int)((DWORD_PTR)slash - (DWORD_PTR)path);
  strncpy(key,path,i);
  key[i] = '\0';
  strcpy(keypath,path+i+1);
  return predefined_keys(key);
}


/// map message box names onto corresponding constants.
// @param name string like "ok" or "ok-cancel"
// @return MB_OK, MB_OKCANCEL, etc
// @function mb_const
int mb_const (LPCSTR name) {
  if (eq(name,"ok")) return MB_OK;
  if (eq(name,"abort-retry-ignore")) return MB_ABORTRETRYIGNORE;
  if (eq(name,"ok-cancel")) return MB_OKCANCEL;
  if (eq(name,"retry-cancel")) return MB_RETRYCANCEL;
  if (eq(name,"yes-no")) return MB_YESNO;
  if (eq(name,"yes-no-cancel")) return MB_YESNOCANCEL;
  // icons
  if (eq(name,"warning")) return MB_ICONWARNING;
  if (eq(name,"information")) return MB_ICONINFORMATION;
  if (eq(name,"question")) return MB_ICONQUESTION;
  if (eq(name,"error")) return MB_ICONERROR;
  return 0;
}

/// map message box returns to strings.
// @param res the return code (like IDOK)
// @return a string (like "ok")
// @function mb_result
LPCSTR mb_result (int res) {
  switch(res) {
  case IDABORT: return "abort";
  case IDCANCEL: return "cancel";
  case IDIGNORE: return "ignore";
  case IDNO: return "no";
  case IDOK: return "ok";
  case IDRETRY: return "retry";
  case IDYES: return "yes";
  default: return "?";
  }
}

