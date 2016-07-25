====================== Listing 4 ======================
/* signed comparison : lint1 < lint2
   (assumes normalized operands)
   --------------------------------- */
int LargeInt::operator<(const LargeInt& lint) const {
   if (sign < lint.sign)
      return 1;
   if (sign > lint.sign)
      return 0;
// at this point, the signs are the same
   if (sign == 0)
      return 0;
   if ((sign == -1  &&  len > lint.len)  ||
       (sign ==  1  &&  len < lint.len))
      return 1;
   if ((sign == -1  &&  len < lint.len)  ||
       (sign ==  1  &&  len > lint.len))
      return 0;
// here the signs and the lengths are both the same
   int compare = memcmpInt(adr, lint.adr, len);
   if ((sign ==  1  &&  compare < 0)  ||
       (sign == -1  &&  compare > 0))
      return 1;
   return 0;
}

/* signed comparison : lint < 123
   ------------------------------- */
int LargeInt::operator<(int num) const {
   LargeInt test;
   test = num;
   return *this < test;
}
