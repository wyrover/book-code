class KERNINFO {
   public:
        // Constructors
        KERNINFO(CDC * dc);
        ~KERNINFO();

        // Data
        int count;
        LPKERNINGPAIR pairs;

        // Methods
        LPKERNINGPAIR findPair(UINT first, UINT second);
               };


