//VB String Function For C
//Author: Tesla.Angela
//Create Time: 2010-12-05
//Last Updata: 2011-02-26

#include <string.h>
#include <ctype.h>

typedef char* CSTR;

//==========//
//字符串数组//
//==========//

typedef struct _STRARY {
    long strmaxlength;
    long strcount;
    char *buffer;
} STRARY, *PSTRARY;

void AllocStringArray(PSTRARY psa, long strmaxlength, long strcount)
{
    psa->strcount = strcount; //从1数起
    psa->strmaxlength = strmaxlength + 1; //包括'\0'
    psa->buffer = (char *)malloc(psa->strcount * psa->strmaxlength);
    memset(psa->buffer, 0x0, psa->strcount * psa->strmaxlength);
}

void ReDimStringArray(PSTRARY psa, long strmaxlength, long strcount, long preserve)
{
    char *oldbuf = psa->buffer;
    long oldmaxlen = psa->strmaxlength;
    long oldstrcnt = psa->strcount;
    long i = 0;

    if (preserve == 0 || (preserve == 1 && strmaxlength + 1 >= oldmaxlen)) //如果(抛弃以前的内容)或者(保存以前的内容且新长度大于旧长度)
        psa->strmaxlength = strmaxlength + 1;

    psa->strcount = strcount;
    psa->buffer = (char *)malloc(psa->strcount * psa->strmaxlength);
    memset(psa->buffer, 0x0, psa->strcount * psa->strmaxlength);

    if (preserve == 0) return;
    else {
        for (i = 0; i < oldstrcnt; i++)
            memcpy(psa->buffer + psa->strmaxlength * i, oldbuf + oldmaxlen * i, strlen(oldbuf + oldmaxlen * i) + 1);
    }
}

void SetArrayString(PSTRARY psa, long stringid, char *string) //stringid从0数起
{
    memcpy(psa->buffer + stringid * psa->strmaxlength, string, strlen(string) + 1);
}

char *GetArrayString(PSTRARY psa, long stringid)
{
    char *newstr = (char *)malloc(psa->strmaxlength);
    memset(newstr, 0x0, psa->strmaxlength);
    strcpy(newstr, psa->buffer + stringid * psa->strmaxlength); //memcpy(newstr, psa->buffer + stringid * psa->strmaxlength, psa->strmaxlength);
    return newstr;
}

//========//
//其它操作//
//========//

char *cs(char *str1, char *str2) //connect string
{
    long newstrlen = strlen(str1) + strlen(str2) + 1;
    char *newstr = (char*)malloc(newstrlen);
    memcpy(newstr, str1, strlen(str1));
    memcpy(newstr + strlen(str1), str2, strlen(str2) + 1);
    return newstr;
}

char *ss(char *strSRC) //set string
{
    long strSRClen = strlen(strSRC);
    char *newstr = (char*)malloc(strSRClen + 1);
    memset(newstr, 0x0, strSRClen + 1);
    memcpy(newstr, strSRC, strSRClen + 1);
    return newstr;
}

char *LngToStr(long n, long radix)
{
    char *newstr = (char*)malloc(11);
    memset(newstr, 0x0, 11);
    ltoa(n, newstr, radix);
    return newstr;
}

char *DblToStr(double n)
{
    char *newstr = (char*)malloc(22);
    memset(newstr, 0x0, 22);
    gcvt(n, 10, newstr);
    return newstr;
}

long StrToLng(char *string)
{
    return atol(string);
}

double StrToDbl(char *string)
{
    return atof(string);
}

char *WSTR2STR(wchar_t *pwc)
{
    int requiredSize = wcstombs(NULL, pwc, 0);
    char *pmb = (char *)malloc(requiredSize + 1);
    wcstombs(pmb, pwc, requiredSize + 1);
    return pmb;
}

wchar_t *STR2WSTR(char *pmb)
{
    int requiredSize = mbstowcs(NULL, pmb, 0);
    wchar_t *pwc = (wchar_t *)malloc((requiredSize + 1) * sizeof(wchar_t));
    mbstowcs(pwc, pmb, requiredSize + 1);
    return pwc;
}

//========//
//完全实现//
//========//

long Len(char *string)
{
    return strlen(string);
}

char* Left(char *string, long length)
{
    long dwstrlen = strlen(string);
    char *newstr;

    if (length > dwstrlen)
        length = dwstrlen;

    newstr = (char*)malloc(length + 1);
    memset(newstr, 0x0, length + 1);
    memcpy(newstr, string, length);
    return newstr;
}

char *Right(char *string, long length)
{
    long dwstrlen = strlen(string);
    long dwstartpos = 0;
    char *newstr;

    if (length > dwstrlen)
        length = dwstrlen;

    dwstartpos = dwstrlen - length;
    newstr = (char*)malloc(length + 1);
    memcpy(newstr, string + dwstartpos, length + 1);
    return newstr;
}

char *Mid(char *string, long startpos, long length)
{
    long dwstrlen = strlen(string);
    long dwmaxcutlen = dwstrlen - startpos + 1;

    if (length == 0 || length > dwmaxcutlen)
        length = dwmaxcutlen;

    char *newstr = (char*)malloc(length + 1);
    memset(newstr, 0x0, length + 1);
    memcpy(newstr, string + startpos - 1, length);
    return newstr;
}

long InStr(long startpos, char *str1, char *str2, long txtcmp) //vbBinraryCompare=0;vbTextCompare=1
{
    char *str1new = (char*)malloc(strlen(str1) + 1);
    memset(str1new, 0x0, strlen(str1) + 1);
    strcpy(str1new, str1);
    char *str2new = (char*)malloc(strlen(str2) + 1);
    memset(str2new, 0x0, strlen(str2) + 1);
    strcpy(str2new, str2);
    char *posfound = NULL;

    if (txtcmp == 1) {
        str1new = strlwr(str1new);
        str2new = strlwr(str2new);
    }

    posfound = strstr(str1new + startpos - 1, str2new); //hello,world! <-> llo

    if (posfound == NULL)
        return 0;

    return (long)posfound - (long)str1new + 1;
}

long InStrRev(long startpos, char *str1, char *str2, long txtcmp) //vbBinraryCompare=0;vbTextCompare=1
{
    char *str1new = (char*)malloc(strlen(str1) + 1);
    memset(str1new, 0x0, strlen(str1) + 1);
    strcpy(str1new, str1);
    str1new = strrev(str1new);
    char *str2new = (char*)malloc(strlen(str2) + 1);
    memset(str2new, 0x0, strlen(str2) + 1);
    strcpy(str2new, str2);
    char *posfound = NULL;

    if (txtcmp == 1) {
        str1new = strlwr(str1new);
        str2new = strlwr(str2new);
    }

    posfound = strstr(str1new + startpos - 1, str2new); //hello,world! <-> llo

    if (posfound == NULL)
        return 0;

    return (long)posfound - (long)str1new + 1;
}

char *StrReverse(char *string)
{
    char *newstr = (char*)malloc(strlen(string) + 1);
    newstr = strdup(string);
    return strrev(newstr);
}

char *LCase(char *string)
{
    char *newstr = (char*)malloc(strlen(string) + 1);
    newstr = strdup(string);
    return strlwr(newstr);
}

char *UCase(char *string)
{
    char *newstr = (char*)malloc(strlen(string) + 1);
    newstr = strdup(string);
    return strupr(newstr);
}

char *String(long number, char character)
{
    char *newstr = (char*)malloc(number + 1);
    memset(newstr, 0x0, number + 1);
    memset(newstr, character, number);
    return newstr;
}

char *Space(long number)
{
    return String(number, ' ');
}

long StrComp(char *str1, char *str2, long txtcmp) //vbBinraryCompare=0;vbTextCompare=1
{
    return (txtcmp == 0 ? strcmp(str1, str2) : stricmp(str1, str2));
}

char *LTrim(char *string)
{
    long i = 0, cutlen = 0, dwstrlen = strlen(string);

    for (i = 0; i < dwstrlen + 1; i++) {
        if (string[i] == ' ')
            cutlen++;
        else
            break;
    }//printf("LTrim-Internal: %ld\n",cutlen);

    char *newstr = (char*)malloc(dwstrlen - cutlen + 1);
    memset(newstr, 0x0, dwstrlen - cutlen + 1);
    memcpy(newstr, &string[cutlen], dwstrlen - cutlen);
    return newstr;
}

char *RTrim(char *string)
{
    long i = 0, cutlen = 0, dwstrlen = strlen(string);

    for (i = dwstrlen - 1; i >= 0; i--) {
        if (string[i] == ' ')
            cutlen++;
        else
            break;
    }//printf("RTrim-Internal: %ld\n",cutlen);

    char *newstr = (char*)malloc(dwstrlen - cutlen + 1);
    memset(newstr, 0x0, dwstrlen - cutlen + 1);
    memcpy(newstr, string, dwstrlen - cutlen);
    return newstr;
}

char *Trim(char *string)
{
    char *newstr = LTrim(RTrim(string));
    return newstr;
}

long Asc(char c)
{
    return (long)c;
}

char Chr(long l)
{
    return (char)l;
}

char *Hex(long n)
{
    char *newstr = (char*)malloc(11);
    memset(newstr, 0x0, 11);
    ltoa(n, newstr, 16);
    return newstr;
}

long IsNumeric(char *string) //YES=1,NO=0
{
    long i = 0, dwstrlen = strlen(string);

    for (i = 0; i < dwstrlen; i++) {
        if (string[i] != 'a' && string[i] != 'b' && string[i] != 'c' && string[i] != 'd' && string[i] != 'e' && string[i] != 'f' &&
            string[i] != 'A' && string[i] != 'B' && string[i] != 'C' && string[i] != 'D' && string[i] != 'E' && string[i] != 'F' &&
            string[i] != '0' && string[i] != '1' && string[i] != '2' && string[i] != '3' && string[i] != '4' && string[i] != '5' &&
            string[i] != '6' && string[i] != '7' && string[i] != '8' && string[i] != '9') {
            return 0;
        }
    }

    return 1;
}

char *Replace(char *source, char *sub, char *rep, long startpos, long count, long txtcmp)
{
    char *result = NULL, *oldsrc = NULL;
    char *pc1, *pc2, *pc3;
    int isource, isub, irep, icnt = 0, isbk = 0;
    oldsrc = source;
    source = source + startpos - 1;
    isub = strlen(sub);
    irep = strlen(rep);
    isource = strlen(source);

    if (0 == *sub)
        return strdup(source);

    result = (char *)malloc(((irep > isub) ? strlen(source) / isub * irep + 1 : isource) * sizeof(char));
    pc1 = result;

    while (*source != 0) {
        pc2 = source;
        pc3 = sub;

        if (txtcmp == 0) {
            while (*pc2 == *pc3 && *pc3 != 0 && *pc2 != 0)
                pc2++, pc3++;
        } else {
            while (tolower(*pc2) == tolower(*pc3) && *pc3 != 0 && *pc2 != 0)
                pc2++, pc3++;
        }

        if (0 == *pc3) {
            pc3 = rep;

            while (*pc3 != 0)
                *pc1++ = *pc3++;

            pc2--;
            source = pc2;
            icnt++;

            if (icnt == count) {
                isbk = 1;
                break;
            }
        } else
            *pc1++ = *source;

        source++;
    }

    *pc1 = 0;

    if (startpos == 1 && count == -1)
        return result;

    char *newret = (char *)malloc(startpos + strlen(result));
    memset(newret, 0x0, startpos + strlen(result));
    memcpy(newret, oldsrc, startpos - 1);
    strcpy(newret + startpos - 1, result);

    if (isbk == 1) {
        source++;
        newret = cs(newret, source);
    }

    return newret;
}

//==========//
//不完全实现//
//==========//

long CountSubString(char *string, char *delimiter, long txtcmp)
{
    long cnt = 0, i = 0;

    while (1) {
        if (txtcmp == 0)
            i = InStr(i + 1, string, delimiter, 0);
        else
            i = InStr(i + 1, LCase(string), LCase(delimiter), 0);

        if (i != 0)
            cnt++;
        else
            return cnt;
    }

    return cnt;
}

char *Split_(char *string, char *delimiter, long stringid, long txtcmp)
{
    long dwlast = 0, dwnext = 0, dwmaxsplit = 0, i = 0, cutlen = 0, dlmlen = 0, dwl = 0, dwr = 0, dwt = 0, startcut = 0;
    char *newstr;
    dlmlen = strlen(delimiter);
    dwmaxsplit = CountSubString(string, delimiter, txtcmp);

    if (txtcmp == 0)
        dwlast = InStr(1, string, delimiter, 0); //xxxx->yyy   xxxx.yyy
    else
        dwlast = InStr(1, LCase(string), LCase(delimiter), 0); //xxxx->yyy   xxxx.yyy

    if (txtcmp == 0)
        dwnext = InStr(dwlast + 1, string, delimiter, 0); //xxxx->yyy->zz->qq->ww->ee
    else
        dwnext = InStr(dwlast + 1, LCase(string), LCase(delimiter), 0); //xxxx->yyy->zz->qq->ww->ee

    if (dwlast == 0 && dwnext == 0)
        return string;

    if (stringid == 1) {
        newstr = (char *)malloc(dwlast);
        memset(newstr, 0x0, dwlast);
        memcpy(newstr, string, dwlast - 1);
        return newstr;
    }

    if (stringid == dwmaxsplit + 1) {
        for (i = 1; i < stringid; i++) {
            if (txtcmp == 0)
                dwlast = InStr(dwlast + 1, string, delimiter, 0);
            else
                dwlast = InStr(dwlast + 1, LCase(string), LCase(delimiter), 0);

            i++;
        }//printf("Split-Internal: %ld\n",dwlast);

        cutlen = strlen(string + dwlast + dlmlen - 1) + 1;
        newstr = (char*)malloc(cutlen); //printf("Split-Internal: %ld\n",cutlen);
        memset(newstr, 0x0, cutlen);
        memcpy(newstr, string + dwlast + dlmlen - 1, cutlen);
        return newstr;
    }

    if (stringid >= 2 && stringid <= dwmaxsplit) {
        dwt = dwlast;

        for (i = 1; i < stringid; i++) {
            if (txtcmp == 0)
                dwlast = InStr(dwlast + 1, string, delimiter, 0);
            else
                dwlast = InStr(dwlast + 1, LCase(string), LCase(delimiter), 0);

            i++;
        }

        dwr = dwlast;
        dwlast = dwt;

        for (i = 1; i < stringid - 1; i++) {
            if (txtcmp == 0)
                dwlast = InStr(dwlast + 1, string, delimiter, 0);
            else
                dwlast = InStr(dwlast + 1, LCase(string), LCase(delimiter), 0);

            i++;
        }

        dwl = dwlast; //printf("Split-Internal: %ld;%ld\n",dwl,dwr);
        cutlen = dwr - dwl - dlmlen;
        startcut = dwl + dlmlen - 1;
        newstr = (char *)malloc(cutlen + 1);
        memset(newstr, 0x0, cutlen + 1);
        memcpy(newstr, string + startcut, cutlen);
        return newstr;
    }

    return NULL;
}

PSTRARY Split(char *string, char *delimiter, long count, long txtcmp)
{
    PSTRARY psa = (PSTRARY)malloc(sizeof(STRARY));
    long i = 0;

    if (count == -1)
        count = CountSubString(string, delimiter, txtcmp) + 1;

    AllocStringArray(psa, strlen(string) + 1, count);

    for (i = 0; i < count; i++)
        SetArrayString(psa, i, Split_(string, delimiter, i + 1, txtcmp)); //printf("Split-Internal: %s\n",Split_(string,delimiter,i+1,txtcmp));

    return psa;
}

char *Join(PSTRARY psa, char *delimiter)
{
    long i = 0;
    char *tmpstr = NULL, *retstr = NULL;
    retstr = ss(GetArrayString(psa, 0));

    for (i = 1; i < psa->strcount; i++) {
        tmpstr = GetArrayString(psa, i);
        retstr = cs(cs(retstr, "->"), tmpstr);
    }

    return retstr;
}

PSTRARY Filter(PSTRARY opsa, char *substr, long isinc, long txtcmp)
{
    PSTRARY psa = (PSTRARY)malloc(sizeof(STRARY));
    long i = 0, tmppos = 0, incstr = 0, noincstr = 0, j = 0;
    char *tmpstr = NULL;

    for (i = 0; i < opsa->strcount; i++) {
        tmppos = InStr(1, GetArrayString(opsa, i), substr, txtcmp);

        if (tmppos == 0)
            noincstr++;
        else
            incstr++;
    }

    if (isinc == 0)
        AllocStringArray(psa, opsa->strmaxlength, noincstr);
    else
        AllocStringArray(psa, opsa->strmaxlength, incstr);

    for (i = 0; i < opsa->strcount; i++) {
        tmpstr = GetArrayString(opsa, i); //printf("Filter-Internal: %s\n",tmpstr);
        tmppos = InStr(1, tmpstr, substr, txtcmp);

        if (tmppos == 0 && isinc == 0) {
            SetArrayString(psa, j, tmpstr);
            j++;
        } else if (tmppos != 0 && isinc != 0) {
            SetArrayString(psa, j, tmpstr);
            j++;
        }
    }

    return psa;
}

PSTRARY Array(char *string) //char *ts="qq\0www\0eeee\0r\0" //每个字符串后面都加个'\0'
{
    PSTRARY psa = (PSTRARY)malloc(sizeof(STRARY));
    long wholelen = 0, substrcnt = 0, i = 0, j = 0, stringlength;
    char c1 = 0, c2 = 0, *tmpstr = NULL;

    while (1) {
        c1 = *(string + i);
        c2 = *(string + i + 1);

        if (c1 == 0)
            substrcnt++;

        if (c1 == 0 && c2 == 0) {
            wholelen = i + 1;
            break;
        }

        i++;
    }//printf("substr=%ld;strlen=%ld",substrcnt,wholelen);

    AllocStringArray(psa, wholelen + 1 - substrcnt, substrcnt);

    for (j = 0; j < substrcnt; j++) {
        stringlength = strlen(string);
        tmpstr = (char *)malloc(stringlength + 1);
        memset(tmpstr, 0x0, stringlength + 1);
        strcpy(tmpstr, string);
        SetArrayString(psa, j, tmpstr);
        string = string + stringlength + 1;
    }

    return psa;
}
