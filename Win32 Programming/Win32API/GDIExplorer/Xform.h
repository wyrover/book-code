class CXform {
   public:
        XFORM matrix;
        struct {
                int M11;
                int M12;
                int M21;
                int M22;
               } kind;
        CXform();
        CXform(int M11kind, float M11, int M12kind, float M12,
                  int M21kind, float M21, int M22kind, float M22, 
                               float DX,               float DY);
        ~CXform();
        void SetMatrix(int M11kind, float M11, int M12kind, float M12,
                  int M21kind, float M21, int M22kind, float M22, 
                               float DX,               float DY);
        float convert(int kind, float value);
        float unconvert(int kind, float value);
        static inline float deg_to_rad(float deg) { return (float)(deg / (360.0 / (2 * 3.141592654))); }
        static inline float rad_to_deg(float rad) { return (float)(rad * (360.0 / (2 * 3.141592654))); }
        void GetMatrix(XFORM * mat);
        void SetIdentity();
            };

#define MATRIX_NONE    0
#define MATRIX_SIN     1
#define MATRIX_NEG_SIN 2
#define MATRIX_COS     3
#define MATRIX_NEG_COS 4
