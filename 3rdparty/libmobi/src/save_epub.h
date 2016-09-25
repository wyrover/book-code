/*
 * Added by Grzegorz Kochaniak, greg@hyperionics.com, in Feb. 2016 -
 */

#ifndef SAVE_EPUB_H
#define SAVE_EPUB_H

#include "mobi.h"

#ifdef __cplusplus
extern "C" {
int epub_rawml_parts(const MOBIRawml *rawml, const char *epub_fn);
MOBIRawml* loadMobiRawml(MOBIData *m, const char *mobiFn, const char* pid = NULL, bool parse_kf7_opt = false);
int convertMobiToEpub(const char* mobiFn, const char* epubFn, const char* pid = NULL, bool parse_kf7_opt = false); 
}
#else

extern int epub_rawml_parts(const MOBIRawml *rawml, const char *epub_fn);
extern MOBIRawml* loadMobiRawml(MOBIData *m, const char *mobiFn, const char* pid, bool parse_kf7_opt);
extern int convertMobiToEpub(const char* mobiFn, const char* epubFn, const char* pid, bool parse_kf7_opt);

#endif
#endif
