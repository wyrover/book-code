#ifdef __cplusplus
extern "C" {
#endif

UINT CALLBACK printHook(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
int encodeYYMM(WORD yy, WORD mm);
int decodeYY(int yymm);
int decodeMM(int mm);

#ifdef __cplusplus
}
#endif
