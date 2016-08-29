class LegendInfo {
    public:
        LegendInfo(CBrush * b, CPen * p, UINT i) 
                                        { pen = p; brush = b; id = i; }

        CPen * pen;
        CBrush * brush;
        int id;
                 };

extern LegendInfo Legend_1;
extern LegendInfo Legend_2;
extern LegendInfo Legend_Free;
extern LegendInfo Legend_This;
extern LegendInfo Legend_Admin;
extern LegendInfo Legend_Overhead;
extern LegendInfo Legend_Filler;
extern LegendInfo Legend_1_Ovr;
extern LegendInfo Legend_2_Ovr;
