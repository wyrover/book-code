typedef struct {
        int count;              // count of number of polygons
        LPINT points;           // selection points in data array

        int fillmode;           // current fill mode

        int valuecount;         // number of points in data array
        LPPOINT data;           // vertices
               } PolyPolyData;
