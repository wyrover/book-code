#include <string.h>
#include <string>

// ==========================================================================
enum { SURROGATE_LEAD_FIRST = 0xD800 };
enum { SURROGATE_TRAIL_FIRST = 0xDC00 };
enum { SURROGATE_TRAIL_LAST = 0xDFFF };

static unsigned int UTF8Length(const wchar_t *uptr, size_t tlen) {
	size_t len = 0;
	for (size_t i = 0; i < tlen && uptr[i];) {
		unsigned int uch = uptr[i];
		if (uch < 0x80) {
			len++;
		} else if (uch < 0x800) {
			len += 2;
		} else if ((uch >= SURROGATE_LEAD_FIRST) &&
			(uch <= SURROGATE_TRAIL_LAST)) {
			len += 4;
			i++;
		} else {
			len += 3;
		}
		i++;
	}
	return len;
}

static void UTF8FromUTF16(const wchar_t *uptr, size_t tlen, char *putf, size_t len) {
	int k = 0;
	for (size_t i = 0; i < tlen && uptr[i];) {
		unsigned int uch = uptr[i];
		if (uch < 0x80) {
			putf[k++] = static_cast<char>(uch);
		} else if (uch < 0x800) {
			putf[k++] = static_cast<char>(0xC0 | (uch >> 6));
			putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
		} else if ((uch >= SURROGATE_LEAD_FIRST) &&
			(uch <= SURROGATE_TRAIL_LAST)) {
			// Half a surrogate pair
			i++;
			unsigned int xch = 0x10000 + ((uch & 0x3ff) << 10) + (uptr[i] & 0x3ff);
			putf[k++] = static_cast<char>(0xF0 | (xch >> 18));
			putf[k++] = static_cast<char>(0x80 | ((xch >> 12) & 0x3f));
			putf[k++] = static_cast<char>(0x80 | ((xch >> 6) & 0x3f));
			putf[k++] = static_cast<char>(0x80 | (xch & 0x3f));
		} else {
			putf[k++] = static_cast<char>(0xE0 | (uch >> 12));
			putf[k++] = static_cast<char>(0x80 | ((uch >> 6) & 0x3f));
			putf[k++] = static_cast<char>(0x80 | (uch & 0x3f));
		}
		i++;
	}
	putf[len] = '\0';
}

static size_t UTF16Length(const char *s, unsigned int len) {
	size_t ulen = 0;
	size_t charLen;
	for (size_t i=0; i<len;) {
		unsigned char ch = static_cast<unsigned char>(s[i]);
		if (ch < 0x80) {
			charLen = 1;
		} else if (ch < 0x80 + 0x40 + 0x20) {
			charLen = 2;
		} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
			charLen = 3;
		} else {
			charLen = 4;
			ulen++;
		}
		i += charLen;
		ulen++;
	}
	return ulen;
}

static size_t UTF16FromUTF8(const char *s, size_t len, wchar_t *tbuf, size_t tlen) {
	size_t ui=0;
	const unsigned char *us = reinterpret_cast<const unsigned char *>(s);
	size_t i=0;
	while ((i<len) && (ui<tlen)) {
		unsigned char ch = us[i++];
		if (ch < 0x80) {
			tbuf[ui] = ch;
		} else if (ch < 0x80 + 0x40 + 0x20) {
			tbuf[ui] = static_cast<wchar_t>((ch & 0x1F) << 6);
			ch = us[i++];
			tbuf[ui] = static_cast<wchar_t>(tbuf[ui] + (ch & 0x7F));
		} else if (ch < 0x80 + 0x40 + 0x20 + 0x10) {
			tbuf[ui] = static_cast<wchar_t>((ch & 0xF) << 12);
			ch = us[i++];
			tbuf[ui] = static_cast<wchar_t>(tbuf[ui] + ((ch & 0x7F) << 6));
			ch = us[i++];
			tbuf[ui] = static_cast<wchar_t>(tbuf[ui] + (ch & 0x7F));
		} else {
			// Outside the BMP so need two surrogates
			int val = (ch & 0x7) << 18;
			ch = us[i++];
			val += (ch & 0x3F) << 12;
			ch = us[i++];
			val += (ch & 0x3F) << 6;
			ch = us[i++];
			val += (ch & 0x3F);
			tbuf[ui] = static_cast<wchar_t>(((val - 0x10000) >> 10) + SURROGATE_LEAD_FIRST);
			ui++;
			tbuf[ui] = static_cast<wchar_t>((val & 0x3ff) + SURROGATE_TRAIL_FIRST);
		}
		ui++;
	}
	return ui;
}

wchar_t* StringFromUTF8(const char *s) {
	if(!s) return NULL;
	size_t sLen = s ? strlen(s) : 0;
	size_t wideLen = UTF16Length(s, sLen);
	wchar_t* vgc = new wchar_t[wideLen + 1];
	size_t outLen = UTF16FromUTF8(s, sLen, vgc, wideLen);
	vgc[outLen] = 0;
	return vgc;
}

char* UTF8FromString(const std::wstring &s) {
	size_t sLen = s.size();
	size_t narrowLen = UTF8Length(s.c_str(), sLen);
	char* vc = new char[narrowLen + 1];
	UTF8FromUTF16(s.c_str(), sLen, vc, narrowLen);
	return vc;
}
// -------------------------------------------------------------------