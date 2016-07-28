// Listing 64-6. Two-Argument Form of the setf Function
void setf(fmtflags flags_to_set, fmtflags field)
{
   flags_ &= ~field;
   flags_ |= flags_to_set;
}
