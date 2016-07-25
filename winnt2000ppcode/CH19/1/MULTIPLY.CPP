-------------------- M U L T I P L Y . C P P ---------------------------


/* multiply.cpp  Multiplication of nonnegative
   integers forms the radix-2^32 product of arrays
   u[n] and v[m]. Returns the result in array
   w[n + m]. After Knuth, volume 2, section 4.3.1
   Copyright (C) 1994 John K. Gotwals
   ----------------------------------------------- */
void multiply(const int *u, const int *v, int *w,
               int n, int m) {
    int edisav, esisav;
    int carry;

__asm {
    mov edisav,edi      ; edi and esi must be preserved
    mov esisav,esi

    ; set w[m] to w[m+n-1] to zero inclusive
    mov eax,0
    mov ecx,n           ; ecx = n
    mov esi,w           ; esi -> w[0]
    mov edx,m           ; edx = m
    init:
        mov [esi+edx*4],eax
        inc edx
    loop init

    ; rI1 = ecx = i
    ; rI2 = esi = j
    ; rI3 = edi = i + j
    ; edx = k = carry
    ; rIn are the index registers of Knuth's MIX

    mov esi,m
    dec esi                   ; j = m - 1
    h1:                       ; M2. zero multiplier?
      mov ebx,v               ; ebx -> v[0]
      mov edx,[ebx+esi*4]
      cmp edx,0 ;if v[j] = 0, goto h8 and set w[j] = 0
      je h8
      mov ecx,n               ; M3. initialize i = n;
      lea edi,[ecx+esi]       ; (i+j) = (n+j)
      dec ecx                 ; i = n - 1
      mov edx,0               ; k = 0
      h2:                     ; M4. multiply and add
        mov carry,edx
        mov ebx,u
        mov eax,[ebx+ecx*4] ; eax = u[i]
        mov ebx,v
        mul DWORD PTR[ebx+esi*4] ; edx:eax=u[i]*v[j]
        mov ebx,w
        add eax,[ebx+edi*4] ; add w[i+j] to lower half
        adc edx,0       ; add carry bit into upper half
        add eax,carry   ; add k to lower half
        adc edx,0       ; add carry bit into upper half
        mov [ebx+edi*4],eax ; w[i+j] = t mod 2^32
                        ; where t = u[i]*v[j]+w[i+j]+k
        dec edi             ; decrease i and (i+j) by 1
        dec ecx             ; M5. loop on i
      jge h2                ; note: edx = t/b
      h8:
      mov ebx,w
      mov [ebx+esi*4],edx   ; w[j] = k
      dec esi               ; M6. loop on j
    jge h1

    mov edi, edisav         ; restore edi and esi
    mov esi, esisav
  }
}

