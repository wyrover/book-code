====================== Listing 3 ======================
void LargeInt::decToBin(const char* str) {
//
// code to determine value of strSign belongs here
//
   sign = 1; // sign adjusted below
   // find out how many blocks will be transferred
   numblocks = numchars = strlen(str);
   if (numblocks % PackFactor != 0)
      numblocks = numblocks / PackFactor + 1;
   else
      numblocks /= PackFactor;
   // carry out the intial coversion
   last = get9(buf, str, 1 + (numchars-1)%PackFactor);
   *this = (unsigned)strtoul(buf, 0, 10);
   // carry out the rest of the conversion
   while (--numblocks > 0) {
      last++;
      last = get9(buf, last, PackFactor);
      *this = (*this) * tenTo9;
      *this = (*this) + (unsigned)strtoul(buf, 0, 10);
   }
   sign = strSign;
}

/* Copy qty characters and store as string
   Return adr of last source character transferred
   ----------------------------------------------- */
const char* get9(char*dest, const char*src, int qty) {
   assert (*src != '\0');
   for (int i = 0; i < qty  &&  (dest[i]=src[i]); i++)
      { };
   dest[i] = '\0';
   return src + i - 1;
}
