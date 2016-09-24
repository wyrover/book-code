// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// Adjustments wrapper class

class Adjustments : public COleDispatchDriver
{
public:
    Adjustments() {}        // Calls COleDispatchDriver default constructor
    Adjustments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Adjustments(const Adjustments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    float GetItem(long Index);
    void SetItem(long Index, float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CalloutFormat wrapper class

class CalloutFormat : public COleDispatchDriver
{
public:
    CalloutFormat() {}      // Calls COleDispatchDriver default constructor
    CalloutFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CalloutFormat(const CalloutFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void AutomaticLength();
    void CustomDrop(float Drop);
    void CustomLength(float Length);
    void PresetDrop(long DropType);
    long GetAccent();
    void SetAccent(long nNewValue);
    long GetAngle();
    void SetAngle(long nNewValue);
    long GetAutoAttach();
    void SetAutoAttach(long nNewValue);
    long GetAutoLength();
    long GetBorder();
    void SetBorder(long nNewValue);
    float GetDrop();
    long GetDropType();
    float GetGap();
    void SetGap(float newValue);
    float GetLength();
    long GetType();
    void SetType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ColorFormat wrapper class

class ColorFormat : public COleDispatchDriver
{
public:
    ColorFormat() {}        // Calls COleDispatchDriver default constructor
    ColorFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ColorFormat(const ColorFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetRgb();
    void SetRgb(long nNewValue);
    long GetSchemeColor();
    void SetSchemeColor(long nNewValue);
    long GetType();
    float GetTintAndShade();
    void SetTintAndShade(float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// LineFormat wrapper class

class LineFormat : public COleDispatchDriver
{
public:
    LineFormat() {}     // Calls COleDispatchDriver default constructor
    LineFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    LineFormat(const LineFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBackColor();
    void SetBackColor(LPDISPATCH newValue);
    long GetBeginArrowheadLength();
    void SetBeginArrowheadLength(long nNewValue);
    long GetBeginArrowheadStyle();
    void SetBeginArrowheadStyle(long nNewValue);
    long GetBeginArrowheadWidth();
    void SetBeginArrowheadWidth(long nNewValue);
    long GetDashStyle();
    void SetDashStyle(long nNewValue);
    long GetEndArrowheadLength();
    void SetEndArrowheadLength(long nNewValue);
    long GetEndArrowheadStyle();
    void SetEndArrowheadStyle(long nNewValue);
    long GetEndArrowheadWidth();
    void SetEndArrowheadWidth(long nNewValue);
    LPDISPATCH GetForeColor();
    void SetForeColor(LPDISPATCH newValue);
    long GetPattern();
    void SetPattern(long nNewValue);
    long GetStyle();
    void SetStyle(long nNewValue);
    float GetTransparency();
    void SetTransparency(float newValue);
    long GetVisible();
    void SetVisible(long nNewValue);
    float GetWeight();
    void SetWeight(float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ShapeNode wrapper class

class ShapeNode : public COleDispatchDriver
{
public:
    ShapeNode() {}      // Calls COleDispatchDriver default constructor
    ShapeNode(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ShapeNode(const ShapeNode& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetEditingType();
    VARIANT GetPoints();
    long GetSegmentType();
};
/////////////////////////////////////////////////////////////////////////////
// ShapeNodes wrapper class

class ShapeNodes : public COleDispatchDriver
{
public:
    ShapeNodes() {}     // Calls COleDispatchDriver default constructor
    ShapeNodes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ShapeNodes(const ShapeNodes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    void Delete(long Index);
    void Insert(long Index, long SegmentType, long EditingType, float X1, float Y1, float X2, float Y2, float X3, float Y3);
    void SetEditingType(long Index, long EditingType);
    void SetPosition(long Index, float X1, float Y1);
    void SetSegmentType(long Index, long SegmentType);
};
/////////////////////////////////////////////////////////////////////////////
// PictureFormat wrapper class

class PictureFormat : public COleDispatchDriver
{
public:
    PictureFormat() {}      // Calls COleDispatchDriver default constructor
    PictureFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PictureFormat(const PictureFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void IncrementBrightness(float Increment);
    void IncrementContrast(float Increment);
    float GetBrightness();
    void SetBrightness(float newValue);
    long GetColorType();
    void SetColorType(long nNewValue);
    float GetContrast();
    void SetContrast(float newValue);
    float GetCropBottom();
    void SetCropBottom(float newValue);
    float GetCropLeft();
    void SetCropLeft(float newValue);
    float GetCropRight();
    void SetCropRight(float newValue);
    float GetCropTop();
    void SetCropTop(float newValue);
    long GetTransparencyColor();
    void SetTransparencyColor(long nNewValue);
    long GetTransparentBackground();
    void SetTransparentBackground(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ShadowFormat wrapper class

class ShadowFormat : public COleDispatchDriver
{
public:
    ShadowFormat() {}       // Calls COleDispatchDriver default constructor
    ShadowFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ShadowFormat(const ShadowFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void IncrementOffsetX(float Increment);
    void IncrementOffsetY(float Increment);
    LPDISPATCH GetForeColor();
    void SetForeColor(LPDISPATCH newValue);
    long GetObscured();
    void SetObscured(long nNewValue);
    float GetOffsetX();
    void SetOffsetX(float newValue);
    float GetOffsetY();
    void SetOffsetY(float newValue);
    float GetTransparency();
    void SetTransparency(float newValue);
    long GetType();
    void SetType(long nNewValue);
    long GetVisible();
    void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// TextEffectFormat wrapper class

class TextEffectFormat : public COleDispatchDriver
{
public:
    TextEffectFormat() {}       // Calls COleDispatchDriver default constructor
    TextEffectFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    TextEffectFormat(const TextEffectFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void ToggleVerticalText();
    long GetAlignment();
    void SetAlignment(long nNewValue);
    long GetFontBold();
    void SetFontBold(long nNewValue);
    long GetFontItalic();
    void SetFontItalic(long nNewValue);
    CString GetFontName();
    void SetFontName(LPCTSTR lpszNewValue);
    float GetFontSize();
    void SetFontSize(float newValue);
    long GetKernedPairs();
    void SetKernedPairs(long nNewValue);
    long GetNormalizedHeight();
    void SetNormalizedHeight(long nNewValue);
    long GetPresetShape();
    void SetPresetShape(long nNewValue);
    long GetPresetTextEffect();
    void SetPresetTextEffect(long nNewValue);
    long GetRotatedChars();
    void SetRotatedChars(long nNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    float GetTracking();
    void SetTracking(float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ThreeDFormat wrapper class

class ThreeDFormat : public COleDispatchDriver
{
public:
    ThreeDFormat() {}       // Calls COleDispatchDriver default constructor
    ThreeDFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ThreeDFormat(const ThreeDFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void IncrementRotationX(float Increment);
    void IncrementRotationY(float Increment);
    void ResetRotation();
    void SetThreeDFormat(long PresetThreeDFormat);
    void SetExtrusionDirection(long PresetExtrusionDirection);
    float GetDepth();
    void SetDepth(float newValue);
    LPDISPATCH GetExtrusionColor();
    long GetExtrusionColorType();
    void SetExtrusionColorType(long nNewValue);
    long GetPerspective();
    void SetPerspective(long nNewValue);
    long GetPresetExtrusionDirection();
    long GetPresetLightingDirection();
    void SetPresetLightingDirection(long nNewValue);
    long GetPresetLightingSoftness();
    void SetPresetLightingSoftness(long nNewValue);
    long GetPresetMaterial();
    void SetPresetMaterial(long nNewValue);
    long GetPresetThreeDFormat();
    float GetRotationX();
    void SetRotationX(float newValue);
    float GetRotationY();
    void SetRotationY(float newValue);
    long GetVisible();
    void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// FillFormat wrapper class

class FillFormat : public COleDispatchDriver
{
public:
    FillFormat() {}     // Calls COleDispatchDriver default constructor
    FillFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    FillFormat(const FillFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void OneColorGradient(long Style, long Variant, float Degree);
    void Patterned(long Pattern);
    void PresetGradient(long Style, long Variant, long PresetGradientType);
    void PresetTextured(long PresetTexture);
    void Solid();
    void TwoColorGradient(long Style, long Variant);
    void UserPicture(LPCTSTR PictureFile);
    void UserTextured(LPCTSTR TextureFile);
    LPDISPATCH GetBackColor();
    void SetBackColor(LPDISPATCH newValue);
    LPDISPATCH GetForeColor();
    void SetForeColor(LPDISPATCH newValue);
    long GetGradientColorType();
    float GetGradientDegree();
    long GetGradientStyle();
    long GetGradientVariant();
    long GetPattern();
    long GetPresetGradientType();
    long GetPresetTexture();
    CString GetTextureName();
    long GetTextureType();
    float GetTransparency();
    void SetTransparency(float newValue);
    long GetType();
    long GetVisible();
    void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DiagramNodes wrapper class

class DiagramNodes : public COleDispatchDriver
{
public:
    DiagramNodes() {}       // Calls COleDispatchDriver default constructor
    DiagramNodes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DiagramNodes(const DiagramNodes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Item(const VARIANT& Index);
    void SelectAll();
    LPDISPATCH GetParent();
    long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// DiagramNodeChildren wrapper class

class DiagramNodeChildren : public COleDispatchDriver
{
public:
    DiagramNodeChildren() {}        // Calls COleDispatchDriver default constructor
    DiagramNodeChildren(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DiagramNodeChildren(const DiagramNodeChildren& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH AddNode(const VARIANT& Index, long nodeType);
    void SelectAll();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetFirstChild();
    LPDISPATCH GetLastChild();
};
/////////////////////////////////////////////////////////////////////////////
// DiagramNode wrapper class

class DiagramNode : public COleDispatchDriver
{
public:
    DiagramNode() {}        // Calls COleDispatchDriver default constructor
    DiagramNode(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DiagramNode(const DiagramNode& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH AddNode(long pos, long nodeType);
    void Delete();
    void MoveNode(LPDISPATCH pTargetNode, long pos);
    void ReplaceNode(LPDISPATCH pTargetNode);
    void SwapNode(LPDISPATCH pTargetNode, BOOL swapChildren);
    LPDISPATCH CloneNode(BOOL copyChildren, LPDISPATCH pTargetNode, long pos);
    void TransferChildren(LPDISPATCH pReceivingNode);
    LPDISPATCH NextNode();
    LPDISPATCH PrevNode();
    LPDISPATCH GetParent();
    LPDISPATCH GetChildren();
    LPDISPATCH GetShape();
    LPDISPATCH GetRoot();
    LPDISPATCH GetDiagram();
    long GetLayout();
    void SetLayout(long nNewValue);
    LPDISPATCH GetTextShape();
};
/////////////////////////////////////////////////////////////////////////////
// IRTDUpdateEvent wrapper class

class IRTDUpdateEvent : public COleDispatchDriver
{
public:
    IRTDUpdateEvent() {}        // Calls COleDispatchDriver default constructor
    IRTDUpdateEvent(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    IRTDUpdateEvent(const IRTDUpdateEvent& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void UpdateNotify();
    long GetHeartbeatInterval();
    void SetHeartbeatInterval(long nNewValue);
    void Disconnect();
};
/////////////////////////////////////////////////////////////////////////////
// IRtdServer wrapper class

class IRtdServer : public COleDispatchDriver
{
public:
    IRtdServer() {}     // Calls COleDispatchDriver default constructor
    IRtdServer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    IRtdServer(const IRtdServer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long ServerStart(LPDISPATCH CallbackObject);
    // method 'ConnectData' not emitted because of invalid return type or parameter type
    // method 'RefreshData' not emitted because of invalid return type or parameter type
    void DisconnectData(long TopicID);
    long Heartbeat();
    void ServerTerminate();
};
/////////////////////////////////////////////////////////////////////////////
// _Application wrapper class

class _Application : public COleDispatchDriver
{
public:
    _Application() {}       // Calls COleDispatchDriver default constructor
    _Application(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Application(const _Application& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetActiveCell();
    LPDISPATCH GetActiveChart();
    CString GetActivePrinter();
    void SetActivePrinter(LPCTSTR lpszNewValue);
    LPDISPATCH GetActiveSheet();
    LPDISPATCH GetActiveWindow();
    LPDISPATCH GetActiveWorkbook();
    LPDISPATCH GetAddIns();
    LPDISPATCH GetAssistant();
    void Calculate();
    LPDISPATCH GetCells();
    LPDISPATCH GetCharts();
    LPDISPATCH GetColumns();
    LPDISPATCH GetCommandBars();
    long GetDDEAppReturnCode();
    void DDEExecute(long Channel, LPCTSTR String);
    long DDEInitiate(LPCTSTR App, LPCTSTR Topic);
    void DDEPoke(long Channel, const VARIANT& Item, const VARIANT& Data);
    VARIANT DDERequest(long Channel, LPCTSTR Item);
    void DDETerminate(long Channel);
    VARIANT Evaluate(const VARIANT& Name);
    VARIANT _Evaluate(const VARIANT& Name);
    VARIANT ExecuteExcel4Macro(LPCTSTR String);
    LPDISPATCH Intersect(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                         const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                         const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    LPDISPATCH GetNames();
    LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
    LPDISPATCH GetRows();
    VARIANT Run(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9,
                const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19,
                const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29,
                const VARIANT& Arg30);
    VARIANT _Run2(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9,
                  const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19,
                  const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29,
                  const VARIANT& Arg30);
    LPDISPATCH GetSelection();
    void SendKeys(const VARIANT& Keys, const VARIANT& Wait);
    LPDISPATCH GetSheets();
    LPDISPATCH GetThisWorkbook();
    LPDISPATCH Union(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
                     const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
                     const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    LPDISPATCH GetWindows();
    LPDISPATCH GetWorkbooks();
    LPDISPATCH GetWorksheetFunction();
    LPDISPATCH GetWorksheets();
    LPDISPATCH GetExcel4IntlMacroSheets();
    LPDISPATCH GetExcel4MacroSheets();
    void ActivateMicrosoftApp(long Index);
    void AddChartAutoFormat(const VARIANT& Chart, LPCTSTR Name, const VARIANT& Description);
    void AddCustomList(const VARIANT& ListArray, const VARIANT& ByRow);
    BOOL GetAlertBeforeOverwriting();
    void SetAlertBeforeOverwriting(BOOL bNewValue);
    CString GetAltStartupPath();
    void SetAltStartupPath(LPCTSTR lpszNewValue);
    BOOL GetAskToUpdateLinks();
    void SetAskToUpdateLinks(BOOL bNewValue);
    BOOL GetEnableAnimations();
    void SetEnableAnimations(BOOL bNewValue);
    LPDISPATCH GetAutoCorrect();
    long GetBuild();
    BOOL GetCalculateBeforeSave();
    void SetCalculateBeforeSave(BOOL bNewValue);
    long GetCalculation();
    void SetCalculation(long nNewValue);
    VARIANT GetCaller(const VARIANT& Index);
    BOOL GetCanPlaySounds();
    BOOL GetCanRecordSounds();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    BOOL GetCellDragAndDrop();
    void SetCellDragAndDrop(BOOL bNewValue);
    double CentimetersToPoints(double Centimeters);
    BOOL CheckSpelling(LPCTSTR Word, const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase);
    VARIANT GetClipboardFormats(const VARIANT& Index);
    BOOL GetDisplayClipboardWindow();
    void SetDisplayClipboardWindow(BOOL bNewValue);
    long GetCommandUnderlines();
    void SetCommandUnderlines(long nNewValue);
    BOOL GetConstrainNumeric();
    void SetConstrainNumeric(BOOL bNewValue);
    VARIANT ConvertFormula(const VARIANT& Formula, long FromReferenceStyle, const VARIANT& ToReferenceStyle, const VARIANT& ToAbsolute, const VARIANT& RelativeTo);
    BOOL GetCopyObjectsWithCells();
    void SetCopyObjectsWithCells(BOOL bNewValue);
    long GetCursor();
    void SetCursor(long nNewValue);
    long GetCustomListCount();
    long GetCutCopyMode();
    void SetCutCopyMode(long nNewValue);
    long GetDataEntryMode();
    void SetDataEntryMode(long nNewValue);
    CString Get_Default();
    CString GetDefaultFilePath();
    void SetDefaultFilePath(LPCTSTR lpszNewValue);
    void DeleteChartAutoFormat(LPCTSTR Name);
    void DeleteCustomList(long ListNum);
    LPDISPATCH GetDialogs();
    BOOL GetDisplayAlerts();
    void SetDisplayAlerts(BOOL bNewValue);
    BOOL GetDisplayFormulaBar();
    void SetDisplayFormulaBar(BOOL bNewValue);
    BOOL GetDisplayFullScreen();
    void SetDisplayFullScreen(BOOL bNewValue);
    BOOL GetDisplayNoteIndicator();
    void SetDisplayNoteIndicator(BOOL bNewValue);
    long GetDisplayCommentIndicator();
    void SetDisplayCommentIndicator(long nNewValue);
    BOOL GetDisplayExcel4Menus();
    void SetDisplayExcel4Menus(BOOL bNewValue);
    BOOL GetDisplayRecentFiles();
    void SetDisplayRecentFiles(BOOL bNewValue);
    BOOL GetDisplayScrollBars();
    void SetDisplayScrollBars(BOOL bNewValue);
    BOOL GetDisplayStatusBar();
    void SetDisplayStatusBar(BOOL bNewValue);
    void DoubleClick();
    BOOL GetEditDirectlyInCell();
    void SetEditDirectlyInCell(BOOL bNewValue);
    BOOL GetEnableAutoComplete();
    void SetEnableAutoComplete(BOOL bNewValue);
    long GetEnableCancelKey();
    void SetEnableCancelKey(long nNewValue);
    BOOL GetEnableSound();
    void SetEnableSound(BOOL bNewValue);
    VARIANT GetFileConverters(const VARIANT& Index1, const VARIANT& Index2);
    LPDISPATCH GetFileSearch();
    LPDISPATCH GetFileFind();
    BOOL GetFixedDecimal();
    void SetFixedDecimal(BOOL bNewValue);
    long GetFixedDecimalPlaces();
    void SetFixedDecimalPlaces(long nNewValue);
    VARIANT GetCustomListContents(long ListNum);
    long GetCustomListNum(const VARIANT& ListArray);
    VARIANT GetOpenFilename(const VARIANT& FileFilter, const VARIANT& FilterIndex, const VARIANT& Title, const VARIANT& ButtonText, const VARIANT& MultiSelect);
    VARIANT GetSaveAsFilename(const VARIANT& InitialFilename, const VARIANT& FileFilter, const VARIANT& FilterIndex, const VARIANT& Title, const VARIANT& ButtonText);
    void Goto(const VARIANT& Reference, const VARIANT& Scroll);
    double GetHeight();
    void SetHeight(double newValue);
    void Help(const VARIANT& HelpFile, const VARIANT& HelpContextID);
    BOOL GetIgnoreRemoteRequests();
    void SetIgnoreRemoteRequests(BOOL bNewValue);
    double InchesToPoints(double Inches);
    VARIANT InputBox(LPCTSTR Prompt, const VARIANT& Title, const VARIANT& Default, const VARIANT& Left, const VARIANT& Top, const VARIANT& HelpFile, const VARIANT& HelpContextID, const VARIANT& Type);
    BOOL GetInteractive();
    void SetInteractive(BOOL bNewValue);
    VARIANT GetInternational(const VARIANT& Index);
    BOOL GetIteration();
    void SetIteration(BOOL bNewValue);
    double GetLeft();
    void SetLeft(double newValue);
    CString GetLibraryPath();
    void MacroOptions(const VARIANT& Macro, const VARIANT& Description, const VARIANT& HasMenu, const VARIANT& MenuText, const VARIANT& HasShortcutKey, const VARIANT& ShortcutKey, const VARIANT& Category, const VARIANT& StatusBar,
                      const VARIANT& HelpContextID, const VARIANT& HelpFile);
    void MailLogoff();
    void MailLogon(const VARIANT& Name, const VARIANT& Password, const VARIANT& DownloadNewMail);
    VARIANT GetMailSession();
    long GetMailSystem();
    BOOL GetMathCoprocessorAvailable();
    double GetMaxChange();
    void SetMaxChange(double newValue);
    long GetMaxIterations();
    void SetMaxIterations(long nNewValue);
    BOOL GetMouseAvailable();
    BOOL GetMoveAfterReturn();
    void SetMoveAfterReturn(BOOL bNewValue);
    long GetMoveAfterReturnDirection();
    void SetMoveAfterReturnDirection(long nNewValue);
    LPDISPATCH GetRecentFiles();
    CString GetName();
    LPDISPATCH NextLetter();
    CString GetNetworkTemplatesPath();
    LPDISPATCH GetODBCErrors();
    long GetODBCTimeout();
    void SetODBCTimeout(long nNewValue);
    void OnKey(LPCTSTR Key, const VARIANT& Procedure);
    void OnRepeat(LPCTSTR Text, LPCTSTR Procedure);
    void OnTime(const VARIANT& EarliestTime, LPCTSTR Procedure, const VARIANT& LatestTime, const VARIANT& Schedule);
    void OnUndo(LPCTSTR Text, LPCTSTR Procedure);
    CString GetOnWindow();
    void SetOnWindow(LPCTSTR lpszNewValue);
    CString GetOperatingSystem();
    CString GetOrganizationName();
    CString GetPath();
    CString GetPathSeparator();
    VARIANT GetPreviousSelections(const VARIANT& Index);
    BOOL GetPivotTableSelection();
    void SetPivotTableSelection(BOOL bNewValue);
    BOOL GetPromptForSummaryInfo();
    void SetPromptForSummaryInfo(BOOL bNewValue);
    void Quit();
    void RecordMacro(const VARIANT& BasicCode, const VARIANT& XlmCode);
    BOOL GetRecordRelative();
    long GetReferenceStyle();
    void SetReferenceStyle(long nNewValue);
    VARIANT GetRegisteredFunctions(const VARIANT& Index1, const VARIANT& Index2);
    BOOL RegisterXLL(LPCTSTR Filename);
    void Repeat();
    BOOL GetRollZoom();
    void SetRollZoom(BOOL bNewValue);
    void SaveWorkspace(const VARIANT& Filename);
    BOOL GetScreenUpdating();
    void SetScreenUpdating(BOOL bNewValue);
    void SetDefaultChart(const VARIANT& FormatName, const VARIANT& Gallery);
    long GetSheetsInNewWorkbook();
    void SetSheetsInNewWorkbook(long nNewValue);
    BOOL GetShowChartTipNames();
    void SetShowChartTipNames(BOOL bNewValue);
    BOOL GetShowChartTipValues();
    void SetShowChartTipValues(BOOL bNewValue);
    CString GetStandardFont();
    void SetStandardFont(LPCTSTR lpszNewValue);
    double GetStandardFontSize();
    void SetStandardFontSize(double newValue);
    CString GetStartupPath();
    VARIANT GetStatusBar();
    void SetStatusBar(const VARIANT& newValue);
    CString GetTemplatesPath();
    BOOL GetShowToolTips();
    void SetShowToolTips(BOOL bNewValue);
    double GetTop();
    void SetTop(double newValue);
    long GetDefaultSaveFormat();
    void SetDefaultSaveFormat(long nNewValue);
    CString GetTransitionMenuKey();
    void SetTransitionMenuKey(LPCTSTR lpszNewValue);
    long GetTransitionMenuKeyAction();
    void SetTransitionMenuKeyAction(long nNewValue);
    BOOL GetTransitionNavigKeys();
    void SetTransitionNavigKeys(BOOL bNewValue);
    void Undo();
    double GetUsableHeight();
    double GetUsableWidth();
    BOOL GetUserControl();
    void SetUserControl(BOOL bNewValue);
    CString GetUserName_();
    void SetUserName(LPCTSTR lpszNewValue);
    CString GetValue();
    LPDISPATCH GetVbe();
    CString GetVersion();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    void Volatile(const VARIANT& Volatile);
    double GetWidth();
    void SetWidth(double newValue);
    BOOL GetWindowsForPens();
    long GetWindowState();
    void SetWindowState(long nNewValue);
    long GetDefaultSheetDirection();
    void SetDefaultSheetDirection(long nNewValue);
    long GetCursorMovement();
    void SetCursorMovement(long nNewValue);
    BOOL GetControlCharacters();
    void SetControlCharacters(BOOL bNewValue);
    BOOL GetEnableEvents();
    void SetEnableEvents(BOOL bNewValue);
    BOOL Wait(const VARIANT& Time);
    BOOL GetExtendList();
    void SetExtendList(BOOL bNewValue);
    LPDISPATCH GetOLEDBErrors();
    CString GetPhonetic(const VARIANT& Text);
    LPDISPATCH GetCOMAddIns();
    LPDISPATCH GetDefaultWebOptions();
    CString GetProductCode();
    CString GetUserLibraryPath();
    BOOL GetAutoPercentEntry();
    void SetAutoPercentEntry(BOOL bNewValue);
    LPDISPATCH GetLanguageSettings();
    LPDISPATCH GetAnswerWizard();
    void CalculateFull();
    BOOL FindFile();
    long GetCalculationVersion();
    BOOL GetShowWindowsInTaskbar();
    void SetShowWindowsInTaskbar(BOOL bNewValue);
    long GetFeatureInstall();
    void SetFeatureInstall(long nNewValue);
    BOOL GetReady();
    LPDISPATCH GetFindFormat();
    void SetRefFindFormat(LPDISPATCH newValue);
    LPDISPATCH GetReplaceFormat();
    void SetRefReplaceFormat(LPDISPATCH newValue);
    LPDISPATCH GetUsedObjects();
    long GetCalculationState();
    long GetCalculationInterruptKey();
    void SetCalculationInterruptKey(long nNewValue);
    LPDISPATCH GetWatches();
    BOOL GetDisplayFunctionToolTips();
    void SetDisplayFunctionToolTips(BOOL bNewValue);
    long GetAutomationSecurity();
    void SetAutomationSecurity(long nNewValue);
    LPDISPATCH GetFileDialog(long fileDialogType);
    void CalculateFullRebuild();
    BOOL GetDisplayPasteOptions();
    void SetDisplayPasteOptions(BOOL bNewValue);
    BOOL GetDisplayInsertOptions();
    void SetDisplayInsertOptions(BOOL bNewValue);
    BOOL GetGenerateGetPivotData();
    void SetGenerateGetPivotData(BOOL bNewValue);
    LPDISPATCH GetAutoRecover();
    long GetHwnd();
    long GetHinstance();
    void CheckAbort(const VARIANT& KeepAbort);
    LPDISPATCH GetErrorCheckingOptions();
    BOOL GetAutoFormatAsYouTypeReplaceHyperlinks();
    void SetAutoFormatAsYouTypeReplaceHyperlinks(BOOL bNewValue);
    LPDISPATCH GetSmartTagRecognizers();
    LPDISPATCH GetNewWorkbook();
    LPDISPATCH GetSpellingOptions();
    LPDISPATCH GetSpeech();
    BOOL GetMapPaperSize();
    void SetMapPaperSize(BOOL bNewValue);
    BOOL GetShowStartupDialog();
    void SetShowStartupDialog(BOOL bNewValue);
    CString GetDecimalSeparator();
    void SetDecimalSeparator(LPCTSTR lpszNewValue);
    CString GetThousandsSeparator();
    void SetThousandsSeparator(LPCTSTR lpszNewValue);
    BOOL GetUseSystemSeparators();
    void SetUseSystemSeparators(BOOL bNewValue);
    LPDISPATCH GetThisCell();
    LPDISPATCH GetRtd();
    BOOL GetDisplayDocumentActionTaskPane();
    void SetDisplayDocumentActionTaskPane(BOOL bNewValue);
    void DisplayXMLSourcePane(const VARIANT& XmlMap);
    BOOL GetArbitraryXMLSupportAvailable();
};
/////////////////////////////////////////////////////////////////////////////
// _Chart wrapper class

class _Chart : public COleDispatchDriver
{
public:
    _Chart() {}     // Calls COleDispatchDriver default constructor
    _Chart(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Chart(const _Chart& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Activate();
    void Copy(const VARIANT& Before, const VARIANT& After);
    void Delete();
    CString GetCodeName();
    CString Get_CodeName();
    void Set_CodeName(LPCTSTR lpszNewValue);
    long GetIndex();
    void Move(const VARIANT& Before, const VARIANT& After);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetNext();
    LPDISPATCH GetPageSetup();
    LPDISPATCH GetPrevious();
    void PrintPreview(const VARIANT& EnableChanges);
    BOOL GetProtectContents();
    BOOL GetProtectDrawingObjects();
    BOOL GetProtectionMode();
    void Select(const VARIANT& Replace);
    void Unprotect(const VARIANT& Password);
    long GetVisible();
    void SetVisible(long nNewValue);
    LPDISPATCH GetShapes();
    LPDISPATCH GetArea3DGroup();
    LPDISPATCH AreaGroups(const VARIANT& Index);
    BOOL GetAutoScaling();
    void SetAutoScaling(BOOL bNewValue);
    LPDISPATCH Axes(const VARIANT& Type, long AxisGroup);
    void SetBackgroundPicture(LPCTSTR Filename);
    LPDISPATCH GetBar3DGroup();
    LPDISPATCH BarGroups(const VARIANT& Index);
    LPDISPATCH GetChartArea();
    LPDISPATCH ChartGroups(const VARIANT& Index);
    LPDISPATCH ChartObjects(const VARIANT& Index);
    LPDISPATCH GetChartTitle();
    void ChartWizard(const VARIANT& Source, const VARIANT& Gallery, const VARIANT& Format, const VARIANT& PlotBy, const VARIANT& CategoryLabels, const VARIANT& SeriesLabels, const VARIANT& HasLegend, const VARIANT& Title,
                     const VARIANT& CategoryTitle, const VARIANT& ValueTitle, const VARIANT& ExtraTitle);
    void CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetColumn3DGroup();
    LPDISPATCH ColumnGroups(const VARIANT& Index);
    void CopyPicture(long Appearance, long Format, long Size);
    LPDISPATCH GetCorners();
    void CreatePublisher(const VARIANT& Edition, long Appearance, long Size, const VARIANT& ContainsPICT, const VARIANT& ContainsBIFF, const VARIANT& ContainsRTF, const VARIANT& ContainsVALU);
    LPDISPATCH GetDataTable();
    long GetDepthPercent();
    void SetDepthPercent(long nNewValue);
    void Deselect();
    long GetDisplayBlanksAs();
    void SetDisplayBlanksAs(long nNewValue);
    LPDISPATCH DoughnutGroups(const VARIANT& Index);
    long GetElevation();
    void SetElevation(long nNewValue);
    VARIANT Evaluate(const VARIANT& Name);
    VARIANT _Evaluate(const VARIANT& Name);
    LPDISPATCH GetFloor();
    long GetGapDepth();
    void SetGapDepth(long nNewValue);
    VARIANT GetHasAxis(const VARIANT& Index1, const VARIANT& Index2);
    void SetHasAxis(const VARIANT& Index1, const VARIANT& Index2, const VARIANT& newValue);
    BOOL GetHasDataTable();
    void SetHasDataTable(BOOL bNewValue);
    BOOL GetHasLegend();
    void SetHasLegend(BOOL bNewValue);
    BOOL GetHasTitle();
    void SetHasTitle(BOOL bNewValue);
    long GetHeightPercent();
    void SetHeightPercent(long nNewValue);
    LPDISPATCH GetHyperlinks();
    LPDISPATCH GetLegend();
    LPDISPATCH GetLine3DGroup();
    LPDISPATCH LineGroups(const VARIANT& Index);
    LPDISPATCH Location(long Where, const VARIANT& Name);
    LPDISPATCH OLEObjects(const VARIANT& Index);
    void Paste(const VARIANT& Type);
    long GetPerspective();
    void SetPerspective(long nNewValue);
    LPDISPATCH GetPie3DGroup();
    LPDISPATCH PieGroups(const VARIANT& Index);
    LPDISPATCH GetPlotArea();
    BOOL GetPlotVisibleOnly();
    void SetPlotVisibleOnly(BOOL bNewValue);
    LPDISPATCH RadarGroups(const VARIANT& Index);
    VARIANT GetRightAngleAxes();
    void SetRightAngleAxes(const VARIANT& newValue);
    VARIANT GetRotation();
    void SetRotation(const VARIANT& newValue);
    LPDISPATCH SeriesCollection(const VARIANT& Index);
    BOOL GetSizeWithWindow();
    void SetSizeWithWindow(BOOL bNewValue);
    BOOL GetShowWindow();
    void SetShowWindow(BOOL bNewValue);
    LPDISPATCH GetSurfaceGroup();
    long GetChartType();
    void SetChartType(long nNewValue);
    void ApplyCustomType(long ChartType, const VARIANT& TypeName);
    LPDISPATCH GetWalls();
    BOOL GetWallsAndGridlines2D();
    void SetWallsAndGridlines2D(BOOL bNewValue);
    LPDISPATCH XYGroups(const VARIANT& Index);
    long GetBarShape();
    void SetBarShape(long nNewValue);
    long GetPlotBy();
    void SetPlotBy(long nNewValue);
    BOOL GetProtectFormatting();
    void SetProtectFormatting(BOOL bNewValue);
    BOOL GetProtectData();
    void SetProtectData(BOOL bNewValue);
    BOOL GetProtectGoalSeek();
    void SetProtectGoalSeek(BOOL bNewValue);
    BOOL GetProtectSelection();
    void SetProtectSelection(BOOL bNewValue);
    void GetChartElement(long x, long y, long* ElementID, long* Arg1, long* Arg2);
    void SetSourceData(LPDISPATCH Source, const VARIANT& PlotBy);
    BOOL Export(LPCTSTR Filename, const VARIANT& FilterName, const VARIANT& Interactive);
    void Refresh();
    LPDISPATCH GetPivotLayout();
    BOOL GetHasPivotFields();
    void SetHasPivotFields(BOOL bNewValue);
    LPDISPATCH GetScripts();
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
    LPDISPATCH GetTab();
    LPDISPATCH GetMailEnvelope();
    void ApplyDataLabels(long Type, const VARIANT& LegendKey, const VARIANT& AutoText, const VARIANT& HasLeaderLines, const VARIANT& ShowSeriesName, const VARIANT& ShowCategoryName, const VARIANT& ShowValue, const VARIANT& ShowPercentage,
                         const VARIANT& ShowBubbleSize, const VARIANT& Separator);
    void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage,
                const VARIANT& TextVisualLayout, const VARIANT& Local);
    void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly);
};
/////////////////////////////////////////////////////////////////////////////
// Sheets wrapper class

class Sheets : public COleDispatchDriver
{
public:
    Sheets() {}     // Calls COleDispatchDriver default constructor
    Sheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Sheets(const Sheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type);
    void Copy(const VARIANT& Before, const VARIANT& After);
    long GetCount();
    void Delete();
    void FillAcrossSheets(LPDISPATCH Range, long Type);
    LPDISPATCH GetItem(const VARIANT& Index);
    void Move(const VARIANT& Before, const VARIANT& After);
    LPUNKNOWN Get_NewEnum();
    void PrintPreview(const VARIANT& EnableChanges);
    void Select(const VARIANT& Replace);
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    VARIANT GetVisible();
    void SetVisible(const VARIANT& newValue);
    LPDISPATCH Get_Default(const VARIANT& Index);
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// _Worksheet wrapper class

class _Worksheet : public COleDispatchDriver
{
public:
    _Worksheet() {}     // Calls COleDispatchDriver default constructor
    _Worksheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Worksheet(const _Worksheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Activate();
    void Copy(const VARIANT& Before, const VARIANT& After);
    void Delete();
    CString GetCodeName();
    CString Get_CodeName();
    void Set_CodeName(LPCTSTR lpszNewValue);
    long GetIndex();
    void Move(const VARIANT& Before, const VARIANT& After);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetNext();
    LPDISPATCH GetPageSetup();
    LPDISPATCH GetPrevious();
    void PrintPreview(const VARIANT& EnableChanges);
    BOOL GetProtectContents();
    BOOL GetProtectDrawingObjects();
    BOOL GetProtectionMode();
    BOOL GetProtectScenarios();
    void Select(const VARIANT& Replace);
    void Unprotect(const VARIANT& Password);
    long GetVisible();
    void SetVisible(long nNewValue);
    LPDISPATCH GetShapes();
    BOOL GetTransitionExpEval();
    void SetTransitionExpEval(BOOL bNewValue);
    BOOL GetAutoFilterMode();
    void SetAutoFilterMode(BOOL bNewValue);
    void SetBackgroundPicture(LPCTSTR Filename);
    void Calculate();
    BOOL GetEnableCalculation();
    void SetEnableCalculation(BOOL bNewValue);
    LPDISPATCH GetCells();
    LPDISPATCH ChartObjects(const VARIANT& Index);
    void CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetCircularReference();
    void ClearArrows();
    LPDISPATCH GetColumns();
    long GetConsolidationFunction();
    VARIANT GetConsolidationOptions();
    VARIANT GetConsolidationSources();
    BOOL GetEnableAutoFilter();
    void SetEnableAutoFilter(BOOL bNewValue);
    long GetEnableSelection();
    void SetEnableSelection(long nNewValue);
    BOOL GetEnableOutlining();
    void SetEnableOutlining(BOOL bNewValue);
    BOOL GetEnablePivotTable();
    void SetEnablePivotTable(BOOL bNewValue);
    VARIANT Evaluate(const VARIANT& Name);
    VARIANT _Evaluate(const VARIANT& Name);
    BOOL GetFilterMode();
    void ResetAllPageBreaks();
    LPDISPATCH GetNames();
    LPDISPATCH OLEObjects(const VARIANT& Index);
    LPDISPATCH GetOutline();
    void Paste(const VARIANT& Destination, const VARIANT& Link);
    LPDISPATCH PivotTables(const VARIANT& Index);
    LPDISPATCH PivotTableWizard(const VARIANT& SourceType, const VARIANT& SourceData, const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& RowGrand, const VARIANT& ColumnGrand, const VARIANT& SaveData,
                                const VARIANT& HasAutoFormat, const VARIANT& AutoPage, const VARIANT& Reserved, const VARIANT& BackgroundQuery, const VARIANT& OptimizeCache, const VARIANT& PageFieldOrder, const VARIANT& PageFieldWrapCount, const VARIANT& ReadData,
                                const VARIANT& Connection);
    LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
    LPDISPATCH GetRows();
    LPDISPATCH Scenarios(const VARIANT& Index);
    CString GetScrollArea();
    void SetScrollArea(LPCTSTR lpszNewValue);
    void ShowAllData();
    void ShowDataForm();
    double GetStandardHeight();
    double GetStandardWidth();
    void SetStandardWidth(double newValue);
    BOOL GetTransitionFormEntry();
    void SetTransitionFormEntry(BOOL bNewValue);
    long GetType();
    LPDISPATCH GetUsedRange();
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    LPDISPATCH GetQueryTables();
    BOOL GetDisplayPageBreaks();
    void SetDisplayPageBreaks(BOOL bNewValue);
    LPDISPATCH GetComments();
    LPDISPATCH GetHyperlinks();
    void ClearCircles();
    void CircleInvalid();
    LPDISPATCH GetAutoFilter();
    BOOL GetDisplayRightToLeft();
    void SetDisplayRightToLeft(BOOL bNewValue);
    LPDISPATCH GetScripts();
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
    LPDISPATCH GetTab();
    LPDISPATCH GetMailEnvelope();
    void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage,
                const VARIANT& TextVisualLayout, const VARIANT& Local);
    LPDISPATCH GetCustomProperties();
    LPDISPATCH GetSmartTags();
    LPDISPATCH GetProtection();
    void PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& NoHTMLFormatting);
    void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly, const VARIANT& AllowFormattingCells, const VARIANT& AllowFormattingColumns,
                 const VARIANT& AllowFormattingRows, const VARIANT& AllowInsertingColumns, const VARIANT& AllowInsertingRows, const VARIANT& AllowInsertingHyperlinks, const VARIANT& AllowDeletingColumns, const VARIANT& AllowDeletingRows,
                 const VARIANT& AllowSorting, const VARIANT& AllowFiltering, const VARIANT& AllowUsingPivotTables);
    LPDISPATCH GetListObjects();
    LPDISPATCH XmlDataQuery(LPCTSTR XPath, const VARIANT& SelectionNamespaces, const VARIANT& Map);
    LPDISPATCH XmlMapQuery(LPCTSTR XPath, const VARIANT& SelectionNamespaces, const VARIANT& Map);
};
/////////////////////////////////////////////////////////////////////////////
// _Global wrapper class

class _Global : public COleDispatchDriver
{
public:
    _Global() {}        // Calls COleDispatchDriver default constructor
    _Global(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Global(const _Global& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetActiveCell();
    LPDISPATCH GetActiveChart();
    CString GetActivePrinter();
    void SetActivePrinter(LPCTSTR lpszNewValue);
    LPDISPATCH GetActiveSheet();
    LPDISPATCH GetActiveWindow();
    LPDISPATCH GetActiveWorkbook();
    LPDISPATCH GetAddIns();
    LPDISPATCH GetAssistant();
    void Calculate();
    LPDISPATCH GetCells();
    LPDISPATCH GetCharts();
    LPDISPATCH GetColumns();
    LPDISPATCH GetCommandBars();
    long GetDDEAppReturnCode();
    void DDEExecute(long Channel, LPCTSTR String);
    long DDEInitiate(LPCTSTR App, LPCTSTR Topic);
    void DDEPoke(long Channel, const VARIANT& Item, const VARIANT& Data);
    VARIANT DDERequest(long Channel, LPCTSTR Item);
    void DDETerminate(long Channel);
    VARIANT Evaluate(const VARIANT& Name);
    VARIANT _Evaluate(const VARIANT& Name);
    VARIANT ExecuteExcel4Macro(LPCTSTR String);
    LPDISPATCH Intersect(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                         const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                         const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    LPDISPATCH GetNames();
    LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
    LPDISPATCH GetRows();
    VARIANT Run(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9,
                const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19,
                const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29,
                const VARIANT& Arg30);
    VARIANT _Run2(const VARIANT& Macro, const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9,
                  const VARIANT& Arg10, const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19,
                  const VARIANT& Arg20, const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29,
                  const VARIANT& Arg30);
    LPDISPATCH GetSelection();
    void SendKeys(const VARIANT& Keys, const VARIANT& Wait);
    LPDISPATCH GetSheets();
    LPDISPATCH GetThisWorkbook();
    LPDISPATCH Union(LPDISPATCH Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
                     const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
                     const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    LPDISPATCH GetWindows();
    LPDISPATCH GetWorkbooks();
    LPDISPATCH GetWorksheetFunction();
    LPDISPATCH GetWorksheets();
    LPDISPATCH GetExcel4IntlMacroSheets();
    LPDISPATCH GetExcel4MacroSheets();
};
/////////////////////////////////////////////////////////////////////////////
// _Workbook wrapper class

class _Workbook : public COleDispatchDriver
{
public:
    _Workbook() {}      // Calls COleDispatchDriver default constructor
    _Workbook(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _Workbook(const _Workbook& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetAcceptLabelsInFormulas();
    void SetAcceptLabelsInFormulas(BOOL bNewValue);
    void Activate();
    LPDISPATCH GetActiveChart();
    LPDISPATCH GetActiveSheet();
    long GetAutoUpdateFrequency();
    void SetAutoUpdateFrequency(long nNewValue);
    BOOL GetAutoUpdateSaveChanges();
    void SetAutoUpdateSaveChanges(BOOL bNewValue);
    long GetChangeHistoryDuration();
    void SetChangeHistoryDuration(long nNewValue);
    LPDISPATCH GetBuiltinDocumentProperties();
    void ChangeFileAccess(long Mode, const VARIANT& WritePassword, const VARIANT& Notify);
    void ChangeLink(LPCTSTR Name, LPCTSTR NewName, long Type);
    LPDISPATCH GetCharts();
    void Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook);
    CString GetCodeName();
    CString Get_CodeName();
    void Set_CodeName(LPCTSTR lpszNewValue);
    VARIANT GetColors(const VARIANT& Index);
    void SetColors(const VARIANT& Index, const VARIANT& newValue);
    LPDISPATCH GetCommandBars();
    long GetConflictResolution();
    void SetConflictResolution(long nNewValue);
    LPDISPATCH GetContainer();
    BOOL GetCreateBackup();
    LPDISPATCH GetCustomDocumentProperties();
    BOOL GetDate1904();
    void SetDate1904(BOOL bNewValue);
    void DeleteNumberFormat(LPCTSTR NumberFormat);
    long GetDisplayDrawingObjects();
    void SetDisplayDrawingObjects(long nNewValue);
    BOOL ExclusiveAccess();
    long GetFileFormat();
    void ForwardMailer();
    CString GetFullName();
    BOOL GetHasPassword();
    BOOL GetHasRoutingSlip();
    void SetHasRoutingSlip(BOOL bNewValue);
    BOOL GetIsAddin();
    void SetIsAddin(BOOL bNewValue);
    VARIANT LinkInfo(LPCTSTR Name, long LinkInfo, const VARIANT& Type, const VARIANT& EditionRef);
    VARIANT LinkSources(const VARIANT& Type);
    LPDISPATCH GetMailer();
    void MergeWorkbook(const VARIANT& Filename);
    BOOL GetMultiUserEditing();
    CString GetName();
    LPDISPATCH GetNames();
    LPDISPATCH NewWindow();
    void OpenLinks(LPCTSTR Name, const VARIANT& ReadOnly, const VARIANT& Type);
    CString GetPath();
    BOOL GetPersonalViewListSettings();
    void SetPersonalViewListSettings(BOOL bNewValue);
    BOOL GetPersonalViewPrintSettings();
    void SetPersonalViewPrintSettings(BOOL bNewValue);
    LPDISPATCH PivotCaches();
    void Post(const VARIANT& DestName);
    BOOL GetPrecisionAsDisplayed();
    void SetPrecisionAsDisplayed(BOOL bNewValue);
    void PrintPreview(const VARIANT& EnableChanges);
    void ProtectSharing(const VARIANT& Filename, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& SharingPassword);
    BOOL GetProtectStructure();
    BOOL GetProtectWindows();
    BOOL GetReadOnly();
    void RefreshAll();
    void Reply();
    void ReplyAll();
    void RemoveUser(long Index);
    long GetRevisionNumber();
    void Route();
    BOOL GetRouted();
    LPDISPATCH GetRoutingSlip();
    void RunAutoMacros(long Which);
    void Save();
    void SaveCopyAs(const VARIANT& Filename);
    BOOL GetSaved();
    void SetSaved(BOOL bNewValue);
    BOOL GetSaveLinkValues();
    void SetSaveLinkValues(BOOL bNewValue);
    void SendMail(const VARIANT& Recipients, const VARIANT& Subject, const VARIANT& ReturnReceipt);
    void SendMailer(const VARIANT& FileFormat, long Priority);
    void SetLinkOnData(LPCTSTR Name, const VARIANT& Procedure);
    LPDISPATCH GetSheets();
    BOOL GetShowConflictHistory();
    void SetShowConflictHistory(BOOL bNewValue);
    LPDISPATCH GetStyles();
    void Unprotect(const VARIANT& Password);
    void UnprotectSharing(const VARIANT& SharingPassword);
    void UpdateFromFile();
    void UpdateLink(const VARIANT& Name, const VARIANT& Type);
    BOOL GetUpdateRemoteReferences();
    void SetUpdateRemoteReferences(BOOL bNewValue);
    VARIANT GetUserStatus();
    LPDISPATCH GetCustomViews();
    LPDISPATCH GetWindows();
    LPDISPATCH GetWorksheets();
    BOOL GetWriteReserved();
    CString GetWriteReservedBy();
    LPDISPATCH GetExcel4IntlMacroSheets();
    LPDISPATCH GetExcel4MacroSheets();
    BOOL GetTemplateRemoveExtData();
    void SetTemplateRemoveExtData(BOOL bNewValue);
    void HighlightChangesOptions(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
    BOOL GetHighlightChangesOnScreen();
    void SetHighlightChangesOnScreen(BOOL bNewValue);
    BOOL GetKeepChangeHistory();
    void SetKeepChangeHistory(BOOL bNewValue);
    BOOL GetListChangesOnNewSheet();
    void SetListChangesOnNewSheet(BOOL bNewValue);
    void PurgeChangeHistoryNow(long Days, const VARIANT& SharingPassword);
    void AcceptAllChanges(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
    void RejectAllChanges(const VARIANT& When, const VARIANT& Who, const VARIANT& Where);
    void ResetColors();
    LPDISPATCH GetVBProject();
    void FollowHyperlink(LPCTSTR Address, const VARIANT& SubAddress, const VARIANT& NewWindow, const VARIANT& AddHistory, const VARIANT& ExtraInfo, const VARIANT& Method, const VARIANT& HeaderInfo);
    void AddToFavorites();
    BOOL GetIsInplace();
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
    void WebPagePreview();
    LPDISPATCH GetPublishObjects();
    LPDISPATCH GetWebOptions();
    void ReloadAs(long Encoding);
    LPDISPATCH GetHTMLProject();
    BOOL GetEnvelopeVisible();
    void SetEnvelopeVisible(BOOL bNewValue);
    long GetCalculationVersion();
    BOOL GetVBASigned();
    BOOL GetShowPivotTableFieldList();
    void SetShowPivotTableFieldList(BOOL bNewValue);
    long GetUpdateLinks();
    void SetUpdateLinks(long nNewValue);
    void BreakLink(LPCTSTR Name, long Type);
    void SaveAs(const VARIANT& Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, long AccessMode, const VARIANT& ConflictResolution,
                const VARIANT& AddToMru, const VARIANT& TextCodepage, const VARIANT& TextVisualLayout, const VARIANT& Local);
    BOOL GetEnableAutoRecover();
    void SetEnableAutoRecover(BOOL bNewValue);
    BOOL GetRemovePersonalInformation();
    void SetRemovePersonalInformation(BOOL bNewValue);
    CString GetFullNameURLEncoded();
    void CheckIn(const VARIANT& SaveChanges, const VARIANT& Comments, const VARIANT& MakePublic);
    BOOL CanCheckIn();
    void SendForReview(const VARIANT& Recipients, const VARIANT& Subject, const VARIANT& ShowMessage, const VARIANT& IncludeAttachment);
    void ReplyWithChanges(const VARIANT& ShowMessage);
    void EndReview();
    CString GetPassword();
    void SetPassword(LPCTSTR lpszNewValue);
    CString GetWritePassword();
    void SetWritePassword(LPCTSTR lpszNewValue);
    CString GetPasswordEncryptionProvider();
    CString GetPasswordEncryptionAlgorithm();
    long GetPasswordEncryptionKeyLength();
    void SetPasswordEncryptionOptions(const VARIANT& PasswordEncryptionProvider, const VARIANT& PasswordEncryptionAlgorithm, const VARIANT& PasswordEncryptionKeyLength, const VARIANT& PasswordEncryptionFileProperties);
    BOOL GetPasswordEncryptionFileProperties();
    BOOL GetReadOnlyRecommended();
    void SetReadOnlyRecommended(BOOL bNewValue);
    void Protect(const VARIANT& Password, const VARIANT& Structure, const VARIANT& Windows);
    LPDISPATCH GetSmartTagOptions();
    void RecheckSmartTags();
    LPDISPATCH GetPermission();
    LPDISPATCH GetSharedWorkspace();
    LPDISPATCH GetSync();
    void SendFaxOverInternet(const VARIANT& Recipients, const VARIANT& Subject, const VARIANT& ShowMessage);
    LPDISPATCH GetXmlNamespaces();
    LPDISPATCH GetXmlMaps();
    long XmlImport(LPCTSTR Url, LPDISPATCH* ImportMap, const VARIANT& Overwrite, const VARIANT& Destination);
    LPDISPATCH GetSmartDocument();
    LPDISPATCH GetDocumentLibraryVersions();
    BOOL GetInactiveListBorderVisible();
    void SetInactiveListBorderVisible(BOOL bNewValue);
    BOOL GetDisplayInkComments();
    void SetDisplayInkComments(BOOL bNewValue);
    long XmlImportXml(LPCTSTR Data, LPDISPATCH* ImportMap, const VARIANT& Overwrite, const VARIANT& Destination);
    void SaveAsXMLData(LPCTSTR Filename, LPDISPATCH Map);
    void ToggleFormsDesign();
};
/////////////////////////////////////////////////////////////////////////////
// Workbooks wrapper class

class Workbooks : public COleDispatchDriver
{
public:
    Workbooks() {}      // Calls COleDispatchDriver default constructor
    Workbooks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Workbooks(const Workbooks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Template);
    void Close();
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH Open(LPCTSTR Filename, const VARIANT& UpdateLinks, const VARIANT& ReadOnly, const VARIANT& Format, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& IgnoreReadOnlyRecommended, const VARIANT& Origin,
                    const VARIANT& Delimiter, const VARIANT& Editable, const VARIANT& Notify, const VARIANT& Converter, const VARIANT& AddToMru, const VARIANT& Local, const VARIANT& CorruptLoad);
    void OpenText(LPCTSTR Filename, const VARIANT& Origin, const VARIANT& StartRow, const VARIANT& DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma,
                  const VARIANT& Space, const VARIANT& Other, const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& TextVisualLayout, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers,
                  const VARIANT& Local);
    LPDISPATCH OpenDatabase(LPCTSTR Filename, const VARIANT& CommandText, const VARIANT& CommandType, const VARIANT& BackgroundQuery, const VARIANT& ImportDataAs);
    void CheckOut(LPCTSTR Filename);
    BOOL CanCheckOut(LPCTSTR Filename);
    LPDISPATCH OpenXML(LPCTSTR Filename, const VARIANT& Stylesheets, const VARIANT& LoadOption);
};
/////////////////////////////////////////////////////////////////////////////
// PublishObject wrapper class

class PublishObject : public COleDispatchDriver
{
public:
    PublishObject() {}      // Calls COleDispatchDriver default constructor
    PublishObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PublishObject(const PublishObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    void Publish(const VARIANT& Create);
    CString GetDivID();
    CString GetSheet();
    long GetSourceType();
    CString GetSource();
    long GetHtmlType();
    void SetHtmlType(long nNewValue);
    CString GetTitle();
    void SetTitle(LPCTSTR lpszNewValue);
    CString GetFilename();
    void SetFilename(LPCTSTR lpszNewValue);
    BOOL GetAutoRepublish();
    void SetAutoRepublish(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DefaultWebOptions wrapper class

class DefaultWebOptions : public COleDispatchDriver
{
public:
    DefaultWebOptions() {}      // Calls COleDispatchDriver default constructor
    DefaultWebOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DefaultWebOptions(const DefaultWebOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetRelyOnCSS();
    void SetRelyOnCSS(BOOL bNewValue);
    BOOL GetSaveHiddenData();
    void SetSaveHiddenData(BOOL bNewValue);
    BOOL GetLoadPictures();
    void SetLoadPictures(BOOL bNewValue);
    BOOL GetOrganizeInFolder();
    void SetOrganizeInFolder(BOOL bNewValue);
    BOOL GetUpdateLinksOnSave();
    void SetUpdateLinksOnSave(BOOL bNewValue);
    BOOL GetUseLongFileNames();
    void SetUseLongFileNames(BOOL bNewValue);
    BOOL GetCheckIfOfficeIsHTMLEditor();
    void SetCheckIfOfficeIsHTMLEditor(BOOL bNewValue);
    BOOL GetDownloadComponents();
    void SetDownloadComponents(BOOL bNewValue);
    BOOL GetRelyOnVML();
    void SetRelyOnVML(BOOL bNewValue);
    BOOL GetAllowPNG();
    void SetAllowPNG(BOOL bNewValue);
    long GetScreenSize();
    void SetScreenSize(long nNewValue);
    long GetPixelsPerInch();
    void SetPixelsPerInch(long nNewValue);
    CString GetLocationOfComponents();
    void SetLocationOfComponents(LPCTSTR lpszNewValue);
    long GetEncoding();
    void SetEncoding(long nNewValue);
    BOOL GetAlwaysSaveInDefaultEncoding();
    void SetAlwaysSaveInDefaultEncoding(BOOL bNewValue);
    LPDISPATCH GetFonts();
    CString GetFolderSuffix();
    long GetTargetBrowser();
    void SetTargetBrowser(long nNewValue);
    BOOL GetSaveNewWebPagesAsWebArchives();
    void SetSaveNewWebPagesAsWebArchives(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// WebOptions wrapper class

class WebOptions : public COleDispatchDriver
{
public:
    WebOptions() {}     // Calls COleDispatchDriver default constructor
    WebOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    WebOptions(const WebOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetRelyOnCSS();
    void SetRelyOnCSS(BOOL bNewValue);
    BOOL GetOrganizeInFolder();
    void SetOrganizeInFolder(BOOL bNewValue);
    BOOL GetUseLongFileNames();
    void SetUseLongFileNames(BOOL bNewValue);
    BOOL GetDownloadComponents();
    void SetDownloadComponents(BOOL bNewValue);
    BOOL GetRelyOnVML();
    void SetRelyOnVML(BOOL bNewValue);
    BOOL GetAllowPNG();
    void SetAllowPNG(BOOL bNewValue);
    long GetScreenSize();
    void SetScreenSize(long nNewValue);
    long GetPixelsPerInch();
    void SetPixelsPerInch(long nNewValue);
    CString GetLocationOfComponents();
    void SetLocationOfComponents(LPCTSTR lpszNewValue);
    long GetEncoding();
    void SetEncoding(long nNewValue);
    CString GetFolderSuffix();
    void UseDefaultFolderSuffix();
    long GetTargetBrowser();
    void SetTargetBrowser(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// TreeviewControl wrapper class

class TreeviewControl : public COleDispatchDriver
{
public:
    TreeviewControl() {}        // Calls COleDispatchDriver default constructor
    TreeviewControl(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    TreeviewControl(const TreeviewControl& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetHidden();
    void SetHidden(const VARIANT& newValue);
    VARIANT GetDrilled();
    void SetDrilled(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CubeField wrapper class

class CubeField : public COleDispatchDriver
{
public:
    CubeField() {}      // Calls COleDispatchDriver default constructor
    CubeField(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CubeField(const CubeField& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCubeFieldType();
    CString GetCaption();
    CString GetName();
    CString GetValue();
    long GetOrientation();
    void SetOrientation(long nNewValue);
    long GetPosition();
    void SetPosition(long nNewValue);
    LPDISPATCH GetTreeviewControl();
    BOOL GetDragToColumn();
    void SetDragToColumn(BOOL bNewValue);
    BOOL GetDragToHide();
    void SetDragToHide(BOOL bNewValue);
    BOOL GetDragToPage();
    void SetDragToPage(BOOL bNewValue);
    BOOL GetDragToRow();
    void SetDragToRow(BOOL bNewValue);
    BOOL GetDragToData();
    void SetDragToData(BOOL bNewValue);
    long GetHiddenLevels();
    void SetHiddenLevels(long nNewValue);
    BOOL GetHasMemberProperties();
    long GetLayoutForm();
    void SetLayoutForm(long nNewValue);
    LPDISPATCH GetPivotFields();
    void AddMemberPropertyField(LPCTSTR Property_, const VARIANT& PropertyOrder);
    BOOL GetEnableMultiplePageItems();
    void SetEnableMultiplePageItems(BOOL bNewValue);
    long GetLayoutSubtotalLocation();
    void SetLayoutSubtotalLocation(long nNewValue);
    BOOL GetShowInFieldList();
    void SetShowInFieldList(BOOL bNewValue);
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// CubeFields wrapper class

class CubeFields : public COleDispatchDriver
{
public:
    CubeFields() {}     // Calls COleDispatchDriver default constructor
    CubeFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CubeFields(const CubeFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH AddSet(LPCTSTR Name, LPCTSTR Caption);
};
/////////////////////////////////////////////////////////////////////////////
// Font wrapper class

class Font : public COleDispatchDriver
{
public:
    Font() {}       // Calls COleDispatchDriver default constructor
    Font(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Font(const Font& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetBackground();
    void SetBackground(const VARIANT& newValue);
    VARIANT GetBold();
    void SetBold(const VARIANT& newValue);
    VARIANT GetColor();
    void SetColor(const VARIANT& newValue);
    VARIANT GetColorIndex();
    void SetColorIndex(const VARIANT& newValue);
    VARIANT GetFontStyle();
    void SetFontStyle(const VARIANT& newValue);
    VARIANT GetItalic();
    void SetItalic(const VARIANT& newValue);
    VARIANT GetName();
    void SetName(const VARIANT& newValue);
    VARIANT GetOutlineFont();
    void SetOutlineFont(const VARIANT& newValue);
    VARIANT GetShadow();
    void SetShadow(const VARIANT& newValue);
    VARIANT GetSize();
    void SetSize(const VARIANT& newValue);
    VARIANT GetStrikethrough();
    void SetStrikethrough(const VARIANT& newValue);
    VARIANT GetSubscript();
    void SetSubscript(const VARIANT& newValue);
    VARIANT GetSuperscript();
    void SetSuperscript(const VARIANT& newValue);
    VARIANT GetUnderline();
    void SetUnderline(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Window wrapper class

class Window : public COleDispatchDriver
{
public:
    Window() {}     // Calls COleDispatchDriver default constructor
    Window(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Window(const Window& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT Activate();
    VARIANT ActivateNext();
    VARIANT ActivatePrevious();
    LPDISPATCH GetActiveCell();
    LPDISPATCH GetActiveChart();
    LPDISPATCH GetActivePane();
    LPDISPATCH GetActiveSheet();
    VARIANT GetCaption();
    void SetCaption(const VARIANT& newValue);
    BOOL Close(const VARIANT& SaveChanges, const VARIANT& Filename, const VARIANT& RouteWorkbook);
    BOOL GetDisplayFormulas();
    void SetDisplayFormulas(BOOL bNewValue);
    BOOL GetDisplayGridlines();
    void SetDisplayGridlines(BOOL bNewValue);
    BOOL GetDisplayHeadings();
    void SetDisplayHeadings(BOOL bNewValue);
    BOOL GetDisplayHorizontalScrollBar();
    void SetDisplayHorizontalScrollBar(BOOL bNewValue);
    BOOL GetDisplayOutline();
    void SetDisplayOutline(BOOL bNewValue);
    BOOL GetDisplayVerticalScrollBar();
    void SetDisplayVerticalScrollBar(BOOL bNewValue);
    BOOL GetDisplayWorkbookTabs();
    void SetDisplayWorkbookTabs(BOOL bNewValue);
    BOOL GetDisplayZeros();
    void SetDisplayZeros(BOOL bNewValue);
    BOOL GetEnableResize();
    void SetEnableResize(BOOL bNewValue);
    BOOL GetFreezePanes();
    void SetFreezePanes(BOOL bNewValue);
    long GetGridlineColor();
    void SetGridlineColor(long nNewValue);
    long GetGridlineColorIndex();
    void SetGridlineColorIndex(long nNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    VARIANT LargeScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
    double GetLeft();
    void SetLeft(double newValue);
    LPDISPATCH NewWindow();
    CString GetOnWindow();
    void SetOnWindow(LPCTSTR lpszNewValue);
    LPDISPATCH GetPanes();
    VARIANT PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
    VARIANT PrintPreview(const VARIANT& EnableChanges);
    LPDISPATCH GetRangeSelection();
    long GetScrollColumn();
    void SetScrollColumn(long nNewValue);
    long GetScrollRow();
    void SetScrollRow(long nNewValue);
    VARIANT ScrollWorkbookTabs(const VARIANT& Sheets, const VARIANT& Position);
    LPDISPATCH GetSelectedSheets();
    LPDISPATCH GetSelection();
    VARIANT SmallScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
    BOOL GetSplit();
    void SetSplit(BOOL bNewValue);
    long GetSplitColumn();
    void SetSplitColumn(long nNewValue);
    double GetSplitHorizontal();
    void SetSplitHorizontal(double newValue);
    long GetSplitRow();
    void SetSplitRow(long nNewValue);
    double GetSplitVertical();
    void SetSplitVertical(double newValue);
    double GetTabRatio();
    void SetTabRatio(double newValue);
    double GetTop();
    void SetTop(double newValue);
    long GetType();
    double GetUsableHeight();
    double GetUsableWidth();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    LPDISPATCH GetVisibleRange();
    double GetWidth();
    void SetWidth(double newValue);
    long GetWindowNumber();
    long GetWindowState();
    void SetWindowState(long nNewValue);
    VARIANT GetZoom();
    void SetZoom(const VARIANT& newValue);
    long GetView();
    void SetView(long nNewValue);
    BOOL GetDisplayRightToLeft();
    void SetDisplayRightToLeft(BOOL bNewValue);
    long PointsToScreenPixelsX(long Points);
    long PointsToScreenPixelsY(long Points);
    LPDISPATCH RangeFromPoint(long x, long y);
    void ScrollIntoView(long Left, long Top, long Width, long Height, const VARIANT& Start);
};
/////////////////////////////////////////////////////////////////////////////
// Windows wrapper class

class Windows : public COleDispatchDriver
{
public:
    Windows() {}        // Calls COleDispatchDriver default constructor
    Windows(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Windows(const Windows& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT Arrange(long ArrangeStyle, const VARIANT& ActiveWorkbook, const VARIANT& SyncHorizontal, const VARIANT& SyncVertical);
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(const VARIANT& Index);
    BOOL CompareSideBySideWith(const VARIANT& WindowName);
    BOOL BreakSideBySide();
    BOOL GetSyncScrollingSideBySide();
    void SetSyncScrollingSideBySide(BOOL bNewValue);
    void ResetPositionsSideBySide();
};
/////////////////////////////////////////////////////////////////////////////
// AppEvents wrapper class

class AppEvents : public COleDispatchDriver
{
public:
    AppEvents() {}      // Calls COleDispatchDriver default constructor
    AppEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AppEvents(const AppEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void NewWorkbook(LPDISPATCH Wb);
    void SheetSelectionChange(LPDISPATCH Sh, LPDISPATCH Target);
    void SheetBeforeDoubleClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
    void SheetBeforeRightClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
    void SheetActivate(LPDISPATCH Sh);
    void SheetDeactivate(LPDISPATCH Sh);
    void SheetCalculate(LPDISPATCH Sh);
    void SheetChange(LPDISPATCH Sh, LPDISPATCH Target);
    void WorkbookOpen(LPDISPATCH Wb);
    void WorkbookActivate(LPDISPATCH Wb);
    void WorkbookDeactivate(LPDISPATCH Wb);
    void WorkbookBeforeClose(LPDISPATCH Wb, BOOL* Cancel);
    void WorkbookBeforeSave(LPDISPATCH Wb, BOOL SaveAsUI, BOOL* Cancel);
    void WorkbookBeforePrint(LPDISPATCH Wb, BOOL* Cancel);
    void WorkbookNewSheet(LPDISPATCH Wb, LPDISPATCH Sh);
    void WorkbookAddinInstall(LPDISPATCH Wb);
    void WorkbookAddinUninstall(LPDISPATCH Wb);
    void WindowResize(LPDISPATCH Wb, LPDISPATCH Wn);
    void WindowActivate(LPDISPATCH Wb, LPDISPATCH Wn);
    void WindowDeactivate(LPDISPATCH Wb, LPDISPATCH Wn);
    void SheetFollowHyperlink(LPDISPATCH Sh, LPDISPATCH Target);
    void SheetPivotTableUpdate(LPDISPATCH Sh, LPDISPATCH Target);
    void WorkbookPivotTableCloseConnection(LPDISPATCH Wb, LPDISPATCH Target);
    void WorkbookPivotTableOpenConnection(LPDISPATCH Wb, LPDISPATCH Target);
    void WorkbookSync(LPDISPATCH Wb, long SyncEventType);
    void WorkbookBeforeXmlImport(LPDISPATCH Wb, LPDISPATCH Map, LPCTSTR Url, BOOL IsRefresh, BOOL* Cancel);
    void WorkbookAfterXmlImport(LPDISPATCH Wb, LPDISPATCH Map, BOOL IsRefresh, long Result);
    void WorkbookBeforeXmlExport(LPDISPATCH Wb, LPDISPATCH Map, LPCTSTR Url, BOOL* Cancel);
    void WorkbookAfterXmlExport(LPDISPATCH Wb, LPDISPATCH Map, LPCTSTR Url, long Result);
};
/////////////////////////////////////////////////////////////////////////////
// WorksheetFunction wrapper class

class WorksheetFunction : public COleDispatchDriver
{
public:
    WorksheetFunction() {}      // Calls COleDispatchDriver default constructor
    WorksheetFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    WorksheetFunction(const WorksheetFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    double Count(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                 const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                 const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    BOOL IsNA(const VARIANT& Arg1);
    BOOL IsError(const VARIANT& Arg1);
    double Sum(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
               const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
               const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Average(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                   const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                   const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Min(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
               const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
               const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Max(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
               const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
               const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Npv(double Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
               const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
               const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double StDev(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                 const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                 const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    CString Dollar(double Arg1, const VARIANT& Arg2);
    CString Fixed(double Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double Pi();
    double Ln(double Arg1);
    double Log10(double Arg1);
    double Round(double Arg1, double Arg2);
    VARIANT Lookup(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    VARIANT Index(const VARIANT& Arg1, double Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    CString Rept(LPCTSTR Arg1, double Arg2);
    BOOL And(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
             const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
             const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    BOOL Or(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
            const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
            const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double DCount(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double DSum(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double DAverage(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double DMin(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double DMax(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double DStDev(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double Var(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
               const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
               const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double DVar(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    CString Text(const VARIANT& Arg1, LPCTSTR Arg2);
    VARIANT LinEst(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    VARIANT Trend(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    VARIANT LogEst(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    VARIANT Growth(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    double Pv(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
    double Fv(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
    double NPer(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
    double Pmt(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
    double Rate(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6);
    double MIrr(const VARIANT& Arg1, double Arg2, double Arg3);
    double Irr(const VARIANT& Arg1, const VARIANT& Arg2);
    double Match(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double Weekday(const VARIANT& Arg1, const VARIANT& Arg2);
    double Search(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
    VARIANT Transpose(const VARIANT& Arg1);
    double Atan2(double Arg1, double Arg2);
    double Asin(double Arg1);
    double Acos(double Arg1);
    VARIANT Choose(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                   const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                   const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    VARIANT HLookup(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    VARIANT VLookup(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4);
    double Log(double Arg1, const VARIANT& Arg2);
    CString Proper(LPCTSTR Arg1);
    CString Trim(LPCTSTR Arg1);
    CString Replace(LPCTSTR Arg1, double Arg2, double Arg3, LPCTSTR Arg4);
    CString Substitute(LPCTSTR Arg1, LPCTSTR Arg2, LPCTSTR Arg3, const VARIANT& Arg4);
    double Find(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
    BOOL IsErr(const VARIANT& Arg1);
    BOOL IsText(const VARIANT& Arg1);
    BOOL IsNumber(const VARIANT& Arg1);
    double Sln(double Arg1, double Arg2, double Arg3);
    double Syd(double Arg1, double Arg2, double Arg3, double Arg4);
    double Ddb(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5);
    CString Clean(LPCTSTR Arg1);
    double MDeterm(const VARIANT& Arg1);
    VARIANT MInverse(const VARIANT& Arg1);
    VARIANT MMult(const VARIANT& Arg1, const VARIANT& Arg2);
    double Ipmt(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5, const VARIANT& Arg6);
    double Ppmt(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5, const VARIANT& Arg6);
    double CountA(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                  const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                  const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Product(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                   const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                   const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Fact(double Arg1);
    double DProduct(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    BOOL IsNonText(const VARIANT& Arg1);
    double StDevP(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                  const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                  const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double VarP(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double DStDevP(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double DVarP(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    BOOL IsLogical(const VARIANT& Arg1);
    double DCountA(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    CString USDollar(double Arg1, double Arg2);
    double FindB(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
    double SearchB(LPCTSTR Arg1, LPCTSTR Arg2, const VARIANT& Arg3);
    CString ReplaceB(LPCTSTR Arg1, double Arg2, double Arg3, LPCTSTR Arg4);
    double RoundUp(double Arg1, double Arg2);
    double RoundDown(double Arg1, double Arg2);
    double Rank(double Arg1, LPDISPATCH Arg2, const VARIANT& Arg3);
    double Days360(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double Vdb(double Arg1, double Arg2, double Arg3, double Arg4, double Arg5, const VARIANT& Arg6, const VARIANT& Arg7);
    double Median(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                  const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                  const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double SumProduct(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                      const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                      const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Sinh(double Arg1);
    double Cosh(double Arg1);
    double Tanh(double Arg1);
    double Asinh(double Arg1);
    double Acosh(double Arg1);
    double Atanh(double Arg1);
    VARIANT DGet(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double Db(double Arg1, double Arg2, double Arg3, double Arg4, const VARIANT& Arg5);
    VARIANT Frequency(const VARIANT& Arg1, const VARIANT& Arg2);
    double AveDev(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                  const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                  const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double BetaDist(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
    double GammaLn(double Arg1);
    double BetaInv(double Arg1, double Arg2, double Arg3, const VARIANT& Arg4, const VARIANT& Arg5);
    double BinomDist(double Arg1, double Arg2, double Arg3, BOOL Arg4);
    double ChiDist(double Arg1, double Arg2);
    double ChiInv(double Arg1, double Arg2);
    double Combin(double Arg1, double Arg2);
    double Confidence(double Arg1, double Arg2, double Arg3);
    double CritBinom(double Arg1, double Arg2, double Arg3);
    double Even(double Arg1);
    double ExponDist(double Arg1, double Arg2, BOOL Arg3);
    double FDist(double Arg1, double Arg2, double Arg3);
    double FInv(double Arg1, double Arg2, double Arg3);
    double Fisher(double Arg1);
    double FisherInv(double Arg1);
    double Floor(double Arg1, double Arg2);
    double GammaDist(double Arg1, double Arg2, double Arg3, BOOL Arg4);
    double GammaInv(double Arg1, double Arg2, double Arg3);
    double Ceiling(double Arg1, double Arg2);
    double HypGeomDist(double Arg1, double Arg2, double Arg3, double Arg4);
    double LogNormDist(double Arg1, double Arg2, double Arg3);
    double LogInv(double Arg1, double Arg2, double Arg3);
    double NegBinomDist(double Arg1, double Arg2, double Arg3);
    double NormDist(double Arg1, double Arg2, double Arg3, BOOL Arg4);
    double NormSDist(double Arg1);
    double NormInv(double Arg1, double Arg2, double Arg3);
    double NormSInv(double Arg1);
    double Standardize(double Arg1, double Arg2, double Arg3);
    double Odd(double Arg1);
    double Permut(double Arg1, double Arg2);
    double Poisson(double Arg1, double Arg2, BOOL Arg3);
    double TDist(double Arg1, double Arg2, double Arg3);
    double Weibull(double Arg1, double Arg2, double Arg3, BOOL Arg4);
    double SumXMY2(const VARIANT& Arg1, const VARIANT& Arg2);
    double SumX2MY2(const VARIANT& Arg1, const VARIANT& Arg2);
    double SumX2PY2(const VARIANT& Arg1, const VARIANT& Arg2);
    double ChiTest(const VARIANT& Arg1, const VARIANT& Arg2);
    double Correl(const VARIANT& Arg1, const VARIANT& Arg2);
    double Covar(const VARIANT& Arg1, const VARIANT& Arg2);
    double Forecast(double Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double FTest(const VARIANT& Arg1, const VARIANT& Arg2);
    double Intercept(const VARIANT& Arg1, const VARIANT& Arg2);
    double Pearson(const VARIANT& Arg1, const VARIANT& Arg2);
    double RSq(const VARIANT& Arg1, const VARIANT& Arg2);
    double StEyx(const VARIANT& Arg1, const VARIANT& Arg2);
    double Slope(const VARIANT& Arg1, const VARIANT& Arg2);
    double TTest(const VARIANT& Arg1, const VARIANT& Arg2, double Arg3, double Arg4);
    double Prob(const VARIANT& Arg1, const VARIANT& Arg2, double Arg3, const VARIANT& Arg4);
    double DevSq(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                 const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                 const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double GeoMean(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                   const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                   const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double HarMean(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                   const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                   const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double SumSq(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                 const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                 const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Kurt(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double Skew(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double ZTest(const VARIANT& Arg1, double Arg2, const VARIANT& Arg3);
    double Large(const VARIANT& Arg1, double Arg2);
    double Small(const VARIANT& Arg1, double Arg2);
    double Quartile(const VARIANT& Arg1, double Arg2);
    double Percentile(const VARIANT& Arg1, double Arg2);
    double PercentRank(const VARIANT& Arg1, double Arg2, const VARIANT& Arg3);
    double Mode(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double TrimMean(const VARIANT& Arg1, double Arg2);
    double TInv(double Arg1, double Arg2);
    double Power(double Arg1, double Arg2);
    double Radians(double Arg1);
    double Degrees(double Arg1);
    double Subtotal(double Arg1, LPDISPATCH Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
                    const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
                    const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    double SumIf(LPDISPATCH Arg1, const VARIANT& Arg2, const VARIANT& Arg3);
    double CountIf(LPDISPATCH Arg1, const VARIANT& Arg2);
    double CountBlank(LPDISPATCH Arg1);
    double Ispmt(double Arg1, double Arg2, double Arg3, double Arg4);
    CString Roman(double Arg1, const VARIANT& Arg2);
    CString Asc(LPCTSTR Arg1);
    CString Dbcs(LPCTSTR Arg1);
    CString Phonetic(LPDISPATCH Arg1);
    CString BahtText(double Arg1);
    VARIANT RTD(const VARIANT& progID, const VARIANT& server, const VARIANT& topic1, const VARIANT& topic2, const VARIANT& topic3, const VARIANT& topic4, const VARIANT& topic5, const VARIANT& topic6, const VARIANT& topic7, const VARIANT& topic8,
                const VARIANT& topic9, const VARIANT& topic10, const VARIANT& topic11, const VARIANT& topic12, const VARIANT& topic13, const VARIANT& topic14, const VARIANT& topic15, const VARIANT& topic16, const VARIANT& topic17, const VARIANT& topic18,
                const VARIANT& topic19, const VARIANT& topic20, const VARIANT& topic21, const VARIANT& topic22, const VARIANT& topic23, const VARIANT& topic24, const VARIANT& topic25, const VARIANT& topic26, const VARIANT& topic27,
                const VARIANT& topic28);
};
/////////////////////////////////////////////////////////////////////////////
// Range wrapper class

class Range : public COleDispatchDriver
{
public:
    Range() {}      // Calls COleDispatchDriver default constructor
    Range(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Range(const Range& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT Activate();
    VARIANT GetAddIndent();
    void SetAddIndent(const VARIANT& newValue);
    CString GetAddress(const VARIANT& RowAbsolute, const VARIANT& ColumnAbsolute, long ReferenceStyle, const VARIANT& External, const VARIANT& RelativeTo);
    CString GetAddressLocal(const VARIANT& RowAbsolute, const VARIANT& ColumnAbsolute, long ReferenceStyle, const VARIANT& External, const VARIANT& RelativeTo);
    VARIANT AdvancedFilter(long Action, const VARIANT& CriteriaRange, const VARIANT& CopyToRange, const VARIANT& Unique);
    VARIANT ApplyNames(const VARIANT& Names, const VARIANT& IgnoreRelativeAbsolute, const VARIANT& UseRowColumnNames, const VARIANT& OmitColumn, const VARIANT& OmitRow, long Order, const VARIANT& AppendLast);
    VARIANT ApplyOutlineStyles();
    LPDISPATCH GetAreas();
    CString AutoComplete(LPCTSTR String);
    VARIANT AutoFill(LPDISPATCH Destination, long Type);
    VARIANT AutoFilter(const VARIANT& Field, const VARIANT& Criteria1, long Operator, const VARIANT& Criteria2, const VARIANT& VisibleDropDown);
    VARIANT AutoFit();
    VARIANT AutoFormat(long Format, const VARIANT& Number, const VARIANT& Font, const VARIANT& Alignment, const VARIANT& Border, const VARIANT& Pattern, const VARIANT& Width);
    VARIANT AutoOutline();
    VARIANT BorderAround(const VARIANT& LineStyle, long Weight, long ColorIndex, const VARIANT& Color);
    LPDISPATCH GetBorders();
    VARIANT Calculate();
    LPDISPATCH GetCells();
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    VARIANT Clear();
    VARIANT ClearContents();
    VARIANT ClearFormats();
    VARIANT ClearNotes();
    VARIANT ClearOutline();
    long GetColumn();
    LPDISPATCH ColumnDifferences(const VARIANT& Comparison);
    LPDISPATCH GetColumns();
    VARIANT GetColumnWidth();
    void SetColumnWidth(const VARIANT& newValue);
    VARIANT Consolidate(const VARIANT& Sources, const VARIANT& Function, const VARIANT& TopRow, const VARIANT& LeftColumn, const VARIANT& CreateLinks);
    VARIANT Copy(const VARIANT& Destination);
    long CopyFromRecordset(LPUNKNOWN Data, const VARIANT& MaxRows, const VARIANT& MaxColumns);
    VARIANT CopyPicture(long Appearance, long Format);
    long GetCount();
    VARIANT CreateNames(const VARIANT& Top, const VARIANT& Left, const VARIANT& Bottom, const VARIANT& Right);
    VARIANT CreatePublisher(const VARIANT& Edition, long Appearance, const VARIANT& ContainsPICT, const VARIANT& ContainsBIFF, const VARIANT& ContainsRTF, const VARIANT& ContainsVALU);
    LPDISPATCH GetCurrentArray();
    LPDISPATCH GetCurrentRegion();
    VARIANT Cut(const VARIANT& Destination);
    VARIANT DataSeries(const VARIANT& Rowcol, long Type, long Date, const VARIANT& Step, const VARIANT& Stop, const VARIANT& Trend);
    VARIANT Get_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex);
    void Set_Default(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue);
    VARIANT Delete(const VARIANT& Shift);
    LPDISPATCH GetDependents();
    VARIANT DialogBox_();
    LPDISPATCH GetDirectDependents();
    LPDISPATCH GetDirectPrecedents();
    VARIANT EditionOptions(long Type, long Option, const VARIANT& Name, const VARIANT& Reference, long Appearance, long ChartSize, const VARIANT& Format);
    LPDISPATCH GetEnd(long Direction);
    LPDISPATCH GetEntireColumn();
    LPDISPATCH GetEntireRow();
    VARIANT FillDown();
    VARIANT FillLeft();
    VARIANT FillRight();
    VARIANT FillUp();
    LPDISPATCH Find(const VARIANT& What, const VARIANT& After, const VARIANT& LookIn, const VARIANT& LookAt, const VARIANT& SearchOrder, long SearchDirection, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat);
    LPDISPATCH FindNext(const VARIANT& After);
    LPDISPATCH FindPrevious(const VARIANT& After);
    LPDISPATCH GetFont();
    VARIANT GetFormula();
    void SetFormula(const VARIANT& newValue);
    VARIANT GetFormulaArray();
    void SetFormulaArray(const VARIANT& newValue);
    long GetFormulaLabel();
    void SetFormulaLabel(long nNewValue);
    VARIANT GetFormulaHidden();
    void SetFormulaHidden(const VARIANT& newValue);
    VARIANT GetFormulaLocal();
    void SetFormulaLocal(const VARIANT& newValue);
    VARIANT GetFormulaR1C1();
    void SetFormulaR1C1(const VARIANT& newValue);
    VARIANT GetFormulaR1C1Local();
    void SetFormulaR1C1Local(const VARIANT& newValue);
    VARIANT FunctionWizard();
    BOOL GoalSeek(const VARIANT& Goal, LPDISPATCH ChangingCell);
    VARIANT Group(const VARIANT& Start, const VARIANT& End, const VARIANT& By, const VARIANT& Periods);
    VARIANT GetHasArray();
    VARIANT GetHasFormula();
    VARIANT GetHeight();
    VARIANT GetHidden();
    void SetHidden(const VARIANT& newValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    VARIANT GetIndentLevel();
    void SetIndentLevel(const VARIANT& newValue);
    void InsertIndent(long InsertAmount);
    VARIANT Insert(const VARIANT& Shift, const VARIANT& CopyOrigin);
    LPDISPATCH GetInterior();
    VARIANT GetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex);
    void SetItem(const VARIANT& RowIndex, const VARIANT& ColumnIndex, const VARIANT& newValue);
    VARIANT Justify();
    VARIANT GetLeft();
    long GetListHeaderRows();
    VARIANT ListNames();
    long GetLocationInTable();
    VARIANT GetLocked();
    void SetLocked(const VARIANT& newValue);
    void Merge(const VARIANT& Across);
    void UnMerge();
    LPDISPATCH GetMergeArea();
    VARIANT GetMergeCells();
    void SetMergeCells(const VARIANT& newValue);
    VARIANT GetName();
    void SetName(const VARIANT& newValue);
    VARIANT NavigateArrow(const VARIANT& TowardPrecedent, const VARIANT& ArrowNumber, const VARIANT& LinkNumber);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH GetNext();
    CString NoteText(const VARIANT& Text, const VARIANT& Start, const VARIANT& Length);
    VARIANT GetNumberFormat();
    void SetNumberFormat(const VARIANT& newValue);
    VARIANT GetNumberFormatLocal();
    void SetNumberFormatLocal(const VARIANT& newValue);
    LPDISPATCH GetOffset(const VARIANT& RowOffset, const VARIANT& ColumnOffset);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    VARIANT GetOutlineLevel();
    void SetOutlineLevel(const VARIANT& newValue);
    long GetPageBreak();
    void SetPageBreak(long nNewValue);
    VARIANT Parse(const VARIANT& ParseLine, const VARIANT& Destination);
    LPDISPATCH GetPivotField();
    LPDISPATCH GetPivotItem();
    LPDISPATCH GetPivotTable();
    LPDISPATCH GetPrecedents();
    VARIANT GetPrefixCharacter();
    LPDISPATCH GetPrevious();
    VARIANT _PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate);
    VARIANT PrintPreview(const VARIANT& EnableChanges);
    LPDISPATCH GetQueryTable();
    LPDISPATCH GetRange(const VARIANT& Cell1, const VARIANT& Cell2);
    VARIANT RemoveSubtotal();
    BOOL Replace(const VARIANT& What, const VARIANT& Replacement, const VARIANT& LookAt, const VARIANT& SearchOrder, const VARIANT& MatchCase, const VARIANT& MatchByte, const VARIANT& SearchFormat, const VARIANT& ReplaceFormat);
    LPDISPATCH GetResize(const VARIANT& RowSize, const VARIANT& ColumnSize);
    long GetRow();
    LPDISPATCH RowDifferences(const VARIANT& Comparison);
    VARIANT GetRowHeight();
    void SetRowHeight(const VARIANT& newValue);
    LPDISPATCH GetRows();
    VARIANT Run(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10,
                const VARIANT& Arg11, const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20,
                const VARIANT& Arg21, const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
    VARIANT Select();
    VARIANT Show();
    VARIANT ShowDependents(const VARIANT& Remove);
    VARIANT GetShowDetail();
    void SetShowDetail(const VARIANT& newValue);
    VARIANT ShowErrors();
    VARIANT ShowPrecedents(const VARIANT& Remove);
    VARIANT GetShrinkToFit();
    void SetShrinkToFit(const VARIANT& newValue);
    VARIANT Sort(const VARIANT& Key1, long Order1, const VARIANT& Key2, const VARIANT& Type, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase, long Orientation, long SortMethod,
                 long DataOption1, long DataOption2, long DataOption3);
    VARIANT SortSpecial(long SortMethod, const VARIANT& Key1, long Order1, const VARIANT& Type, const VARIANT& Key2, long Order2, const VARIANT& Key3, long Order3, long Header, const VARIANT& OrderCustom, const VARIANT& MatchCase,
                        long Orientation, long DataOption1, long DataOption2, long DataOption3);
    LPDISPATCH GetSoundNote();
    LPDISPATCH SpecialCells(long Type, const VARIANT& Value);
    VARIANT GetStyle();
    void SetStyle(const VARIANT& newValue);
    VARIANT SubscribeTo(LPCTSTR Edition, long Format);
    VARIANT Subtotal(long GroupBy, long Function, const VARIANT& TotalList, const VARIANT& Replace, const VARIANT& PageBreaks, long SummaryBelowData);
    VARIANT GetSummary();
    VARIANT Table(const VARIANT& RowInput, const VARIANT& ColumnInput);
    VARIANT GetText();
    VARIANT TextToColumns(const VARIANT& Destination, long DataType, long TextQualifier, const VARIANT& ConsecutiveDelimiter, const VARIANT& Tab, const VARIANT& Semicolon, const VARIANT& Comma, const VARIANT& Space, const VARIANT& Other,
                          const VARIANT& OtherChar, const VARIANT& FieldInfo, const VARIANT& DecimalSeparator, const VARIANT& ThousandsSeparator, const VARIANT& TrailingMinusNumbers);
    VARIANT GetTop();
    VARIANT Ungroup();
    VARIANT GetUseStandardHeight();
    void SetUseStandardHeight(const VARIANT& newValue);
    VARIANT GetUseStandardWidth();
    void SetUseStandardWidth(const VARIANT& newValue);
    LPDISPATCH GetValidation();
    VARIANT GetValue(const VARIANT& RangeValueDataType);
    void SetValue(const VARIANT& RangeValueDataType, const VARIANT& newValue);
    VARIANT GetValue2();
    void SetValue2(const VARIANT& newValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    VARIANT GetWidth();
    LPDISPATCH GetWorksheet();
    VARIANT GetWrapText();
    void SetWrapText(const VARIANT& newValue);
    LPDISPATCH AddComment(const VARIANT& Text);
    LPDISPATCH GetComment();
    void ClearComments();
    LPDISPATCH GetPhonetic();
    LPDISPATCH GetFormatConditions();
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetHyperlinks();
    LPDISPATCH GetPhonetics();
    void SetPhonetic();
    CString GetId();
    void SetId(LPCTSTR lpszNewValue);
    VARIANT PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
    LPDISPATCH GetPivotCell();
    void Dirty();
    LPDISPATCH GetErrors();
    LPDISPATCH GetSmartTags();
    void Speak(const VARIANT& SpeakDirection, const VARIANT& SpeakFormulas);
    VARIANT PasteSpecial(long Paste, long Operation, const VARIANT& SkipBlanks, const VARIANT& Transpose);
    BOOL GetAllowEdit();
    LPDISPATCH GetListObject();
    LPDISPATCH GetXPath();
};
/////////////////////////////////////////////////////////////////////////////
// ChartEvents wrapper class

class ChartEvents : public COleDispatchDriver
{
public:
    ChartEvents() {}        // Calls COleDispatchDriver default constructor
    ChartEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartEvents(const ChartEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void Activate();
    void Deactivate();
    void Resize();
    void MouseDown(long Button, long Shift, long x, long y);
    void MouseUp(long Button, long Shift, long x, long y);
    void MouseMove(long Button, long Shift, long x, long y);
    void BeforeRightClick(BOOL* Cancel);
    void DragPlot();
    void DragOver();
    void BeforeDoubleClick(long ElementID, long Arg1, long Arg2, BOOL* Cancel);
    void Select(long ElementID, long Arg1, long Arg2);
    void SeriesChange(long SeriesIndex, long PointIndex);
    void Calculate();
};
/////////////////////////////////////////////////////////////////////////////
// VPageBreak wrapper class

class VPageBreak : public COleDispatchDriver
{
public:
    VPageBreak() {}     // Calls COleDispatchDriver default constructor
    VPageBreak(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    VPageBreak(const VPageBreak& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    void DragOff(long Direction, long RegionIndex);
    long GetType();
    void SetType(long nNewValue);
    long GetExtent();
    LPDISPATCH GetLocation();
    void SetRefLocation(LPDISPATCH newValue);
};
/////////////////////////////////////////////////////////////////////////////
// HPageBreak wrapper class

class HPageBreak : public COleDispatchDriver
{
public:
    HPageBreak() {}     // Calls COleDispatchDriver default constructor
    HPageBreak(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    HPageBreak(const HPageBreak& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    void DragOff(long Direction, long RegionIndex);
    long GetType();
    void SetType(long nNewValue);
    long GetExtent();
    LPDISPATCH GetLocation();
    void SetRefLocation(LPDISPATCH newValue);
};
/////////////////////////////////////////////////////////////////////////////
// HPageBreaks wrapper class

class HPageBreaks : public COleDispatchDriver
{
public:
    HPageBreaks() {}        // Calls COleDispatchDriver default constructor
    HPageBreaks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    HPageBreaks(const HPageBreaks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Add(LPDISPATCH Before);
};
/////////////////////////////////////////////////////////////////////////////
// VPageBreaks wrapper class

class VPageBreaks : public COleDispatchDriver
{
public:
    VPageBreaks() {}        // Calls COleDispatchDriver default constructor
    VPageBreaks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    VPageBreaks(const VPageBreaks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Add(LPDISPATCH Before);
};
/////////////////////////////////////////////////////////////////////////////
// RecentFile wrapper class

class RecentFile : public COleDispatchDriver
{
public:
    RecentFile() {}     // Calls COleDispatchDriver default constructor
    RecentFile(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    RecentFile(const RecentFile& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    CString GetPath();
    long GetIndex();
    LPDISPATCH Open();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// RecentFiles wrapper class

class RecentFiles : public COleDispatchDriver
{
public:
    RecentFiles() {}        // Calls COleDispatchDriver default constructor
    RecentFiles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    RecentFiles(const RecentFiles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetMaximum();
    void SetMaximum(long nNewValue);
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPDISPATCH Get_Default(long Index);
    LPDISPATCH Add(LPCTSTR Name);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// DocEvents wrapper class

class DocEvents : public COleDispatchDriver
{
public:
    DocEvents() {}      // Calls COleDispatchDriver default constructor
    DocEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DocEvents(const DocEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void SelectionChange(LPDISPATCH Target);
    void BeforeDoubleClick(LPDISPATCH Target, BOOL* Cancel);
    void BeforeRightClick(LPDISPATCH Target, BOOL* Cancel);
    void Activate();
    void Deactivate();
    void Calculate();
    void Change(LPDISPATCH Target);
    void FollowHyperlink(LPDISPATCH Target);
    void PivotTableUpdate(LPDISPATCH Target);
};
/////////////////////////////////////////////////////////////////////////////
// Style wrapper class

class Style : public COleDispatchDriver
{
public:
    Style() {}      // Calls COleDispatchDriver default constructor
    Style(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Style(const Style& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    BOOL GetBuiltIn();
    LPDISPATCH GetBorders();
    VARIANT Delete();
    LPDISPATCH GetFont();
    BOOL GetFormulaHidden();
    void SetFormulaHidden(BOOL bNewValue);
    long GetHorizontalAlignment();
    void SetHorizontalAlignment(long nNewValue);
    BOOL GetIncludeAlignment();
    void SetIncludeAlignment(BOOL bNewValue);
    BOOL GetIncludeBorder();
    void SetIncludeBorder(BOOL bNewValue);
    BOOL GetIncludeFont();
    void SetIncludeFont(BOOL bNewValue);
    BOOL GetIncludeNumber();
    void SetIncludeNumber(BOOL bNewValue);
    BOOL GetIncludePatterns();
    void SetIncludePatterns(BOOL bNewValue);
    BOOL GetIncludeProtection();
    void SetIncludeProtection(BOOL bNewValue);
    long GetIndentLevel();
    void SetIndentLevel(long nNewValue);
    LPDISPATCH GetInterior();
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetMergeCells();
    void SetMergeCells(const VARIANT& newValue);
    CString GetName();
    CString GetNameLocal();
    CString GetNumberFormat();
    void SetNumberFormat(LPCTSTR lpszNewValue);
    CString GetNumberFormatLocal();
    void SetNumberFormatLocal(LPCTSTR lpszNewValue);
    long GetOrientation();
    void SetOrientation(long nNewValue);
    BOOL GetShrinkToFit();
    void SetShrinkToFit(BOOL bNewValue);
    CString GetValue();
    long GetVerticalAlignment();
    void SetVerticalAlignment(long nNewValue);
    BOOL GetWrapText();
    void SetWrapText(BOOL bNewValue);
    CString Get_Default();
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Styles wrapper class

class Styles : public COleDispatchDriver
{
public:
    Styles() {}     // Calls COleDispatchDriver default constructor
    Styles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Styles(const Styles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Name, const VARIANT& BasedOn);
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    VARIANT Merge(const VARIANT& Workbook);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Borders wrapper class

class Borders : public COleDispatchDriver
{
public:
    Borders() {}        // Calls COleDispatchDriver default constructor
    Borders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Borders(const Borders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetColor();
    void SetColor(const VARIANT& newValue);
    VARIANT GetColorIndex();
    void SetColorIndex(const VARIANT& newValue);
    long GetCount();
    LPDISPATCH GetItem(long Index);
    VARIANT GetLineStyle();
    void SetLineStyle(const VARIANT& newValue);
    LPUNKNOWN Get_NewEnum();
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    VARIANT GetWeight();
    void SetWeight(const VARIANT& newValue);
    LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// AddIn wrapper class

class AddIn : public COleDispatchDriver
{
public:
    AddIn() {}      // Calls COleDispatchDriver default constructor
    AddIn(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AddIn(const AddIn& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetFullName();
    BOOL GetInstalled();
    void SetInstalled(BOOL bNewValue);
    CString GetName();
    CString GetPath();
    CString GetProgID();
    CString GetClsid();
};
/////////////////////////////////////////////////////////////////////////////
// AddIns wrapper class

class AddIns : public COleDispatchDriver
{
public:
    AddIns() {}     // Calls COleDispatchDriver default constructor
    AddIns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AddIns(const AddIns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Filename, const VARIANT& CopyFile);
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Toolbar wrapper class

class Toolbar : public COleDispatchDriver
{
public:
    Toolbar() {}        // Calls COleDispatchDriver default constructor
    Toolbar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Toolbar(const Toolbar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetBuiltIn();
    void Delete();
    long GetHeight();
    void SetHeight(long nNewValue);
    long GetLeft();
    void SetLeft(long nNewValue);
    CString GetName();
    long GetPosition();
    void SetPosition(long nNewValue);
    long GetProtection();
    void SetProtection(long nNewValue);
    void Reset();
    LPDISPATCH GetToolbarButtons();
    long GetTop();
    void SetTop(long nNewValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    long GetWidth();
    void SetWidth(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Toolbars wrapper class

class Toolbars : public COleDispatchDriver
{
public:
    Toolbars() {}       // Calls COleDispatchDriver default constructor
    Toolbars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Toolbars(const Toolbars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Name);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ToolbarButton wrapper class

class ToolbarButton : public COleDispatchDriver
{
public:
    ToolbarButton() {}      // Calls COleDispatchDriver default constructor
    ToolbarButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ToolbarButton(const ToolbarButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetBuiltIn();
    BOOL GetBuiltInFace();
    void SetBuiltInFace(BOOL bNewValue);
    void Copy(LPDISPATCH Toolbar, long Before);
    void CopyFace();
    void Delete();
    void Edit();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    long GetHelpContextID();
    void SetHelpContextID(long nNewValue);
    CString GetHelpFile();
    void SetHelpFile(LPCTSTR lpszNewValue);
    long GetId();
    BOOL GetIsGap();
    void Move(LPDISPATCH Toolbar, long Before);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetOnAction();
    void SetOnAction(LPCTSTR lpszNewValue);
    void PasteFace();
    BOOL GetPushed();
    void SetPushed(BOOL bNewValue);
    void Reset();
    CString GetStatusBar();
    void SetStatusBar(LPCTSTR lpszNewValue);
    long GetWidth();
    void SetWidth(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ToolbarButtons wrapper class

class ToolbarButtons : public COleDispatchDriver
{
public:
    ToolbarButtons() {}     // Calls COleDispatchDriver default constructor
    ToolbarButtons(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ToolbarButtons(const ToolbarButtons& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Button, const VARIANT& Before, const VARIANT& OnAction, const VARIANT& Pushed, const VARIANT& Enabled, const VARIANT& StatusBar, const VARIANT& HelpFile, const VARIANT& HelpContextID);
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Areas wrapper class

class Areas : public COleDispatchDriver
{
public:
    Areas() {}      // Calls COleDispatchDriver default constructor
    Areas(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Areas(const Areas& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// WorkbookEvents wrapper class

class WorkbookEvents : public COleDispatchDriver
{
public:
    WorkbookEvents() {}     // Calls COleDispatchDriver default constructor
    WorkbookEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    WorkbookEvents(const WorkbookEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void Open();
    void Activate();
    void Deactivate();
    void BeforeClose(BOOL* Cancel);
    void BeforeSave(BOOL SaveAsUI, BOOL* Cancel);
    void BeforePrint(BOOL* Cancel);
    void NewSheet(LPDISPATCH Sh);
    void AddinInstall();
    void AddinUninstall();
    void WindowResize(LPDISPATCH Wn);
    void WindowActivate(LPDISPATCH Wn);
    void WindowDeactivate(LPDISPATCH Wn);
    void SheetSelectionChange(LPDISPATCH Sh, LPDISPATCH Target);
    void SheetBeforeDoubleClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
    void SheetBeforeRightClick(LPDISPATCH Sh, LPDISPATCH Target, BOOL* Cancel);
    void SheetActivate(LPDISPATCH Sh);
    void SheetDeactivate(LPDISPATCH Sh);
    void SheetCalculate(LPDISPATCH Sh);
    void SheetChange(LPDISPATCH Sh, LPDISPATCH Target);
    void SheetFollowHyperlink(LPDISPATCH Sh, LPDISPATCH Target);
    void SheetPivotTableUpdate(LPDISPATCH Sh, LPDISPATCH Target);
    void PivotTableCloseConnection(LPDISPATCH Target);
    void PivotTableOpenConnection(LPDISPATCH Target);
    void Sync(long SyncEventType);
    void BeforeXmlImport(LPDISPATCH Map, LPCTSTR Url, BOOL IsRefresh, BOOL* Cancel);
    void AfterXmlImport(LPDISPATCH Map, BOOL IsRefresh, long Result);
    void BeforeXmlExport(LPDISPATCH Map, LPCTSTR Url, BOOL* Cancel);
    void AfterXmlExport(LPDISPATCH Map, LPCTSTR Url, long Result);
};
/////////////////////////////////////////////////////////////////////////////
// MenuBars wrapper class

class MenuBars : public COleDispatchDriver
{
public:
    MenuBars() {}       // Calls COleDispatchDriver default constructor
    MenuBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    MenuBars(const MenuBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Name);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// MenuBar wrapper class

class MenuBar : public COleDispatchDriver
{
public:
    MenuBar() {}        // Calls COleDispatchDriver default constructor
    MenuBar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    MenuBar(const MenuBar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Activate();
    BOOL GetBuiltIn();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    void Delete();
    long GetIndex();
    LPDISPATCH GetMenus();
    void Reset();
};
/////////////////////////////////////////////////////////////////////////////
// Menus wrapper class

class Menus : public COleDispatchDriver
{
public:
    Menus() {}      // Calls COleDispatchDriver default constructor
    Menus(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Menus(const Menus& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Caption, const VARIANT& Before, const VARIANT& Restore);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Menu wrapper class

class Menu : public COleDispatchDriver
{
public:
    Menu() {}       // Calls COleDispatchDriver default constructor
    Menu(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Menu(const Menu& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    void Delete();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    long GetIndex();
    LPDISPATCH GetMenuItems();
};
/////////////////////////////////////////////////////////////////////////////
// MenuItems wrapper class

class MenuItems : public COleDispatchDriver
{
public:
    MenuItems() {}      // Calls COleDispatchDriver default constructor
    MenuItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    MenuItems(const MenuItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Caption, const VARIANT& OnAction, const VARIANT& ShortcutKey, const VARIANT& Before, const VARIANT& Restore, const VARIANT& StatusBar, const VARIANT& HelpFile, const VARIANT& HelpContextID);
    LPDISPATCH AddMenu(LPCTSTR Caption, const VARIANT& Before, const VARIANT& Restore);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// MenuItem wrapper class

class MenuItem : public COleDispatchDriver
{
public:
    MenuItem() {}       // Calls COleDispatchDriver default constructor
    MenuItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    MenuItem(const MenuItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    BOOL GetChecked();
    void SetChecked(BOOL bNewValue);
    void Delete();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    long GetHelpContextID();
    void SetHelpContextID(long nNewValue);
    CString GetHelpFile();
    void SetHelpFile(LPCTSTR lpszNewValue);
    long GetIndex();
    CString GetOnAction();
    void SetOnAction(LPCTSTR lpszNewValue);
    CString GetStatusBar();
    void SetStatusBar(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Charts wrapper class

class Charts : public COleDispatchDriver
{
public:
    Charts() {}     // Calls COleDispatchDriver default constructor
    Charts(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Charts(const Charts& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count);
    void Copy(const VARIANT& Before, const VARIANT& After);
    long GetCount();
    void Delete();
    LPDISPATCH GetItem(const VARIANT& Index);
    void Move(const VARIANT& Before, const VARIANT& After);
    LPUNKNOWN Get_NewEnum();
    void PrintPreview(const VARIANT& EnableChanges);
    void Select(const VARIANT& Replace);
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    VARIANT GetVisible();
    void SetVisible(const VARIANT& newValue);
    LPDISPATCH Get_Default(const VARIANT& Index);
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// DrawingObjects wrapper class

class DrawingObjects : public COleDispatchDriver
{
public:
    DrawingObjects() {}     // Calls COleDispatchDriver default constructor
    DrawingObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DrawingObjects(const DrawingObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
    VARIANT GetArrowHeadLength();
    void SetArrowHeadLength(const VARIANT& newValue);
    VARIANT GetArrowHeadStyle();
    void SetArrowHeadStyle(const VARIANT& newValue);
    VARIANT GetArrowHeadWidth();
    void SetArrowHeadWidth(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    LPDISPATCH GetBorder();
    BOOL GetCancelButton();
    void SetCancelButton(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDefaultButton();
    void SetDefaultButton(BOOL bNewValue);
    BOOL GetDismissButton();
    void SetDismissButton(BOOL bNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    BOOL GetDisplayVerticalScrollBar();
    void SetDisplayVerticalScrollBar(BOOL bNewValue);
    long GetDropDownLines();
    void SetDropDownLines(long nNewValue);
    LPDISPATCH GetFont();
    BOOL GetHelpButton();
    void SetHelpButton(BOOL bNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    long GetInputType();
    void SetInputType(long nNewValue);
    LPDISPATCH GetInterior();
    long GetLargeChange();
    void SetLargeChange(long nNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT List(const VARIANT& Index);
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    long GetListIndex();
    void SetListIndex(long nNewValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    long GetMax();
    void SetMax(long nNewValue);
    long GetMin();
    void SetMin(long nNewValue);
    BOOL GetMultiLine();
    void SetMultiLine(BOOL bNewValue);
    BOOL GetMultiSelect();
    void SetMultiSelect(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    VARIANT RemoveAllItems();
    VARIANT RemoveItem(long Index, const VARIANT& Count);
    VARIANT Reshape(long Vertex, const VARIANT& Insert, const VARIANT& Left, const VARIANT& Top);
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    VARIANT Selected(const VARIANT& Index);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    long GetSmallChange();
    void SetSmallChange(long nNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    LPDISPATCH Ungroup();
    long GetValue();
    void SetValue(long nNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    VARIANT Vertices(const VARIANT& Index1, const VARIANT& Index2);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Group();
    VARIANT LinkCombo(const VARIANT& Link);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// PivotCache wrapper class

class PivotCache : public COleDispatchDriver
{
public:
    PivotCache() {}     // Calls COleDispatchDriver default constructor
    PivotCache(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotCache(const PivotCache& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetBackgroundQuery();
    void SetBackgroundQuery(BOOL bNewValue);
    VARIANT GetConnection();
    void SetConnection(const VARIANT& newValue);
    BOOL GetEnableRefresh();
    void SetEnableRefresh(BOOL bNewValue);
    long GetIndex();
    long GetMemoryUsed();
    BOOL GetOptimizeCache();
    void SetOptimizeCache(BOOL bNewValue);
    long GetRecordCount();
    void Refresh();
    DATE GetRefreshDate();
    CString GetRefreshName();
    BOOL GetRefreshOnFileOpen();
    void SetRefreshOnFileOpen(BOOL bNewValue);
    BOOL GetSavePassword();
    void SetSavePassword(BOOL bNewValue);
    VARIANT GetSourceData();
    void SetSourceData(const VARIANT& newValue);
    VARIANT GetCommandText();
    void SetCommandText(const VARIANT& newValue);
    long GetCommandType();
    void SetCommandType(long nNewValue);
    long GetQueryType();
    BOOL GetMaintainConnection();
    void SetMaintainConnection(BOOL bNewValue);
    long GetRefreshPeriod();
    void SetRefreshPeriod(long nNewValue);
    LPDISPATCH GetRecordset();
    void SetRefRecordset(LPDISPATCH newValue);
    void ResetTimer();
    VARIANT GetLocalConnection();
    void SetLocalConnection(const VARIANT& newValue);
    LPDISPATCH CreatePivotTable(const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& ReadData, const VARIANT& DefaultVersion);
    BOOL GetUseLocalConnection();
    void SetUseLocalConnection(BOOL bNewValue);
    LPDISPATCH GetADOConnection();
    BOOL GetIsConnected();
    void MakeConnection();
    BOOL GetOlap();
    long GetSourceType();
    long GetMissingItemsLimit();
    void SetMissingItemsLimit(long nNewValue);
    CString GetSourceConnectionFile();
    void SetSourceConnectionFile(LPCTSTR lpszNewValue);
    CString GetSourceDataFile();
    long GetRobustConnect();
    void SetRobustConnect(long nNewValue);
    void SaveAsODC(LPCTSTR ODCFileName, const VARIANT& Description, const VARIANT& Keywords);
};
/////////////////////////////////////////////////////////////////////////////
// PivotCaches wrapper class

class PivotCaches : public COleDispatchDriver
{
public:
    PivotCaches() {}        // Calls COleDispatchDriver default constructor
    PivotCaches(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotCaches(const PivotCaches& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
    LPDISPATCH Add(long SourceType, const VARIANT& SourceData);
};
/////////////////////////////////////////////////////////////////////////////
// PivotFormula wrapper class

class PivotFormula : public COleDispatchDriver
{
public:
    PivotFormula() {}       // Calls COleDispatchDriver default constructor
    PivotFormula(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotFormula(const PivotFormula& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    CString Get_Default();
    void Set_Default(LPCTSTR lpszNewValue);
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    CString GetValue();
    void SetValue(LPCTSTR lpszNewValue);
    long GetIndex();
    void SetIndex(long nNewValue);
    CString GetStandardFormula();
    void SetStandardFormula(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotFormulas wrapper class

class PivotFormulas : public COleDispatchDriver
{
public:
    PivotFormulas() {}      // Calls COleDispatchDriver default constructor
    PivotFormulas(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotFormulas(const PivotFormulas& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
    LPDISPATCH Add(LPCTSTR Formula, const VARIANT& UseStandardFormula);
};
/////////////////////////////////////////////////////////////////////////////
// PivotTable wrapper class

class PivotTable : public COleDispatchDriver
{
public:
    PivotTable() {}     // Calls COleDispatchDriver default constructor
    PivotTable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotTable(const PivotTable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT AddFields(const VARIANT& RowFields, const VARIANT& ColumnFields, const VARIANT& PageFields, const VARIANT& AddToTable);
    LPDISPATCH GetColumnFields(const VARIANT& Index);
    BOOL GetColumnGrand();
    void SetColumnGrand(BOOL bNewValue);
    LPDISPATCH GetColumnRange();
    VARIANT ShowPages(const VARIANT& PageField);
    LPDISPATCH GetDataBodyRange();
    LPDISPATCH GetDataFields(const VARIANT& Index);
    LPDISPATCH GetDataLabelRange();
    CString Get_Default();
    void Set_Default(LPCTSTR lpszNewValue);
    BOOL GetHasAutoFormat();
    void SetHasAutoFormat(BOOL bNewValue);
    LPDISPATCH GetHiddenFields(const VARIANT& Index);
    CString GetInnerDetail();
    void SetInnerDetail(LPCTSTR lpszNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetPageFields(const VARIANT& Index);
    LPDISPATCH GetPageRange();
    LPDISPATCH GetPageRangeCells();
    LPDISPATCH PivotFields(const VARIANT& Index);
    DATE GetRefreshDate();
    CString GetRefreshName();
    BOOL RefreshTable();
    LPDISPATCH GetRowFields(const VARIANT& Index);
    BOOL GetRowGrand();
    void SetRowGrand(BOOL bNewValue);
    LPDISPATCH GetRowRange();
    BOOL GetSaveData();
    void SetSaveData(BOOL bNewValue);
    VARIANT GetSourceData();
    void SetSourceData(const VARIANT& newValue);
    LPDISPATCH GetTableRange1();
    LPDISPATCH GetTableRange2();
    CString GetValue();
    void SetValue(LPCTSTR lpszNewValue);
    LPDISPATCH GetVisibleFields(const VARIANT& Index);
    long GetCacheIndex();
    void SetCacheIndex(long nNewValue);
    LPDISPATCH CalculatedFields();
    BOOL GetDisplayErrorString();
    void SetDisplayErrorString(BOOL bNewValue);
    BOOL GetDisplayNullString();
    void SetDisplayNullString(BOOL bNewValue);
    BOOL GetEnableDrilldown();
    void SetEnableDrilldown(BOOL bNewValue);
    BOOL GetEnableFieldDialog();
    void SetEnableFieldDialog(BOOL bNewValue);
    BOOL GetEnableWizard();
    void SetEnableWizard(BOOL bNewValue);
    CString GetErrorString();
    void SetErrorString(LPCTSTR lpszNewValue);
    double GetData(LPCTSTR Name);
    void ListFormulas();
    BOOL GetManualUpdate();
    void SetManualUpdate(BOOL bNewValue);
    BOOL GetMergeLabels();
    void SetMergeLabels(BOOL bNewValue);
    CString GetNullString();
    void SetNullString(LPCTSTR lpszNewValue);
    LPDISPATCH PivotCache();
    LPDISPATCH GetPivotFormulas();
    void PivotTableWizard(const VARIANT& SourceType, const VARIANT& SourceData, const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& RowGrand, const VARIANT& ColumnGrand, const VARIANT& SaveData, const VARIANT& HasAutoFormat,
                          const VARIANT& AutoPage, const VARIANT& Reserved, const VARIANT& BackgroundQuery, const VARIANT& OptimizeCache, const VARIANT& PageFieldOrder, const VARIANT& PageFieldWrapCount, const VARIANT& ReadData, const VARIANT& Connection);
    BOOL GetSubtotalHiddenPageItems();
    void SetSubtotalHiddenPageItems(BOOL bNewValue);
    long GetPageFieldOrder();
    void SetPageFieldOrder(long nNewValue);
    CString GetPageFieldStyle();
    void SetPageFieldStyle(LPCTSTR lpszNewValue);
    long GetPageFieldWrapCount();
    void SetPageFieldWrapCount(long nNewValue);
    BOOL GetPreserveFormatting();
    void SetPreserveFormatting(BOOL bNewValue);
    CString GetPivotSelection();
    void SetPivotSelection(LPCTSTR lpszNewValue);
    long GetSelectionMode();
    void SetSelectionMode(long nNewValue);
    CString GetTableStyle();
    void SetTableStyle(LPCTSTR lpszNewValue);
    CString GetTag();
    void SetTag(LPCTSTR lpszNewValue);
    void Update();
    CString GetVacatedStyle();
    void SetVacatedStyle(LPCTSTR lpszNewValue);
    void Format(long Format);
    BOOL GetPrintTitles();
    void SetPrintTitles(BOOL bNewValue);
    LPDISPATCH GetCubeFields();
    CString GetGrandTotalName();
    void SetGrandTotalName(LPCTSTR lpszNewValue);
    BOOL GetSmallGrid();
    void SetSmallGrid(BOOL bNewValue);
    BOOL GetRepeatItemsOnEachPrintedPage();
    void SetRepeatItemsOnEachPrintedPage(BOOL bNewValue);
    BOOL GetTotalsAnnotation();
    void SetTotalsAnnotation(BOOL bNewValue);
    void PivotSelect(LPCTSTR Name, long Mode, const VARIANT& UseStandardName);
    CString GetPivotSelectionStandard();
    void SetPivotSelectionStandard(LPCTSTR lpszNewValue);
    LPDISPATCH GetPivotData(const VARIANT& DataField, const VARIANT& Field1, const VARIANT& Item1, const VARIANT& Field2, const VARIANT& Item2, const VARIANT& Field3, const VARIANT& Item3, const VARIANT& Field4, const VARIANT& Item4,
                            const VARIANT& Field5, const VARIANT& Item5, const VARIANT& Field6, const VARIANT& Item6, const VARIANT& Field7, const VARIANT& Item7, const VARIANT& Field8, const VARIANT& Item8, const VARIANT& Field9, const VARIANT& Item9,
                            const VARIANT& Field10, const VARIANT& Item10, const VARIANT& Field11, const VARIANT& Item11, const VARIANT& Field12, const VARIANT& Item12, const VARIANT& Field13, const VARIANT& Item13, const VARIANT& Field14, const VARIANT& Item14);
    LPDISPATCH GetDataPivotField();
    BOOL GetEnableDataValueEditing();
    void SetEnableDataValueEditing(BOOL bNewValue);
    LPDISPATCH AddDataField(LPDISPATCH Field, const VARIANT& Caption, const VARIANT& Function);
    CString GetMdx();
    BOOL GetViewCalculatedMembers();
    void SetViewCalculatedMembers(BOOL bNewValue);
    LPDISPATCH GetCalculatedMembers();
    BOOL GetDisplayImmediateItems();
    void SetDisplayImmediateItems(BOOL bNewValue);
    BOOL GetEnableFieldList();
    void SetEnableFieldList(BOOL bNewValue);
    BOOL GetVisualTotals();
    void SetVisualTotals(BOOL bNewValue);
    BOOL GetShowPageMultipleItemLabel();
    void SetShowPageMultipleItemLabel(BOOL bNewValue);
    long GetVersion();
    CString CreateCubeFile(LPCTSTR File, const VARIANT& Measures, const VARIANT& Levels, const VARIANT& Members, const VARIANT& Properties);
    BOOL GetDisplayEmptyRow();
    void SetDisplayEmptyRow(BOOL bNewValue);
    BOOL GetDisplayEmptyColumn();
    void SetDisplayEmptyColumn(BOOL bNewValue);
    BOOL GetShowCellBackgroundFromOLAP();
    void SetShowCellBackgroundFromOLAP(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PivotTables wrapper class

class PivotTables : public COleDispatchDriver
{
public:
    PivotTables() {}        // Calls COleDispatchDriver default constructor
    PivotTables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotTables(const PivotTables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
    LPDISPATCH Add(LPDISPATCH PivotCache, const VARIANT& TableDestination, const VARIANT& TableName, const VARIANT& ReadData, const VARIANT& DefaultVersion);
};
/////////////////////////////////////////////////////////////////////////////
// PivotField wrapper class

class PivotField : public COleDispatchDriver
{
public:
    PivotField() {}     // Calls COleDispatchDriver default constructor
    PivotField(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotField(const PivotField& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCalculation();
    void SetCalculation(long nNewValue);
    LPDISPATCH GetChildField();
    VARIANT GetChildItems(const VARIANT& Index);
    VARIANT GetCurrentPage();
    void SetCurrentPage(const VARIANT& newValue);
    LPDISPATCH GetDataRange();
    long GetDataType();
    CString Get_Default();
    void Set_Default(LPCTSTR lpszNewValue);
    long GetFunction();
    void SetFunction(long nNewValue);
    VARIANT GetGroupLevel();
    VARIANT GetHiddenItems(const VARIANT& Index);
    LPDISPATCH GetLabelRange();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    CString GetNumberFormat();
    void SetNumberFormat(LPCTSTR lpszNewValue);
    long GetOrientation();
    void SetOrientation(long nNewValue);
    BOOL GetShowAllItems();
    void SetShowAllItems(BOOL bNewValue);
    LPDISPATCH GetParentField();
    VARIANT GetParentItems(const VARIANT& Index);
    VARIANT PivotItems(const VARIANT& Index);
    VARIANT GetPosition();
    void SetPosition(const VARIANT& newValue);
    CString GetSourceName();
    VARIANT GetSubtotals(const VARIANT& Index);
    void SetSubtotals(const VARIANT& Index, const VARIANT& newValue);
    VARIANT GetBaseField();
    void SetBaseField(const VARIANT& newValue);
    VARIANT GetBaseItem();
    void SetBaseItem(const VARIANT& newValue);
    VARIANT GetTotalLevels();
    CString GetValue();
    void SetValue(LPCTSTR lpszNewValue);
    VARIANT GetVisibleItems(const VARIANT& Index);
    LPDISPATCH CalculatedItems();
    void Delete();
    BOOL GetDragToColumn();
    void SetDragToColumn(BOOL bNewValue);
    BOOL GetDragToHide();
    void SetDragToHide(BOOL bNewValue);
    BOOL GetDragToPage();
    void SetDragToPage(BOOL bNewValue);
    BOOL GetDragToRow();
    void SetDragToRow(BOOL bNewValue);
    BOOL GetDragToData();
    void SetDragToData(BOOL bNewValue);
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    BOOL GetIsCalculated();
    long GetMemoryUsed();
    BOOL GetServerBased();
    void SetServerBased(BOOL bNewValue);
    void AutoSort(long Order, LPCTSTR Field);
    void AutoShow(long Type, long Range, long Count, LPCTSTR Field);
    long GetAutoSortOrder();
    CString GetAutoSortField();
    long GetAutoShowType();
    long GetAutoShowRange();
    long GetAutoShowCount();
    CString GetAutoShowField();
    BOOL GetLayoutBlankLine();
    void SetLayoutBlankLine(BOOL bNewValue);
    long GetLayoutSubtotalLocation();
    void SetLayoutSubtotalLocation(long nNewValue);
    BOOL GetLayoutPageBreak();
    void SetLayoutPageBreak(BOOL bNewValue);
    long GetLayoutForm();
    void SetLayoutForm(long nNewValue);
    CString GetSubtotalName();
    void SetSubtotalName(LPCTSTR lpszNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    BOOL GetDrilledDown();
    void SetDrilledDown(BOOL bNewValue);
    LPDISPATCH GetCubeField();
    CString GetCurrentPageName();
    void SetCurrentPageName(LPCTSTR lpszNewValue);
    CString GetStandardFormula();
    void SetStandardFormula(LPCTSTR lpszNewValue);
    VARIANT GetHiddenItemsList();
    void SetHiddenItemsList(const VARIANT& newValue);
    BOOL GetDatabaseSort();
    void SetDatabaseSort(BOOL bNewValue);
    BOOL GetIsMemberProperty();
    LPDISPATCH GetPropertyParentField();
    long GetPropertyOrder();
    void SetPropertyOrder(long nNewValue);
    BOOL GetEnableItemSelection();
    void SetEnableItemSelection(BOOL bNewValue);
    VARIANT GetCurrentPageList();
    void SetCurrentPageList(const VARIANT& newValue);
    void AddPageItem(LPCTSTR Item, const VARIANT& ClearList);
};
/////////////////////////////////////////////////////////////////////////////
// PivotFields wrapper class

class PivotFields : public COleDispatchDriver
{
public:
    PivotFields() {}        // Calls COleDispatchDriver default constructor
    PivotFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotFields(const PivotFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CalculatedFields wrapper class

class CalculatedFields : public COleDispatchDriver
{
public:
    CalculatedFields() {}       // Calls COleDispatchDriver default constructor
    CalculatedFields(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CalculatedFields(const CalculatedFields& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Field);
    LPUNKNOWN _NewEnum();
    LPDISPATCH Add(LPCTSTR Name, LPCTSTR Formula, const VARIANT& UseStandardFormula);
};
/////////////////////////////////////////////////////////////////////////////
// PivotItem wrapper class

class PivotItem : public COleDispatchDriver
{
public:
    PivotItem() {}      // Calls COleDispatchDriver default constructor
    PivotItem(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotItem(const PivotItem& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetChildItems(const VARIANT& Index);
    LPDISPATCH GetDataRange();
    CString Get_Default();
    void Set_Default(LPCTSTR lpszNewValue);
    LPDISPATCH GetLabelRange();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetParentItem();
    BOOL GetParentShowDetail();
    long GetPosition();
    void SetPosition(long nNewValue);
    BOOL GetShowDetail();
    void SetShowDetail(BOOL bNewValue);
    VARIANT GetSourceName();
    CString GetValue();
    void SetValue(LPCTSTR lpszNewValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    void Delete();
    BOOL GetIsCalculated();
    long GetRecordCount();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    BOOL GetDrilledDown();
    void SetDrilledDown(BOOL bNewValue);
    CString GetStandardFormula();
    void SetStandardFormula(LPCTSTR lpszNewValue);
    CString GetSourceNameStandard();
};
/////////////////////////////////////////////////////////////////////////////
// PivotItems wrapper class

class PivotItems : public COleDispatchDriver
{
public:
    PivotItems() {}     // Calls COleDispatchDriver default constructor
    PivotItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotItems(const PivotItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Add(LPCTSTR Name);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CalculatedItems wrapper class

class CalculatedItems : public COleDispatchDriver
{
public:
    CalculatedItems() {}        // Calls COleDispatchDriver default constructor
    CalculatedItems(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CalculatedItems(const CalculatedItems& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Field);
    LPUNKNOWN _NewEnum();
    LPDISPATCH Add(LPCTSTR Name, LPCTSTR Formula, const VARIANT& UseStandardFormula);
};
/////////////////////////////////////////////////////////////////////////////
// Characters wrapper class

class Characters : public COleDispatchDriver
{
public:
    Characters() {}     // Calls COleDispatchDriver default constructor
    Characters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Characters(const Characters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    long GetCount();
    VARIANT Delete();
    LPDISPATCH GetFont();
    VARIANT Insert(LPCTSTR String);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    CString GetPhoneticCharacters();
    void SetPhoneticCharacters(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Dialogs wrapper class

class Dialogs : public COleDispatchDriver
{
public:
    Dialogs() {}        // Calls COleDispatchDriver default constructor
    Dialogs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Dialogs(const Dialogs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Dialog wrapper class

class Dialog : public COleDispatchDriver
{
public:
    Dialog() {}     // Calls COleDispatchDriver default constructor
    Dialog(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Dialog(const Dialog& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL Show(const VARIANT& Arg1, const VARIANT& Arg2, const VARIANT& Arg3, const VARIANT& Arg4, const VARIANT& Arg5, const VARIANT& Arg6, const VARIANT& Arg7, const VARIANT& Arg8, const VARIANT& Arg9, const VARIANT& Arg10, const VARIANT& Arg11,
              const VARIANT& Arg12, const VARIANT& Arg13, const VARIANT& Arg14, const VARIANT& Arg15, const VARIANT& Arg16, const VARIANT& Arg17, const VARIANT& Arg18, const VARIANT& Arg19, const VARIANT& Arg20, const VARIANT& Arg21,
              const VARIANT& Arg22, const VARIANT& Arg23, const VARIANT& Arg24, const VARIANT& Arg25, const VARIANT& Arg26, const VARIANT& Arg27, const VARIANT& Arg28, const VARIANT& Arg29, const VARIANT& Arg30);
};
/////////////////////////////////////////////////////////////////////////////
// SoundNote wrapper class

class SoundNote : public COleDispatchDriver
{
public:
    SoundNote() {}      // Calls COleDispatchDriver default constructor
    SoundNote(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SoundNote(const SoundNote& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT Delete();
    VARIANT Import(LPCTSTR Filename);
    VARIANT Play();
    VARIANT Record();
};
/////////////////////////////////////////////////////////////////////////////
// Button wrapper class

class Button : public COleDispatchDriver
{
public:
    Button() {}     // Calls COleDispatchDriver default constructor
    Button(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Button(const Button& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    BOOL GetCancelButton();
    void SetCancelButton(BOOL bNewValue);
    BOOL GetDefaultButton();
    void SetDefaultButton(BOOL bNewValue);
    BOOL GetDismissButton();
    void SetDismissButton(BOOL bNewValue);
    BOOL GetHelpButton();
    void SetHelpButton(BOOL bNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Buttons wrapper class

class Buttons : public COleDispatchDriver
{
public:
    Buttons() {}        // Calls COleDispatchDriver default constructor
    Buttons(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Buttons(const Buttons& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    BOOL GetCancelButton();
    void SetCancelButton(BOOL bNewValue);
    BOOL GetDefaultButton();
    void SetDefaultButton(BOOL bNewValue);
    BOOL GetDismissButton();
    void SetDismissButton(BOOL bNewValue);
    BOOL GetHelpButton();
    void SetHelpButton(BOOL bNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CheckBox wrapper class

class CheckBox : public COleDispatchDriver
{
public:
    CheckBox() {}       // Calls COleDispatchDriver default constructor
    CheckBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CheckBox(const CheckBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    LPDISPATCH GetBorder();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    LPDISPATCH GetInterior();
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CheckBoxes wrapper class

class CheckBoxes : public COleDispatchDriver
{
public:
    CheckBoxes() {}     // Calls COleDispatchDriver default constructor
    CheckBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CheckBoxes(const CheckBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    LPDISPATCH GetBorder();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    LPDISPATCH GetInterior();
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// OptionButton wrapper class

class OptionButton : public COleDispatchDriver
{
public:
    OptionButton() {}       // Calls COleDispatchDriver default constructor
    OptionButton(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OptionButton(const OptionButton& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    LPDISPATCH GetBorder();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    LPDISPATCH GetInterior();
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    LPDISPATCH GetGroupBox();
};
/////////////////////////////////////////////////////////////////////////////
// OptionButtons wrapper class

class OptionButtons : public COleDispatchDriver
{
public:
    OptionButtons() {}      // Calls COleDispatchDriver default constructor
    OptionButtons(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OptionButtons(const OptionButtons& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    LPDISPATCH GetBorder();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    LPDISPATCH GetInterior();
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    LPDISPATCH GetGroupBox();
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// EditBox wrapper class

class EditBox : public COleDispatchDriver
{
public:
    EditBox() {}        // Calls COleDispatchDriver default constructor
    EditBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    EditBox(const EditBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    BOOL GetDisplayVerticalScrollBar();
    void SetDisplayVerticalScrollBar(BOOL bNewValue);
    long GetInputType();
    void SetInputType(long nNewValue);
    CString GetLinkedObject();
    BOOL GetMultiLine();
    void SetMultiLine(BOOL bNewValue);
    BOOL GetPasswordEdit();
    void SetPasswordEdit(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// EditBoxes wrapper class

class EditBoxes : public COleDispatchDriver
{
public:
    EditBoxes() {}      // Calls COleDispatchDriver default constructor
    EditBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    EditBoxes(const EditBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    BOOL GetDisplayVerticalScrollBar();
    void SetDisplayVerticalScrollBar(BOOL bNewValue);
    long GetInputType();
    void SetInputType(long nNewValue);
    BOOL GetMultiLine();
    void SetMultiLine(BOOL bNewValue);
    BOOL GetPasswordEdit();
    void SetPasswordEdit(BOOL bNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    VARIANT Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ScrollBar wrapper class

class ScrollBar : public COleDispatchDriver
{
public:
    ScrollBar() {}      // Calls COleDispatchDriver default constructor
    ScrollBar(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ScrollBar(const ScrollBar& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    long GetMax();
    void SetMax(long nNewValue);
    long GetMin();
    void SetMin(long nNewValue);
    long GetSmallChange();
    void SetSmallChange(long nNewValue);
    long GetValue();
    void SetValue(long nNewValue);
    long GetLargeChange();
    void SetLargeChange(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ScrollBars wrapper class

class ScrollBars : public COleDispatchDriver
{
public:
    ScrollBars() {}     // Calls COleDispatchDriver default constructor
    ScrollBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ScrollBars(const ScrollBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    long GetMax();
    void SetMax(long nNewValue);
    long GetMin();
    void SetMin(long nNewValue);
    long GetSmallChange();
    void SetSmallChange(long nNewValue);
    long GetValue();
    void SetValue(long nNewValue);
    long GetLargeChange();
    void SetLargeChange(long nNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ListBox wrapper class

class ListBox : public COleDispatchDriver
{
public:
    ListBox() {}        // Calls COleDispatchDriver default constructor
    ListBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListBox(const ListBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    long Get_Default();
    void Set_Default(long nNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetLinkedObject();
    VARIANT GetList(const VARIANT& Index);
    void SetList(const VARIANT& Index, const VARIANT& newValue);
    long GetListCount();
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    long GetListIndex();
    void SetListIndex(long nNewValue);
    long GetMultiSelect();
    void SetMultiSelect(long nNewValue);
    VARIANT RemoveAllItems();
    VARIANT RemoveItem(long Index, const VARIANT& Count);
    VARIANT GetSelected(const VARIANT& Index);
    void SetSelected(const VARIANT& Index, const VARIANT& newValue);
    long GetValue();
    void SetValue(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ListBoxes wrapper class

class ListBoxes : public COleDispatchDriver
{
public:
    ListBoxes() {}      // Calls COleDispatchDriver default constructor
    ListBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListBoxes(const ListBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    long Get_Default();
    void Set_Default(long nNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetList(const VARIANT& Index);
    void SetList(const VARIANT& Index, const VARIANT& newValue);
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    long GetListIndex();
    void SetListIndex(long nNewValue);
    long GetMultiSelect();
    void SetMultiSelect(long nNewValue);
    VARIANT RemoveAllItems();
    VARIANT RemoveItem(long Index, const VARIANT& Count);
    VARIANT GetSelected(const VARIANT& Index);
    void SetSelected(const VARIANT& Index, const VARIANT& newValue);
    long GetValue();
    void SetValue(long nNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// GroupBox wrapper class

class GroupBox : public COleDispatchDriver
{
public:
    GroupBox() {}       // Calls COleDispatchDriver default constructor
    GroupBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    GroupBox(const GroupBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// GroupBoxes wrapper class

class GroupBoxes : public COleDispatchDriver
{
public:
    GroupBoxes() {}     // Calls COleDispatchDriver default constructor
    GroupBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    GroupBoxes(const GroupBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// DropDown wrapper class

class DropDown : public COleDispatchDriver
{
public:
    DropDown() {}       // Calls COleDispatchDriver default constructor
    DropDown(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DropDown(const DropDown& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    long Get_Default();
    void Set_Default(long nNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetLinkedObject();
    VARIANT GetList(const VARIANT& Index);
    void SetList(const VARIANT& Index, const VARIANT& newValue);
    long GetListCount();
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    long GetListIndex();
    void SetListIndex(long nNewValue);
    VARIANT RemoveAllItems();
    VARIANT RemoveItem(long Index, const VARIANT& Count);
    VARIANT GetSelected(const VARIANT& Index);
    void SetSelected(const VARIANT& Index, const VARIANT& newValue);
    long GetValue();
    void SetValue(long nNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    long GetDropDownLines();
    void SetDropDownLines(long nNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DropDowns wrapper class

class DropDowns : public COleDispatchDriver
{
public:
    DropDowns() {}      // Calls COleDispatchDriver default constructor
    DropDowns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DropDowns(const DropDowns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT AddItem(const VARIANT& Text, const VARIANT& Index);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    long Get_Default();
    void Set_Default(long nNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT GetList(const VARIANT& Index);
    void SetList(const VARIANT& Index, const VARIANT& newValue);
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    long GetListIndex();
    void SetListIndex(long nNewValue);
    VARIANT RemoveAllItems();
    VARIANT RemoveItem(long Index, const VARIANT& Count);
    VARIANT GetSelected(const VARIANT& Index);
    void SetSelected(const VARIANT& Index, const VARIANT& newValue);
    long GetValue();
    void SetValue(long nNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    long GetDropDownLines();
    void SetDropDownLines(long nNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height, const VARIANT& Editable);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Spinner wrapper class

class Spinner : public COleDispatchDriver
{
public:
    Spinner() {}        // Calls COleDispatchDriver default constructor
    Spinner(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Spinner(const Spinner& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    long GetMax();
    void SetMax(long nNewValue);
    long GetMin();
    void SetMin(long nNewValue);
    long GetSmallChange();
    void SetSmallChange(long nNewValue);
    long GetValue();
    void SetValue(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Spinners wrapper class

class Spinners : public COleDispatchDriver
{
public:
    Spinners() {}       // Calls COleDispatchDriver default constructor
    Spinners(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Spinners(const Spinners& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    long Get_Default();
    void Set_Default(long nNewValue);
    BOOL GetDisplay3DShading();
    void SetDisplay3DShading(BOOL bNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    long GetMax();
    void SetMax(long nNewValue);
    long GetMin();
    void SetMin(long nNewValue);
    long GetSmallChange();
    void SetSmallChange(long nNewValue);
    long GetValue();
    void SetValue(long nNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// DialogFrame wrapper class

class DialogFrame : public COleDispatchDriver
{
public:
    DialogFrame() {}        // Calls COleDispatchDriver default constructor
    DialogFrame(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DialogFrame(const DialogFrame& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT CopyPicture(long Appearance, long Format);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT Select(const VARIANT& Replace);
    double GetTop();
    void SetTop(double newValue);
    double GetWidth();
    void SetWidth(double newValue);
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Label wrapper class

class Label : public COleDispatchDriver
{
public:
    Label() {}      // Calls COleDispatchDriver default constructor
    Label(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Label(const Label& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Labels wrapper class

class Labels : public COleDispatchDriver
{
public:
    Labels() {}     // Calls COleDispatchDriver default constructor
    Labels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Labels(const Labels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetAccelerator();
    void SetAccelerator(const VARIANT& newValue);
    VARIANT GetPhoneticAccelerator();
    void SetPhoneticAccelerator(const VARIANT& newValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Panes wrapper class

class Panes : public COleDispatchDriver
{
public:
    Panes() {}      // Calls COleDispatchDriver default constructor
    Panes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Panes(const Panes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(long Index);
    LPDISPATCH Get_Default(long Index);
};
/////////////////////////////////////////////////////////////////////////////
// Pane wrapper class

class Pane : public COleDispatchDriver
{
public:
    Pane() {}       // Calls COleDispatchDriver default constructor
    Pane(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Pane(const Pane& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL Activate();
    long GetIndex();
    VARIANT LargeScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
    long GetScrollColumn();
    void SetScrollColumn(long nNewValue);
    long GetScrollRow();
    void SetScrollRow(long nNewValue);
    VARIANT SmallScroll(const VARIANT& Down, const VARIANT& Up, const VARIANT& ToRight, const VARIANT& ToLeft);
    LPDISPATCH GetVisibleRange();
    void ScrollIntoView(long Left, long Top, long Width, long Height, const VARIANT& Start);
};
/////////////////////////////////////////////////////////////////////////////
// Scenarios wrapper class

class Scenarios : public COleDispatchDriver
{
public:
    Scenarios() {}      // Calls COleDispatchDriver default constructor
    Scenarios(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Scenarios(const Scenarios& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Name, const VARIANT& ChangingCells, const VARIANT& Values, const VARIANT& Comment, const VARIANT& Locked, const VARIANT& Hidden);
    long GetCount();
    VARIANT CreateSummary(long ReportType, const VARIANT& ResultCells);
    LPDISPATCH Item(const VARIANT& Index);
    VARIANT Merge(const VARIANT& Source);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Scenario wrapper class

class Scenario : public COleDispatchDriver
{
public:
    Scenario() {}       // Calls COleDispatchDriver default constructor
    Scenario(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Scenario(const Scenario& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT ChangeScenario(const VARIANT& ChangingCells, const VARIANT& Values);
    LPDISPATCH GetChangingCells();
    CString GetComment();
    void SetComment(LPCTSTR lpszNewValue);
    VARIANT Delete();
    BOOL GetHidden();
    void SetHidden(BOOL bNewValue);
    long GetIndex();
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT Show();
    VARIANT GetValues(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// GroupObject wrapper class

class GroupObject : public COleDispatchDriver
{
public:
    GroupObject() {}        // Calls COleDispatchDriver default constructor
    GroupObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    GroupObject(const GroupObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetArrowHeadLength();
    void SetArrowHeadLength(const VARIANT& newValue);
    VARIANT GetArrowHeadStyle();
    void SetArrowHeadStyle(const VARIANT& newValue);
    VARIANT GetArrowHeadWidth();
    void SetArrowHeadWidth(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    LPDISPATCH GetBorder();
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    long Get_Default();
    void Set_Default(long nNewValue);
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    LPDISPATCH GetInterior();
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    LPDISPATCH Ungroup();
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// GroupObjects wrapper class

class GroupObjects : public COleDispatchDriver
{
public:
    GroupObjects() {}       // Calls COleDispatchDriver default constructor
    GroupObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    GroupObjects(const GroupObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetArrowHeadLength();
    void SetArrowHeadLength(const VARIANT& newValue);
    VARIANT GetArrowHeadStyle();
    void SetArrowHeadStyle(const VARIANT& newValue);
    VARIANT GetArrowHeadWidth();
    void SetArrowHeadWidth(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    LPDISPATCH GetBorder();
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    long Get_Default();
    void Set_Default(long nNewValue);
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    LPDISPATCH GetInterior();
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    LPDISPATCH Ungroup();
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Line wrapper class

class Line : public COleDispatchDriver
{
public:
    Line() {}       // Calls COleDispatchDriver default constructor
    Line(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Line(const Line& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT GetArrowHeadLength();
    void SetArrowHeadLength(const VARIANT& newValue);
    VARIANT GetArrowHeadStyle();
    void SetArrowHeadStyle(const VARIANT& newValue);
    VARIANT GetArrowHeadWidth();
    void SetArrowHeadWidth(const VARIANT& newValue);
    LPDISPATCH GetBorder();
};
/////////////////////////////////////////////////////////////////////////////
// Lines wrapper class

class Lines : public COleDispatchDriver
{
public:
    Lines() {}      // Calls COleDispatchDriver default constructor
    Lines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Lines(const Lines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT GetArrowHeadLength();
    void SetArrowHeadLength(const VARIANT& newValue);
    VARIANT GetArrowHeadStyle();
    void SetArrowHeadStyle(const VARIANT& newValue);
    VARIANT GetArrowHeadWidth();
    void SetArrowHeadWidth(const VARIANT& newValue);
    LPDISPATCH GetBorder();
    LPDISPATCH Add(double X1, double Y1, double X2, double Y2);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Rectangle wrapper class

class Rectangle : public COleDispatchDriver
{
public:
    Rectangle() {}      // Calls COleDispatchDriver default constructor
    Rectangle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Rectangle(const Rectangle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Rectangles wrapper class

class Rectangles : public COleDispatchDriver
{
public:
    Rectangles() {}     // Calls COleDispatchDriver default constructor
    Rectangles(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Rectangles(const Rectangles& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Oval wrapper class

class Oval : public COleDispatchDriver
{
public:
    Oval() {}       // Calls COleDispatchDriver default constructor
    Oval(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Oval(const Oval& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Ovals wrapper class

class Ovals : public COleDispatchDriver
{
public:
    Ovals() {}      // Calls COleDispatchDriver default constructor
    Ovals(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Ovals(const Ovals& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Arc wrapper class

class Arc : public COleDispatchDriver
{
public:
    Arc() {}        // Calls COleDispatchDriver default constructor
    Arc(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Arc(const Arc& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
};
/////////////////////////////////////////////////////////////////////////////
// Arcs wrapper class

class Arcs : public COleDispatchDriver
{
public:
    Arcs() {}       // Calls COleDispatchDriver default constructor
    Arcs(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Arcs(const Arcs& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    LPDISPATCH Add(double X1, double Y1, double X2, double Y2);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// OLEObjectEvents wrapper class

class OLEObjectEvents : public COleDispatchDriver
{
public:
    OLEObjectEvents() {}        // Calls COleDispatchDriver default constructor
    OLEObjectEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OLEObjectEvents(const OLEObjectEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void GotFocus();
    void LostFocus();
};
/////////////////////////////////////////////////////////////////////////////
// _OLEObject wrapper class

class _OLEObject : public COleDispatchDriver
{
public:
    _OLEObject() {}     // Calls COleDispatchDriver default constructor
    _OLEObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _OLEObject(const _OLEObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    VARIANT Activate();
    BOOL GetAutoLoad();
    void SetAutoLoad(BOOL bNewValue);
    BOOL GetAutoUpdate();
    void SetAutoUpdate(BOOL bNewValue);
    LPDISPATCH GetObject();
    VARIANT GetOLEType();
    CString GetSourceName();
    void SetSourceName(LPCTSTR lpszNewValue);
    VARIANT Update();
    VARIANT Verb(long Verb);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    CString GetProgID();
};
/////////////////////////////////////////////////////////////////////////////
// OLEObjects wrapper class

class OLEObjects : public COleDispatchDriver
{
public:
    OLEObjects() {}     // Calls COleDispatchDriver default constructor
    OLEObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OLEObjects(const OLEObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    BOOL GetAutoLoad();
    void SetAutoLoad(BOOL bNewValue);
    CString GetSourceName();
    void SetSourceName(LPCTSTR lpszNewValue);
    LPDISPATCH Add(const VARIANT& ClassType, const VARIANT& Filename, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& Left, const VARIANT& Top,
                   const VARIANT& Width, const VARIANT& Height);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// TextBox wrapper class

class TextBox : public COleDispatchDriver
{
public:
    TextBox() {}        // Calls COleDispatchDriver default constructor
    TextBox(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    TextBox(const TextBox& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// TextBoxes wrapper class

class TextBoxes : public COleDispatchDriver
{
public:
    TextBoxes() {}      // Calls COleDispatchDriver default constructor
    TextBoxes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    TextBoxes(const TextBoxes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Picture wrapper class

class Picture : public COleDispatchDriver
{
public:
    Picture() {}        // Calls COleDispatchDriver default constructor
    Picture(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Picture(const Picture& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Pictures wrapper class

class Pictures : public COleDispatchDriver
{
public:
    Pictures() {}       // Calls COleDispatchDriver default constructor
    Pictures(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Pictures(const Pictures& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Insert(LPCTSTR Filename, const VARIANT& Converter);
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
    LPDISPATCH Paste(const VARIANT& Link);
};
/////////////////////////////////////////////////////////////////////////////
// Drawing wrapper class

class Drawing : public COleDispatchDriver
{
public:
    Drawing() {}        // Calls COleDispatchDriver default constructor
    Drawing(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Drawing(const Drawing& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    VARIANT AddVertex(double Left, double Top);
    VARIANT Reshape(long Vertex, BOOL Insert, const VARIANT& Left, const VARIANT& Top);
    VARIANT GetVertices(const VARIANT& Index1, const VARIANT& Index2);
};
/////////////////////////////////////////////////////////////////////////////
// Drawings wrapper class

class Drawings : public COleDispatchDriver
{
public:
    Drawings() {}       // Calls COleDispatchDriver default constructor
    Drawings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Drawings(const Drawings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    BOOL GetAddIndent();
    void SetAddIndent(BOOL bNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    VARIANT CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    LPDISPATCH GetFont();
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    VARIANT Reshape(long Vertex, BOOL Insert, const VARIANT& Left, const VARIANT& Top);
    LPDISPATCH Add(double X1, double Y1, double X2, double Y2, BOOL Closed);
    long GetCount();
    LPDISPATCH Group();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// RoutingSlip wrapper class

class RoutingSlip : public COleDispatchDriver
{
public:
    RoutingSlip() {}        // Calls COleDispatchDriver default constructor
    RoutingSlip(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    RoutingSlip(const RoutingSlip& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetDelivery();
    void SetDelivery(long nNewValue);
    VARIANT GetMessage();
    void SetMessage(const VARIANT& newValue);
    VARIANT GetRecipients(const VARIANT& Index);
    void SetRecipients(const VARIANT& Index, const VARIANT& newValue);
    VARIANT Reset();
    BOOL GetReturnWhenDone();
    void SetReturnWhenDone(BOOL bNewValue);
    long GetStatus();
    VARIANT GetSubject();
    void SetSubject(const VARIANT& newValue);
    BOOL GetTrackStatus();
    void SetTrackStatus(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Outline wrapper class

class Outline : public COleDispatchDriver
{
public:
    Outline() {}        // Calls COleDispatchDriver default constructor
    Outline(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Outline(const Outline& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetAutomaticStyles();
    void SetAutomaticStyles(BOOL bNewValue);
    VARIANT ShowLevels(const VARIANT& RowLevels, const VARIANT& ColumnLevels);
    long GetSummaryColumn();
    void SetSummaryColumn(long nNewValue);
    long GetSummaryRow();
    void SetSummaryRow(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Module wrapper class

class Module : public COleDispatchDriver
{
public:
    Module() {}     // Calls COleDispatchDriver default constructor
    Module(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Module(const Module& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Activate();
    void Copy(const VARIANT& Before, const VARIANT& After);
    void Delete();
    CString GetCodeName();
    CString Get_CodeName();
    void Set_CodeName(LPCTSTR lpszNewValue);
    long GetIndex();
    void Move(const VARIANT& Before, const VARIANT& After);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetNext();
    LPDISPATCH GetPageSetup();
    LPDISPATCH GetPrevious();
    BOOL GetProtectContents();
    BOOL GetProtectionMode();
    void Select(const VARIANT& Replace);
    void Unprotect(const VARIANT& Password);
    long GetVisible();
    void SetVisible(long nNewValue);
    LPDISPATCH GetShapes();
    VARIANT InsertFile(const VARIANT& Filename, const VARIANT& Merge);
    void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage,
                const VARIANT& TextVisualLayout);
    void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly);
};
/////////////////////////////////////////////////////////////////////////////
// Modules wrapper class

class Modules : public COleDispatchDriver
{
public:
    Modules() {}        // Calls COleDispatchDriver default constructor
    Modules(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Modules(const Modules& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count);
    void Copy(const VARIANT& Before, const VARIANT& After);
    long GetCount();
    void Delete();
    LPDISPATCH GetItem(const VARIANT& Index);
    void Move(const VARIANT& Before, const VARIANT& After);
    LPUNKNOWN Get_NewEnum();
    void Select(const VARIANT& Replace);
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    VARIANT GetVisible();
    void SetVisible(const VARIANT& newValue);
    LPDISPATCH Get_Default(const VARIANT& Index);
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// DialogSheet wrapper class

class DialogSheet : public COleDispatchDriver
{
public:
    DialogSheet() {}        // Calls COleDispatchDriver default constructor
    DialogSheet(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DialogSheet(const DialogSheet& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Activate();
    void Copy(const VARIANT& Before, const VARIANT& After);
    void Delete();
    CString GetCodeName();
    CString Get_CodeName();
    void Set_CodeName(LPCTSTR lpszNewValue);
    long GetIndex();
    void Move(const VARIANT& Before, const VARIANT& After);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetNext();
    LPDISPATCH GetPageSetup();
    LPDISPATCH GetPrevious();
    void PrintPreview(const VARIANT& EnableChanges);
    BOOL GetProtectContents();
    BOOL GetProtectDrawingObjects();
    BOOL GetProtectionMode();
    BOOL GetProtectScenarios();
    void Select(const VARIANT& Replace);
    void Unprotect(const VARIANT& Password);
    long GetVisible();
    void SetVisible(long nNewValue);
    LPDISPATCH GetShapes();
    BOOL GetEnableCalculation();
    void SetEnableCalculation(BOOL bNewValue);
    LPDISPATCH ChartObjects(const VARIANT& Index);
    void CheckSpelling(const VARIANT& CustomDictionary, const VARIANT& IgnoreUppercase, const VARIANT& AlwaysSuggest, const VARIANT& SpellLang);
    BOOL GetEnableAutoFilter();
    void SetEnableAutoFilter(BOOL bNewValue);
    long GetEnableSelection();
    void SetEnableSelection(long nNewValue);
    BOOL GetEnableOutlining();
    void SetEnableOutlining(BOOL bNewValue);
    BOOL GetEnablePivotTable();
    void SetEnablePivotTable(BOOL bNewValue);
    VARIANT Evaluate(const VARIANT& Name);
    VARIANT _Evaluate(const VARIANT& Name);
    void ResetAllPageBreaks();
    LPDISPATCH GetNames();
    LPDISPATCH OLEObjects(const VARIANT& Index);
    void Paste(const VARIANT& Destination, const VARIANT& Link);
    CString GetScrollArea();
    void SetScrollArea(LPCTSTR lpszNewValue);
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    LPDISPATCH GetQueryTables();
    BOOL GetDisplayPageBreaks();
    void SetDisplayPageBreaks(BOOL bNewValue);
    LPDISPATCH GetComments();
    LPDISPATCH GetHyperlinks();
    void ClearCircles();
    void CircleInvalid();
    LPDISPATCH GetAutoFilter();
    BOOL GetDisplayRightToLeft();
    void SetDisplayRightToLeft(BOOL bNewValue);
    LPDISPATCH GetScripts();
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
    LPDISPATCH GetTab();
    LPDISPATCH GetMailEnvelope();
    void SaveAs(LPCTSTR Filename, const VARIANT& FileFormat, const VARIANT& Password, const VARIANT& WriteResPassword, const VARIANT& ReadOnlyRecommended, const VARIANT& CreateBackup, const VARIANT& AddToMru, const VARIANT& TextCodepage,
                const VARIANT& TextVisualLayout, const VARIANT& Local);
    LPDISPATCH GetCustomProperties();
    LPDISPATCH GetSmartTags();
    LPDISPATCH GetProtection();
    void PasteSpecial(const VARIANT& Format, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& NoHTMLFormatting);
    void Protect(const VARIANT& Password, const VARIANT& DrawingObjects, const VARIANT& Contents, const VARIANT& Scenarios, const VARIANT& UserInterfaceOnly, const VARIANT& AllowFormattingCells, const VARIANT& AllowFormattingColumns,
                 const VARIANT& AllowFormattingRows, const VARIANT& AllowInsertingColumns, const VARIANT& AllowInsertingRows, const VARIANT& AllowInsertingHyperlinks, const VARIANT& AllowDeletingColumns, const VARIANT& AllowDeletingRows,
                 const VARIANT& AllowSorting, const VARIANT& AllowFiltering, const VARIANT& AllowUsingPivotTables);
    VARIANT GetDefaultButton();
    void SetDefaultButton(const VARIANT& newValue);
    VARIANT GetFocus();
    void SetFocus(const VARIANT& newValue);
    BOOL Hide(const VARIANT& Cancel);
    BOOL Show();
};
/////////////////////////////////////////////////////////////////////////////
// DialogSheets wrapper class

class DialogSheets : public COleDispatchDriver
{
public:
    DialogSheets() {}       // Calls COleDispatchDriver default constructor
    DialogSheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DialogSheets(const DialogSheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count);
    void Copy(const VARIANT& Before, const VARIANT& After);
    long GetCount();
    void Delete();
    LPDISPATCH GetItem(const VARIANT& Index);
    void Move(const VARIANT& Before, const VARIANT& After);
    LPUNKNOWN Get_NewEnum();
    void PrintPreview(const VARIANT& EnableChanges);
    void Select(const VARIANT& Replace);
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    VARIANT GetVisible();
    void SetVisible(const VARIANT& newValue);
    LPDISPATCH Get_Default(const VARIANT& Index);
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// Worksheets wrapper class

class Worksheets : public COleDispatchDriver
{
public:
    Worksheets() {}     // Calls COleDispatchDriver default constructor
    Worksheets(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Worksheets(const Worksheets& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Before, const VARIANT& After, const VARIANT& Count, const VARIANT& Type);
    void Copy(const VARIANT& Before, const VARIANT& After);
    long GetCount();
    void Delete();
    void FillAcrossSheets(LPDISPATCH Range, long Type);
    LPDISPATCH GetItem(const VARIANT& Index);
    void Move(const VARIANT& Before, const VARIANT& After);
    LPUNKNOWN Get_NewEnum();
    void PrintPreview(const VARIANT& EnableChanges);
    void Select(const VARIANT& Replace);
    LPDISPATCH GetHPageBreaks();
    LPDISPATCH GetVPageBreaks();
    VARIANT GetVisible();
    void SetVisible(const VARIANT& newValue);
    LPDISPATCH Get_Default(const VARIANT& Index);
    void PrintOut(const VARIANT& From, const VARIANT& To, const VARIANT& Copies, const VARIANT& Preview, const VARIANT& ActivePrinter, const VARIANT& PrintToFile, const VARIANT& Collate, const VARIANT& PrToFileName);
};
/////////////////////////////////////////////////////////////////////////////
// PageSetup wrapper class

class PageSetup : public COleDispatchDriver
{
public:
    PageSetup() {}      // Calls COleDispatchDriver default constructor
    PageSetup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PageSetup(const PageSetup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetBlackAndWhite();
    void SetBlackAndWhite(BOOL bNewValue);
    double GetBottomMargin();
    void SetBottomMargin(double newValue);
    CString GetCenterFooter();
    void SetCenterFooter(LPCTSTR lpszNewValue);
    CString GetCenterHeader();
    void SetCenterHeader(LPCTSTR lpszNewValue);
    BOOL GetCenterHorizontally();
    void SetCenterHorizontally(BOOL bNewValue);
    BOOL GetCenterVertically();
    void SetCenterVertically(BOOL bNewValue);
    long GetChartSize();
    void SetChartSize(long nNewValue);
    BOOL GetDraft();
    void SetDraft(BOOL bNewValue);
    long GetFirstPageNumber();
    void SetFirstPageNumber(long nNewValue);
    VARIANT GetFitToPagesTall();
    void SetFitToPagesTall(const VARIANT& newValue);
    VARIANT GetFitToPagesWide();
    void SetFitToPagesWide(const VARIANT& newValue);
    double GetFooterMargin();
    void SetFooterMargin(double newValue);
    double GetHeaderMargin();
    void SetHeaderMargin(double newValue);
    CString GetLeftFooter();
    void SetLeftFooter(LPCTSTR lpszNewValue);
    CString GetLeftHeader();
    void SetLeftHeader(LPCTSTR lpszNewValue);
    double GetLeftMargin();
    void SetLeftMargin(double newValue);
    long GetOrder();
    void SetOrder(long nNewValue);
    long GetOrientation();
    void SetOrientation(long nNewValue);
    long GetPaperSize();
    void SetPaperSize(long nNewValue);
    CString GetPrintArea();
    void SetPrintArea(LPCTSTR lpszNewValue);
    BOOL GetPrintGridlines();
    void SetPrintGridlines(BOOL bNewValue);
    BOOL GetPrintHeadings();
    void SetPrintHeadings(BOOL bNewValue);
    BOOL GetPrintNotes();
    void SetPrintNotes(BOOL bNewValue);
    VARIANT GetPrintQuality(const VARIANT& Index);
    void SetPrintQuality(const VARIANT& Index, const VARIANT& newValue);
    CString GetPrintTitleColumns();
    void SetPrintTitleColumns(LPCTSTR lpszNewValue);
    CString GetPrintTitleRows();
    void SetPrintTitleRows(LPCTSTR lpszNewValue);
    CString GetRightFooter();
    void SetRightFooter(LPCTSTR lpszNewValue);
    CString GetRightHeader();
    void SetRightHeader(LPCTSTR lpszNewValue);
    double GetRightMargin();
    void SetRightMargin(double newValue);
    double GetTopMargin();
    void SetTopMargin(double newValue);
    VARIANT GetZoom();
    void SetZoom(const VARIANT& newValue);
    long GetPrintComments();
    void SetPrintComments(long nNewValue);
    long GetPrintErrors();
    void SetPrintErrors(long nNewValue);
    LPDISPATCH GetCenterHeaderPicture();
    LPDISPATCH GetCenterFooterPicture();
    LPDISPATCH GetLeftHeaderPicture();
    LPDISPATCH GetLeftFooterPicture();
    LPDISPATCH GetRightHeaderPicture();
    LPDISPATCH GetRightFooterPicture();
};
/////////////////////////////////////////////////////////////////////////////
// Names wrapper class

class Names : public COleDispatchDriver
{
public:
    Names() {}      // Calls COleDispatchDriver default constructor
    Names(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Names(const Names& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Name, const VARIANT& RefersTo, const VARIANT& Visible, const VARIANT& MacroType, const VARIANT& ShortcutKey, const VARIANT& Category, const VARIANT& NameLocal, const VARIANT& RefersToLocal,
                   const VARIANT& CategoryLocal, const VARIANT& RefersToR1C1, const VARIANT& RefersToR1C1Local);
    LPDISPATCH Item(const VARIANT& Index, const VARIANT& IndexLocal, const VARIANT& RefersTo);
    LPDISPATCH _Default(const VARIANT& Index, const VARIANT& IndexLocal, const VARIANT& RefersTo);
    long GetCount();
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Name wrapper class

class Name : public COleDispatchDriver
{
public:
    Name() {}       // Calls COleDispatchDriver default constructor
    Name(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Name(const Name& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString Get_Default();
    long GetIndex();
    CString GetCategory();
    void SetCategory(LPCTSTR lpszNewValue);
    CString GetCategoryLocal();
    void SetCategoryLocal(LPCTSTR lpszNewValue);
    void Delete();
    long GetMacroType();
    void SetMacroType(long nNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetRefersTo();
    void SetRefersTo(const VARIANT& newValue);
    CString GetShortcutKey();
    void SetShortcutKey(LPCTSTR lpszNewValue);
    CString GetValue();
    void SetValue(LPCTSTR lpszNewValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    CString GetNameLocal();
    void SetNameLocal(LPCTSTR lpszNewValue);
    VARIANT GetRefersToLocal();
    void SetRefersToLocal(const VARIANT& newValue);
    VARIANT GetRefersToR1C1();
    void SetRefersToR1C1(const VARIANT& newValue);
    VARIANT GetRefersToR1C1Local();
    void SetRefersToR1C1Local(const VARIANT& newValue);
    LPDISPATCH GetRefersToRange();
};
/////////////////////////////////////////////////////////////////////////////
// ChartObject wrapper class

class ChartObject : public COleDispatchDriver
{
public:
    ChartObject() {}        // Calls COleDispatchDriver default constructor
    ChartObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartObject(const ChartObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBottomRightCell();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    long GetIndex();
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    LPDISPATCH GetTopLeftCell();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    long GetZOrder();
    LPDISPATCH GetShapeRange();
    VARIANT Activate();
    LPDISPATCH GetChart();
    BOOL GetProtectChartObject();
    void SetProtectChartObject(BOOL bNewValue);
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartObjects wrapper class

class ChartObjects : public COleDispatchDriver
{
public:
    ChartObjects() {}       // Calls COleDispatchDriver default constructor
    ChartObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartObjects(const ChartObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT BringToFront();
    VARIANT Copy();
    VARIANT CopyPicture(long Appearance, long Format);
    VARIANT Cut();
    VARIANT Delete();
    LPDISPATCH Duplicate();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    double GetHeight();
    void SetHeight(double newValue);
    double GetLeft();
    void SetLeft(double newValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    VARIANT GetPlacement();
    void SetPlacement(const VARIANT& newValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    VARIANT Select(const VARIANT& Replace);
    VARIANT SendToBack();
    double GetTop();
    void SetTop(double newValue);
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    double GetWidth();
    void SetWidth(double newValue);
    LPDISPATCH GetShapeRange();
    BOOL GetRoundedCorners();
    void SetRoundedCorners(BOOL bNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetInterior();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    LPDISPATCH Add(double Left, double Top, double Width, double Height);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Mailer wrapper class

class Mailer : public COleDispatchDriver
{
public:
    Mailer() {}     // Calls COleDispatchDriver default constructor
    Mailer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Mailer(const Mailer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetBCCRecipients();
    void SetBCCRecipients(const VARIANT& newValue);
    VARIANT GetCCRecipients();
    void SetCCRecipients(const VARIANT& newValue);
    VARIANT GetEnclosures();
    void SetEnclosures(const VARIANT& newValue);
    BOOL GetReceived();
    DATE GetSendDateTime();
    CString GetSender();
    CString GetSubject();
    void SetSubject(LPCTSTR lpszNewValue);
    VARIANT GetToRecipients();
    void SetToRecipients(const VARIANT& newValue);
    VARIANT GetWhichAddress();
    void SetWhichAddress(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CustomViews wrapper class

class CustomViews : public COleDispatchDriver
{
public:
    CustomViews() {}        // Calls COleDispatchDriver default constructor
    CustomViews(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CustomViews(const CustomViews& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& ViewName);
    LPDISPATCH Add(LPCTSTR ViewName, const VARIANT& PrintSettings, const VARIANT& RowColSettings);
    LPDISPATCH Get_Default(const VARIANT& ViewName);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CustomView wrapper class

class CustomView : public COleDispatchDriver
{
public:
    CustomView() {}     // Calls COleDispatchDriver default constructor
    CustomView(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CustomView(const CustomView& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    BOOL GetPrintSettings();
    BOOL GetRowColSettings();
    void Show();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// FormatConditions wrapper class

class FormatConditions : public COleDispatchDriver
{
public:
    FormatConditions() {}       // Calls COleDispatchDriver default constructor
    FormatConditions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    FormatConditions(const FormatConditions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Add(long Type, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// FormatCondition wrapper class

class FormatCondition : public COleDispatchDriver
{
public:
    FormatCondition() {}        // Calls COleDispatchDriver default constructor
    FormatCondition(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    FormatCondition(const FormatCondition& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Modify(long Type, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
    long GetType();
    long GetOperator();
    CString GetFormula1();
    CString GetFormula2();
    LPDISPATCH GetInterior();
    LPDISPATCH GetBorders();
    LPDISPATCH GetFont();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Comments wrapper class

class Comments : public COleDispatchDriver
{
public:
    Comments() {}       // Calls COleDispatchDriver default constructor
    Comments(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Comments(const Comments& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(long Index);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Comment wrapper class

class Comment : public COleDispatchDriver
{
public:
    Comment() {}        // Calls COleDispatchDriver default constructor
    Comment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Comment(const Comment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetAuthor();
    LPDISPATCH GetShape();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    CString Text(const VARIANT& Text, const VARIANT& Start, const VARIANT& Overwrite);
    void Delete();
    LPDISPATCH Next();
    LPDISPATCH Previous();
};
/////////////////////////////////////////////////////////////////////////////
// RefreshEvents wrapper class

class RefreshEvents : public COleDispatchDriver
{
public:
    RefreshEvents() {}      // Calls COleDispatchDriver default constructor
    RefreshEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    RefreshEvents(const RefreshEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void BeforeRefresh(BOOL* Cancel);
    void AfterRefresh(BOOL Success);
};
/////////////////////////////////////////////////////////////////////////////
// _QueryTable wrapper class

class _QueryTable : public COleDispatchDriver
{
public:
    _QueryTable() {}        // Calls COleDispatchDriver default constructor
    _QueryTable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    _QueryTable(const _QueryTable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    BOOL GetFieldNames();
    void SetFieldNames(BOOL bNewValue);
    BOOL GetRowNumbers();
    void SetRowNumbers(BOOL bNewValue);
    BOOL GetFillAdjacentFormulas();
    void SetFillAdjacentFormulas(BOOL bNewValue);
    BOOL GetRefreshOnFileOpen();
    void SetRefreshOnFileOpen(BOOL bNewValue);
    BOOL GetRefreshing();
    BOOL GetFetchedRowOverflow();
    BOOL GetBackgroundQuery();
    void SetBackgroundQuery(BOOL bNewValue);
    void CancelRefresh();
    long GetRefreshStyle();
    void SetRefreshStyle(long nNewValue);
    BOOL GetEnableRefresh();
    void SetEnableRefresh(BOOL bNewValue);
    BOOL GetSavePassword();
    void SetSavePassword(BOOL bNewValue);
    LPDISPATCH GetDestination();
    VARIANT GetConnection();
    void SetConnection(const VARIANT& newValue);
    CString GetPostText();
    void SetPostText(LPCTSTR lpszNewValue);
    LPDISPATCH GetResultRange();
    void Delete();
    BOOL Refresh(const VARIANT& BackgroundQuery);
    LPDISPATCH GetParameters();
    LPDISPATCH GetRecordset();
    void SetRefRecordset(LPDISPATCH newValue);
    BOOL GetSaveData();
    void SetSaveData(BOOL bNewValue);
    BOOL GetEnableEditing();
    void SetEnableEditing(BOOL bNewValue);
    long GetTextFilePlatform();
    void SetTextFilePlatform(long nNewValue);
    long GetTextFileStartRow();
    void SetTextFileStartRow(long nNewValue);
    long GetTextFileParseType();
    void SetTextFileParseType(long nNewValue);
    long GetTextFileTextQualifier();
    void SetTextFileTextQualifier(long nNewValue);
    BOOL GetTextFileConsecutiveDelimiter();
    void SetTextFileConsecutiveDelimiter(BOOL bNewValue);
    BOOL GetTextFileTabDelimiter();
    void SetTextFileTabDelimiter(BOOL bNewValue);
    BOOL GetTextFileSemicolonDelimiter();
    void SetTextFileSemicolonDelimiter(BOOL bNewValue);
    BOOL GetTextFileCommaDelimiter();
    void SetTextFileCommaDelimiter(BOOL bNewValue);
    BOOL GetTextFileSpaceDelimiter();
    void SetTextFileSpaceDelimiter(BOOL bNewValue);
    CString GetTextFileOtherDelimiter();
    void SetTextFileOtherDelimiter(LPCTSTR lpszNewValue);
    VARIANT GetTextFileColumnDataTypes();
    void SetTextFileColumnDataTypes(const VARIANT& newValue);
    VARIANT GetTextFileFixedColumnWidths();
    void SetTextFileFixedColumnWidths(const VARIANT& newValue);
    BOOL GetPreserveColumnInfo();
    void SetPreserveColumnInfo(BOOL bNewValue);
    BOOL GetPreserveFormatting();
    void SetPreserveFormatting(BOOL bNewValue);
    BOOL GetAdjustColumnWidth();
    void SetAdjustColumnWidth(BOOL bNewValue);
    VARIANT GetCommandText();
    void SetCommandText(const VARIANT& newValue);
    long GetCommandType();
    void SetCommandType(long nNewValue);
    BOOL GetTextFilePromptOnRefresh();
    void SetTextFilePromptOnRefresh(BOOL bNewValue);
    long GetQueryType();
    BOOL GetMaintainConnection();
    void SetMaintainConnection(BOOL bNewValue);
    CString GetTextFileDecimalSeparator();
    void SetTextFileDecimalSeparator(LPCTSTR lpszNewValue);
    CString GetTextFileThousandsSeparator();
    void SetTextFileThousandsSeparator(LPCTSTR lpszNewValue);
    long GetRefreshPeriod();
    void SetRefreshPeriod(long nNewValue);
    void ResetTimer();
    long GetWebSelectionType();
    void SetWebSelectionType(long nNewValue);
    long GetWebFormatting();
    void SetWebFormatting(long nNewValue);
    CString GetWebTables();
    void SetWebTables(LPCTSTR lpszNewValue);
    BOOL GetWebPreFormattedTextToColumns();
    void SetWebPreFormattedTextToColumns(BOOL bNewValue);
    BOOL GetWebSingleBlockTextImport();
    void SetWebSingleBlockTextImport(BOOL bNewValue);
    BOOL GetWebDisableDateRecognition();
    void SetWebDisableDateRecognition(BOOL bNewValue);
    BOOL GetWebConsecutiveDelimitersAsOne();
    void SetWebConsecutiveDelimitersAsOne(BOOL bNewValue);
    BOOL GetWebDisableRedirections();
    void SetWebDisableRedirections(BOOL bNewValue);
    VARIANT GetEditWebPage();
    void SetEditWebPage(const VARIANT& newValue);
    CString GetSourceConnectionFile();
    void SetSourceConnectionFile(LPCTSTR lpszNewValue);
    CString GetSourceDataFile();
    void SetSourceDataFile(LPCTSTR lpszNewValue);
    long GetRobustConnect();
    void SetRobustConnect(long nNewValue);
    BOOL GetTextFileTrailingMinusNumbers();
    void SetTextFileTrailingMinusNumbers(BOOL bNewValue);
    void SaveAsODC(LPCTSTR ODCFileName, const VARIANT& Description, const VARIANT& Keywords);
    LPDISPATCH GetListObject();
    long GetTextFileVisualLayout();
    void SetTextFileVisualLayout(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// QueryTables wrapper class

class QueryTables : public COleDispatchDriver
{
public:
    QueryTables() {}        // Calls COleDispatchDriver default constructor
    QueryTables(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    QueryTables(const QueryTables& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Add(const VARIANT& Connection, LPDISPATCH Destination, const VARIANT& Sql);
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Parameter wrapper class

class Parameter : public COleDispatchDriver
{
public:
    Parameter() {}      // Calls COleDispatchDriver default constructor
    Parameter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Parameter(const Parameter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetDataType();
    void SetDataType(long nNewValue);
    long GetType();
    CString GetPromptString();
    VARIANT GetValue();
    LPDISPATCH GetSourceRange();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    void SetParam(long Type, const VARIANT& Value);
    BOOL GetRefreshOnChange();
    void SetRefreshOnChange(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Parameters wrapper class

class Parameters : public COleDispatchDriver
{
public:
    Parameters() {}     // Calls COleDispatchDriver default constructor
    Parameters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Parameters(const Parameters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Name, const VARIANT& iDataType);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    void Delete();
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ODBCError wrapper class

class ODBCError : public COleDispatchDriver
{
public:
    ODBCError() {}      // Calls COleDispatchDriver default constructor
    ODBCError(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ODBCError(const ODBCError& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetSqlState();
    CString GetErrorString();
};
/////////////////////////////////////////////////////////////////////////////
// ODBCErrors wrapper class

class ODBCErrors : public COleDispatchDriver
{
public:
    ODBCErrors() {}     // Calls COleDispatchDriver default constructor
    ODBCErrors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ODBCErrors(const ODBCErrors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(long Index);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Validation wrapper class

class Validation : public COleDispatchDriver
{
public:
    Validation() {}     // Calls COleDispatchDriver default constructor
    Validation(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Validation(const Validation& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Add(long Type, const VARIANT& AlertStyle, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
    long GetAlertStyle();
    BOOL GetIgnoreBlank();
    void SetIgnoreBlank(BOOL bNewValue);
    long GetIMEMode();
    void SetIMEMode(long nNewValue);
    BOOL GetInCellDropdown();
    void SetInCellDropdown(BOOL bNewValue);
    void Delete();
    CString GetErrorMessage();
    void SetErrorMessage(LPCTSTR lpszNewValue);
    CString GetErrorTitle();
    void SetErrorTitle(LPCTSTR lpszNewValue);
    CString GetInputMessage();
    void SetInputMessage(LPCTSTR lpszNewValue);
    CString GetInputTitle();
    void SetInputTitle(LPCTSTR lpszNewValue);
    CString GetFormula1();
    CString GetFormula2();
    void Modify(const VARIANT& Type, const VARIANT& AlertStyle, const VARIANT& Operator, const VARIANT& Formula1, const VARIANT& Formula2);
    long GetOperator();
    BOOL GetShowError();
    void SetShowError(BOOL bNewValue);
    BOOL GetShowInput();
    void SetShowInput(BOOL bNewValue);
    long GetType();
    BOOL GetValue();
};
/////////////////////////////////////////////////////////////////////////////
// Hyperlinks wrapper class

class Hyperlinks : public COleDispatchDriver
{
public:
    Hyperlinks() {}     // Calls COleDispatchDriver default constructor
    Hyperlinks(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Hyperlinks(const Hyperlinks& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPDISPATCH Anchor, LPCTSTR Address, const VARIANT& SubAddress, const VARIANT& ScreenTip, const VARIANT& TextToDisplay);
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Hyperlink wrapper class

class Hyperlink : public COleDispatchDriver
{
public:
    Hyperlink() {}      // Calls COleDispatchDriver default constructor
    Hyperlink(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Hyperlink(const Hyperlink& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    LPDISPATCH GetRange();
    LPDISPATCH GetShape();
    CString GetSubAddress();
    void SetSubAddress(LPCTSTR lpszNewValue);
    CString GetAddress();
    void SetAddress(LPCTSTR lpszNewValue);
    long GetType();
    void AddToFavorites();
    void Delete();
    void Follow(const VARIANT& NewWindow, const VARIANT& AddHistory, const VARIANT& ExtraInfo, const VARIANT& Method, const VARIANT& HeaderInfo);
    CString GetEmailSubject();
    void SetEmailSubject(LPCTSTR lpszNewValue);
    CString GetScreenTip();
    void SetScreenTip(LPCTSTR lpszNewValue);
    CString GetTextToDisplay();
    void SetTextToDisplay(LPCTSTR lpszNewValue);
    void CreateNewDocument(LPCTSTR Filename, BOOL EditNow, BOOL Overwrite);
};
/////////////////////////////////////////////////////////////////////////////
// AutoFilter wrapper class

class AutoFilter : public COleDispatchDriver
{
public:
    AutoFilter() {}     // Calls COleDispatchDriver default constructor
    AutoFilter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AutoFilter(const AutoFilter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetRange();
    LPDISPATCH GetFilters();
};
/////////////////////////////////////////////////////////////////////////////
// Filters wrapper class

class Filters : public COleDispatchDriver
{
public:
    Filters() {}        // Calls COleDispatchDriver default constructor
    Filters(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Filters(const Filters& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Get_Default(long Index);
    LPDISPATCH GetItem(long Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Filter wrapper class

class Filter : public COleDispatchDriver
{
public:
    Filter() {}     // Calls COleDispatchDriver default constructor
    Filter(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Filter(const Filter& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetOn();
    VARIANT GetCriteria1();
    long GetOperator();
    VARIANT GetCriteria2();
};
/////////////////////////////////////////////////////////////////////////////
// AutoCorrect wrapper class

class AutoCorrect : public COleDispatchDriver
{
public:
    AutoCorrect() {}        // Calls COleDispatchDriver default constructor
    AutoCorrect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AutoCorrect(const AutoCorrect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT AddReplacement(LPCTSTR What, LPCTSTR Replacement);
    BOOL GetCapitalizeNamesOfDays();
    void SetCapitalizeNamesOfDays(BOOL bNewValue);
    VARIANT DeleteReplacement(LPCTSTR What);
    VARIANT GetReplacementList(const VARIANT& Index);
    void SetReplacementList(const VARIANT& Index, const VARIANT& newValue);
    BOOL GetReplaceText();
    void SetReplaceText(BOOL bNewValue);
    BOOL GetTwoInitialCapitals();
    void SetTwoInitialCapitals(BOOL bNewValue);
    BOOL GetCorrectSentenceCap();
    void SetCorrectSentenceCap(BOOL bNewValue);
    BOOL GetCorrectCapsLock();
    void SetCorrectCapsLock(BOOL bNewValue);
    BOOL GetDisplayAutoCorrectOptions();
    void SetDisplayAutoCorrectOptions(BOOL bNewValue);
    BOOL GetAutoExpandListRange();
    void SetAutoExpandListRange(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Border wrapper class

class Border : public COleDispatchDriver
{
public:
    Border() {}     // Calls COleDispatchDriver default constructor
    Border(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Border(const Border& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetColor();
    void SetColor(const VARIANT& newValue);
    VARIANT GetColorIndex();
    void SetColorIndex(const VARIANT& newValue);
    VARIANT GetLineStyle();
    void SetLineStyle(const VARIANT& newValue);
    VARIANT GetWeight();
    void SetWeight(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Interior wrapper class

class Interior : public COleDispatchDriver
{
public:
    Interior() {}       // Calls COleDispatchDriver default constructor
    Interior(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Interior(const Interior& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetColor();
    void SetColor(const VARIANT& newValue);
    VARIANT GetColorIndex();
    void SetColorIndex(const VARIANT& newValue);
    VARIANT GetInvertIfNegative();
    void SetInvertIfNegative(const VARIANT& newValue);
    VARIANT GetPattern();
    void SetPattern(const VARIANT& newValue);
    VARIANT GetPatternColor();
    void SetPatternColor(const VARIANT& newValue);
    VARIANT GetPatternColorIndex();
    void SetPatternColorIndex(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartFillFormat wrapper class

class ChartFillFormat : public COleDispatchDriver
{
public:
    ChartFillFormat() {}        // Calls COleDispatchDriver default constructor
    ChartFillFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartFillFormat(const ChartFillFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void OneColorGradient(long Style, long Variant, float Degree);
    void TwoColorGradient(long Style, long Variant);
    void PresetTextured(long PresetTexture);
    void Solid();
    void Patterned(long Pattern);
    void UserPicture(const VARIANT& PictureFile, const VARIANT& PictureFormat, const VARIANT& PictureStackUnit, const VARIANT& PicturePlacement);
    void UserTextured(LPCTSTR TextureFile);
    void PresetGradient(long Style, long Variant, long PresetGradientType);
    LPDISPATCH GetBackColor();
    LPDISPATCH GetForeColor();
    long GetGradientColorType();
    float GetGradientDegree();
    long GetGradientStyle();
    long GetGradientVariant();
    long GetPattern();
    long GetPresetGradientType();
    long GetPresetTexture();
    CString GetTextureName();
    long GetTextureType();
    long GetType();
    long GetVisible();
    void SetVisible(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartColorFormat wrapper class

class ChartColorFormat : public COleDispatchDriver
{
public:
    ChartColorFormat() {}       // Calls COleDispatchDriver default constructor
    ChartColorFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartColorFormat(const ChartColorFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetSchemeColor();
    void SetSchemeColor(long nNewValue);
    long GetRgb();
    long Get_Default();
    long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// Axis wrapper class

class Axis : public COleDispatchDriver
{
public:
    Axis() {}       // Calls COleDispatchDriver default constructor
    Axis(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Axis(const Axis& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetAxisBetweenCategories();
    void SetAxisBetweenCategories(BOOL bNewValue);
    long GetAxisGroup();
    LPDISPATCH GetAxisTitle();
    LPDISPATCH GetBorder();
    VARIANT GetCategoryNames();
    void SetCategoryNames(const VARIANT& newValue);
    long GetCrosses();
    void SetCrosses(long nNewValue);
    double GetCrossesAt();
    void SetCrossesAt(double newValue);
    VARIANT Delete();
    BOOL GetHasMajorGridlines();
    void SetHasMajorGridlines(BOOL bNewValue);
    BOOL GetHasMinorGridlines();
    void SetHasMinorGridlines(BOOL bNewValue);
    BOOL GetHasTitle();
    void SetHasTitle(BOOL bNewValue);
    LPDISPATCH GetMajorGridlines();
    long GetMajorTickMark();
    void SetMajorTickMark(long nNewValue);
    double GetMajorUnit();
    void SetMajorUnit(double newValue);
    BOOL GetMajorUnitIsAuto();
    void SetMajorUnitIsAuto(BOOL bNewValue);
    double GetMaximumScale();
    void SetMaximumScale(double newValue);
    BOOL GetMaximumScaleIsAuto();
    void SetMaximumScaleIsAuto(BOOL bNewValue);
    double GetMinimumScale();
    void SetMinimumScale(double newValue);
    BOOL GetMinimumScaleIsAuto();
    void SetMinimumScaleIsAuto(BOOL bNewValue);
    LPDISPATCH GetMinorGridlines();
    long GetMinorTickMark();
    void SetMinorTickMark(long nNewValue);
    double GetMinorUnit();
    void SetMinorUnit(double newValue);
    BOOL GetMinorUnitIsAuto();
    void SetMinorUnitIsAuto(BOOL bNewValue);
    BOOL GetReversePlotOrder();
    void SetReversePlotOrder(BOOL bNewValue);
    long GetScaleType();
    void SetScaleType(long nNewValue);
    VARIANT Select();
    long GetTickLabelPosition();
    void SetTickLabelPosition(long nNewValue);
    LPDISPATCH GetTickLabels();
    long GetTickLabelSpacing();
    void SetTickLabelSpacing(long nNewValue);
    long GetTickMarkSpacing();
    void SetTickMarkSpacing(long nNewValue);
    long GetType();
    void SetType(long nNewValue);
    long GetBaseUnit();
    void SetBaseUnit(long nNewValue);
    BOOL GetBaseUnitIsAuto();
    void SetBaseUnitIsAuto(BOOL bNewValue);
    long GetMajorUnitScale();
    void SetMajorUnitScale(long nNewValue);
    long GetMinorUnitScale();
    void SetMinorUnitScale(long nNewValue);
    long GetCategoryType();
    void SetCategoryType(long nNewValue);
    double GetLeft();
    double GetTop();
    double GetWidth();
    double GetHeight();
    long GetDisplayUnit();
    void SetDisplayUnit(long nNewValue);
    double GetDisplayUnitCustom();
    void SetDisplayUnitCustom(double newValue);
    BOOL GetHasDisplayUnitLabel();
    void SetHasDisplayUnitLabel(BOOL bNewValue);
    LPDISPATCH GetDisplayUnitLabel();
};
/////////////////////////////////////////////////////////////////////////////
// ChartTitle wrapper class

class ChartTitle : public COleDispatchDriver
{
public:
    ChartTitle() {}     // Calls COleDispatchDriver default constructor
    ChartTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartTitle(const ChartTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    double GetLeft();
    void SetLeft(double newValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    double GetTop();
    void SetTop(double newValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// AxisTitle wrapper class

class AxisTitle : public COleDispatchDriver
{
public:
    AxisTitle() {}      // Calls COleDispatchDriver default constructor
    AxisTitle(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AxisTitle(const AxisTitle& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    double GetLeft();
    void SetLeft(double newValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    double GetTop();
    void SetTop(double newValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartGroup wrapper class

class ChartGroup : public COleDispatchDriver
{
public:
    ChartGroup() {}     // Calls COleDispatchDriver default constructor
    ChartGroup(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartGroup(const ChartGroup& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetAxisGroup();
    void SetAxisGroup(long nNewValue);
    long GetDoughnutHoleSize();
    void SetDoughnutHoleSize(long nNewValue);
    LPDISPATCH GetDownBars();
    LPDISPATCH GetDropLines();
    long GetFirstSliceAngle();
    void SetFirstSliceAngle(long nNewValue);
    long GetGapWidth();
    void SetGapWidth(long nNewValue);
    BOOL GetHasDropLines();
    void SetHasDropLines(BOOL bNewValue);
    BOOL GetHasHiLoLines();
    void SetHasHiLoLines(BOOL bNewValue);
    BOOL GetHasRadarAxisLabels();
    void SetHasRadarAxisLabels(BOOL bNewValue);
    BOOL GetHasSeriesLines();
    void SetHasSeriesLines(BOOL bNewValue);
    BOOL GetHasUpDownBars();
    void SetHasUpDownBars(BOOL bNewValue);
    LPDISPATCH GetHiLoLines();
    long GetIndex();
    long GetOverlap();
    void SetOverlap(long nNewValue);
    LPDISPATCH GetRadarAxisLabels();
    LPDISPATCH SeriesCollection(const VARIANT& Index);
    LPDISPATCH GetSeriesLines();
    LPDISPATCH GetUpBars();
    BOOL GetVaryByCategories();
    void SetVaryByCategories(BOOL bNewValue);
    long GetSizeRepresents();
    void SetSizeRepresents(long nNewValue);
    long GetBubbleScale();
    void SetBubbleScale(long nNewValue);
    BOOL GetShowNegativeBubbles();
    void SetShowNegativeBubbles(BOOL bNewValue);
    long GetSplitType();
    void SetSplitType(long nNewValue);
    VARIANT GetSplitValue();
    void SetSplitValue(const VARIANT& newValue);
    long GetSecondPlotSize();
    void SetSecondPlotSize(long nNewValue);
    BOOL GetHas3DShading();
    void SetHas3DShading(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ChartGroups wrapper class

class ChartGroups : public COleDispatchDriver
{
public:
    ChartGroups() {}        // Calls COleDispatchDriver default constructor
    ChartGroups(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartGroups(const ChartGroups& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Axes wrapper class

class Axes : public COleDispatchDriver
{
public:
    Axes() {}       // Calls COleDispatchDriver default constructor
    Axes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Axes(const Axes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(long Type, long AxisGroup);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Points wrapper class

class Points : public COleDispatchDriver
{
public:
    Points() {}     // Calls COleDispatchDriver default constructor
    Points(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Points(const Points& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(long Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Point wrapper class

class Point : public COleDispatchDriver
{
public:
    Point() {}      // Calls COleDispatchDriver default constructor
    Point(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Point(const Point& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    VARIANT Copy();
    LPDISPATCH GetDataLabel();
    VARIANT Delete();
    long GetExplosion();
    void SetExplosion(long nNewValue);
    BOOL GetHasDataLabel();
    void SetHasDataLabel(BOOL bNewValue);
    LPDISPATCH GetInterior();
    BOOL GetInvertIfNegative();
    void SetInvertIfNegative(BOOL bNewValue);
    long GetMarkerBackgroundColor();
    void SetMarkerBackgroundColor(long nNewValue);
    long GetMarkerBackgroundColorIndex();
    void SetMarkerBackgroundColorIndex(long nNewValue);
    long GetMarkerForegroundColor();
    void SetMarkerForegroundColor(long nNewValue);
    long GetMarkerForegroundColorIndex();
    void SetMarkerForegroundColorIndex(long nNewValue);
    long GetMarkerSize();
    void SetMarkerSize(long nNewValue);
    long GetMarkerStyle();
    void SetMarkerStyle(long nNewValue);
    VARIANT Paste();
    long GetPictureType();
    void SetPictureType(long nNewValue);
    long GetPictureUnit();
    void SetPictureUnit(long nNewValue);
    VARIANT Select();
    BOOL GetApplyPictToSides();
    void SetApplyPictToSides(BOOL bNewValue);
    BOOL GetApplyPictToFront();
    void SetApplyPictToFront(BOOL bNewValue);
    BOOL GetApplyPictToEnd();
    void SetApplyPictToEnd(BOOL bNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    BOOL GetSecondaryPlot();
    void SetSecondaryPlot(BOOL bNewValue);
    LPDISPATCH GetFill();
    VARIANT ApplyDataLabels(long Type, const VARIANT& LegendKey, const VARIANT& AutoText, const VARIANT& HasLeaderLines, const VARIANT& ShowSeriesName, const VARIANT& ShowCategoryName, const VARIANT& ShowValue, const VARIANT& ShowPercentage,
                            const VARIANT& ShowBubbleSize, const VARIANT& Separator);
};
/////////////////////////////////////////////////////////////////////////////
// Series wrapper class

class Series : public COleDispatchDriver
{
public:
    Series() {}     // Calls COleDispatchDriver default constructor
    Series(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Series(const Series& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetAxisGroup();
    void SetAxisGroup(long nNewValue);
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    VARIANT Copy();
    LPDISPATCH DataLabels(const VARIANT& Index);
    VARIANT Delete();
    VARIANT ErrorBar(long Direction, long Include, long Type, const VARIANT& Amount, const VARIANT& MinusValues);
    LPDISPATCH GetErrorBars();
    long GetExplosion();
    void SetExplosion(long nNewValue);
    CString GetFormula();
    void SetFormula(LPCTSTR lpszNewValue);
    CString GetFormulaLocal();
    void SetFormulaLocal(LPCTSTR lpszNewValue);
    CString GetFormulaR1C1();
    void SetFormulaR1C1(LPCTSTR lpszNewValue);
    CString GetFormulaR1C1Local();
    void SetFormulaR1C1Local(LPCTSTR lpszNewValue);
    BOOL GetHasDataLabels();
    void SetHasDataLabels(BOOL bNewValue);
    BOOL GetHasErrorBars();
    void SetHasErrorBars(BOOL bNewValue);
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    BOOL GetInvertIfNegative();
    void SetInvertIfNegative(BOOL bNewValue);
    long GetMarkerBackgroundColor();
    void SetMarkerBackgroundColor(long nNewValue);
    long GetMarkerBackgroundColorIndex();
    void SetMarkerBackgroundColorIndex(long nNewValue);
    long GetMarkerForegroundColor();
    void SetMarkerForegroundColor(long nNewValue);
    long GetMarkerForegroundColorIndex();
    void SetMarkerForegroundColorIndex(long nNewValue);
    long GetMarkerSize();
    void SetMarkerSize(long nNewValue);
    long GetMarkerStyle();
    void SetMarkerStyle(long nNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT Paste();
    long GetPictureType();
    void SetPictureType(long nNewValue);
    long GetPictureUnit();
    void SetPictureUnit(long nNewValue);
    long GetPlotOrder();
    void SetPlotOrder(long nNewValue);
    LPDISPATCH Points(const VARIANT& Index);
    VARIANT Select();
    BOOL GetSmooth();
    void SetSmooth(BOOL bNewValue);
    LPDISPATCH Trendlines(const VARIANT& Index);
    long GetType();
    void SetType(long nNewValue);
    long GetChartType();
    void SetChartType(long nNewValue);
    void ApplyCustomType(long ChartType);
    VARIANT GetValues();
    void SetValues(const VARIANT& newValue);
    VARIANT GetXValues();
    void SetXValues(const VARIANT& newValue);
    VARIANT GetBubbleSizes();
    void SetBubbleSizes(const VARIANT& newValue);
    long GetBarShape();
    void SetBarShape(long nNewValue);
    BOOL GetApplyPictToSides();
    void SetApplyPictToSides(BOOL bNewValue);
    BOOL GetApplyPictToFront();
    void SetApplyPictToFront(BOOL bNewValue);
    BOOL GetApplyPictToEnd();
    void SetApplyPictToEnd(BOOL bNewValue);
    BOOL GetHas3DEffect();
    void SetHas3DEffect(BOOL bNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    BOOL GetHasLeaderLines();
    void SetHasLeaderLines(BOOL bNewValue);
    LPDISPATCH GetLeaderLines();
    VARIANT ApplyDataLabels(long Type, const VARIANT& LegendKey, const VARIANT& AutoText, const VARIANT& HasLeaderLines, const VARIANT& ShowSeriesName, const VARIANT& ShowCategoryName, const VARIANT& ShowValue, const VARIANT& ShowPercentage,
                            const VARIANT& ShowBubbleSize, const VARIANT& Separator);
};
/////////////////////////////////////////////////////////////////////////////
// SeriesCollection wrapper class

class SeriesCollection : public COleDispatchDriver
{
public:
    SeriesCollection() {}       // Calls COleDispatchDriver default constructor
    SeriesCollection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SeriesCollection(const SeriesCollection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Source, long Rowcol, const VARIANT& SeriesLabels, const VARIANT& CategoryLabels, const VARIANT& Replace);
    long GetCount();
    VARIANT Extend(const VARIANT& Source, const VARIANT& Rowcol, const VARIANT& CategoryLabels);
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
    VARIANT Paste(long Rowcol, const VARIANT& SeriesLabels, const VARIANT& CategoryLabels, const VARIANT& Replace, const VARIANT& NewSeries);
    LPDISPATCH NewSeries();
};
/////////////////////////////////////////////////////////////////////////////
// DataLabel wrapper class

class DataLabel : public COleDispatchDriver
{
public:
    DataLabel() {}      // Calls COleDispatchDriver default constructor
    DataLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DataLabel(const DataLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    double GetLeft();
    void SetLeft(double newValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    double GetTop();
    void SetTop(double newValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoText();
    void SetAutoText(BOOL bNewValue);
    CString GetNumberFormat();
    void SetNumberFormat(LPCTSTR lpszNewValue);
    BOOL GetNumberFormatLinked();
    void SetNumberFormatLinked(BOOL bNewValue);
    VARIANT GetNumberFormatLocal();
    void SetNumberFormatLocal(const VARIANT& newValue);
    BOOL GetShowLegendKey();
    void SetShowLegendKey(BOOL bNewValue);
    VARIANT GetType();
    void SetType(const VARIANT& newValue);
    long GetPosition();
    void SetPosition(long nNewValue);
    BOOL GetShowSeriesName();
    void SetShowSeriesName(BOOL bNewValue);
    BOOL GetShowCategoryName();
    void SetShowCategoryName(BOOL bNewValue);
    BOOL GetShowValue();
    void SetShowValue(BOOL bNewValue);
    BOOL GetShowPercentage();
    void SetShowPercentage(BOOL bNewValue);
    BOOL GetShowBubbleSize();
    void SetShowBubbleSize(BOOL bNewValue);
    VARIANT GetSeparator();
    void SetSeparator(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// DataLabels wrapper class

class DataLabels : public COleDispatchDriver
{
public:
    DataLabels() {}     // Calls COleDispatchDriver default constructor
    DataLabels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DataLabels(const DataLabels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    BOOL GetAutoText();
    void SetAutoText(BOOL bNewValue);
    CString GetNumberFormat();
    void SetNumberFormat(LPCTSTR lpszNewValue);
    BOOL GetNumberFormatLinked();
    void SetNumberFormatLinked(BOOL bNewValue);
    VARIANT GetNumberFormatLocal();
    void SetNumberFormatLocal(const VARIANT& newValue);
    BOOL GetShowLegendKey();
    void SetShowLegendKey(BOOL bNewValue);
    VARIANT GetType();
    void SetType(const VARIANT& newValue);
    long GetPosition();
    void SetPosition(long nNewValue);
    BOOL GetShowSeriesName();
    void SetShowSeriesName(BOOL bNewValue);
    BOOL GetShowCategoryName();
    void SetShowCategoryName(BOOL bNewValue);
    BOOL GetShowValue();
    void SetShowValue(BOOL bNewValue);
    BOOL GetShowPercentage();
    void SetShowPercentage(BOOL bNewValue);
    BOOL GetShowBubbleSize();
    void SetShowBubbleSize(BOOL bNewValue);
    VARIANT GetSeparator();
    void SetSeparator(const VARIANT& newValue);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// LegendEntry wrapper class

class LegendEntry : public COleDispatchDriver
{
public:
    LegendEntry() {}        // Calls COleDispatchDriver default constructor
    LegendEntry(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    LegendEntry(const LegendEntry& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT Delete();
    LPDISPATCH GetFont();
    long GetIndex();
    LPDISPATCH GetLegendKey();
    VARIANT Select();
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    double GetLeft();
    double GetTop();
    double GetWidth();
    double GetHeight();
};
/////////////////////////////////////////////////////////////////////////////
// LegendEntries wrapper class

class LegendEntries : public COleDispatchDriver
{
public:
    LegendEntries() {}      // Calls COleDispatchDriver default constructor
    LegendEntries(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    LegendEntries(const LegendEntries& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// LegendKey wrapper class

class LegendKey : public COleDispatchDriver
{
public:
    LegendKey() {}      // Calls COleDispatchDriver default constructor
    LegendKey(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    LegendKey(const LegendKey& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    BOOL GetInvertIfNegative();
    void SetInvertIfNegative(BOOL bNewValue);
    long GetMarkerBackgroundColor();
    void SetMarkerBackgroundColor(long nNewValue);
    long GetMarkerBackgroundColorIndex();
    void SetMarkerBackgroundColorIndex(long nNewValue);
    long GetMarkerForegroundColor();
    void SetMarkerForegroundColor(long nNewValue);
    long GetMarkerForegroundColorIndex();
    void SetMarkerForegroundColorIndex(long nNewValue);
    long GetMarkerSize();
    void SetMarkerSize(long nNewValue);
    long GetMarkerStyle();
    void SetMarkerStyle(long nNewValue);
    long GetPictureType();
    void SetPictureType(long nNewValue);
    long GetPictureUnit();
    void SetPictureUnit(long nNewValue);
    VARIANT Select();
    BOOL GetSmooth();
    void SetSmooth(BOOL bNewValue);
    double GetLeft();
    double GetTop();
    double GetWidth();
    double GetHeight();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Trendlines wrapper class

class Trendlines : public COleDispatchDriver
{
public:
    Trendlines() {}     // Calls COleDispatchDriver default constructor
    Trendlines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Trendlines(const Trendlines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(long Type, const VARIANT& Order, const VARIANT& Period, const VARIANT& Forward, const VARIANT& Backward, const VARIANT& Intercept, const VARIANT& DisplayEquation, const VARIANT& DisplayRSquared, const VARIANT& Name);
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Trendline wrapper class

class Trendline : public COleDispatchDriver
{
public:
    Trendline() {}      // Calls COleDispatchDriver default constructor
    Trendline(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Trendline(const Trendline& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetBackward();
    void SetBackward(long nNewValue);
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    LPDISPATCH GetDataLabel();
    VARIANT Delete();
    BOOL GetDisplayEquation();
    void SetDisplayEquation(BOOL bNewValue);
    BOOL GetDisplayRSquared();
    void SetDisplayRSquared(BOOL bNewValue);
    long GetForward();
    void SetForward(long nNewValue);
    long GetIndex();
    double GetIntercept();
    void SetIntercept(double newValue);
    BOOL GetInterceptIsAuto();
    void SetInterceptIsAuto(BOOL bNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    BOOL GetNameIsAuto();
    void SetNameIsAuto(BOOL bNewValue);
    long GetOrder();
    void SetOrder(long nNewValue);
    long GetPeriod();
    void SetPeriod(long nNewValue);
    VARIANT Select();
    long GetType();
    void SetType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Corners wrapper class

class Corners : public COleDispatchDriver
{
public:
    Corners() {}        // Calls COleDispatchDriver default constructor
    Corners(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Corners(const Corners& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
};
/////////////////////////////////////////////////////////////////////////////
// SeriesLines wrapper class

class SeriesLines : public COleDispatchDriver
{
public:
    SeriesLines() {}        // Calls COleDispatchDriver default constructor
    SeriesLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SeriesLines(const SeriesLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// HiLoLines wrapper class

class HiLoLines : public COleDispatchDriver
{
public:
    HiLoLines() {}      // Calls COleDispatchDriver default constructor
    HiLoLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    HiLoLines(const HiLoLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Gridlines wrapper class

class Gridlines : public COleDispatchDriver
{
public:
    Gridlines() {}      // Calls COleDispatchDriver default constructor
    Gridlines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Gridlines(const Gridlines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// DropLines wrapper class

class DropLines : public COleDispatchDriver
{
public:
    DropLines() {}      // Calls COleDispatchDriver default constructor
    DropLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DropLines(const DropLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
};
/////////////////////////////////////////////////////////////////////////////
// LeaderLines wrapper class

class LeaderLines : public COleDispatchDriver
{
public:
    LeaderLines() {}        // Calls COleDispatchDriver default constructor
    LeaderLines(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    LeaderLines(const LeaderLines& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBorder();
    void Delete();
    void Select();
};
/////////////////////////////////////////////////////////////////////////////
// UpBars wrapper class

class UpBars : public COleDispatchDriver
{
public:
    UpBars() {}     // Calls COleDispatchDriver default constructor
    UpBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    UpBars(const UpBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
};
/////////////////////////////////////////////////////////////////////////////
// DownBars wrapper class

class DownBars : public COleDispatchDriver
{
public:
    DownBars() {}       // Calls COleDispatchDriver default constructor
    DownBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DownBars(const DownBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
};
/////////////////////////////////////////////////////////////////////////////
// Floor wrapper class

class Floor : public COleDispatchDriver
{
public:
    Floor() {}      // Calls COleDispatchDriver default constructor
    Floor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Floor(const Floor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    VARIANT GetPictureType();
    void SetPictureType(const VARIANT& newValue);
    void Paste();
};
/////////////////////////////////////////////////////////////////////////////
// Walls wrapper class

class Walls : public COleDispatchDriver
{
public:
    Walls() {}      // Calls COleDispatchDriver default constructor
    Walls(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Walls(const Walls& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    VARIANT GetPictureType();
    void SetPictureType(const VARIANT& newValue);
    void Paste();
    VARIANT GetPictureUnit();
    void SetPictureUnit(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// TickLabels wrapper class

class TickLabels : public COleDispatchDriver
{
public:
    TickLabels() {}     // Calls COleDispatchDriver default constructor
    TickLabels(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    TickLabels(const TickLabels& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT Delete();
    LPDISPATCH GetFont();
    CString GetName();
    CString GetNumberFormat();
    void SetNumberFormat(LPCTSTR lpszNewValue);
    BOOL GetNumberFormatLinked();
    void SetNumberFormatLinked(BOOL bNewValue);
    VARIANT GetNumberFormatLocal();
    void SetNumberFormatLocal(const VARIANT& newValue);
    long GetOrientation();
    void SetOrientation(long nNewValue);
    VARIANT Select();
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
    long GetDepth();
    long GetOffset();
    void SetOffset(long nNewValue);
    long GetAlignment();
    void SetAlignment(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// PlotArea wrapper class

class PlotArea : public COleDispatchDriver
{
public:
    PlotArea() {}       // Calls COleDispatchDriver default constructor
    PlotArea(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PlotArea(const PlotArea& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT ClearFormats();
    double GetHeight();
    void SetHeight(double newValue);
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    double GetLeft();
    void SetLeft(double newValue);
    double GetTop();
    void SetTop(double newValue);
    double GetWidth();
    void SetWidth(double newValue);
    double GetInsideLeft();
    double GetInsideTop();
    double GetInsideWidth();
    double GetInsideHeight();
};
/////////////////////////////////////////////////////////////////////////////
// ChartArea wrapper class

class ChartArea : public COleDispatchDriver
{
public:
    ChartArea() {}      // Calls COleDispatchDriver default constructor
    ChartArea(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ChartArea(const ChartArea& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Clear();
    VARIANT ClearContents();
    VARIANT Copy();
    LPDISPATCH GetFont();
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    VARIANT ClearFormats();
    double GetHeight();
    void SetHeight(double newValue);
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    double GetLeft();
    void SetLeft(double newValue);
    double GetTop();
    void SetTop(double newValue);
    double GetWidth();
    void SetWidth(double newValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Legend wrapper class

class Legend : public COleDispatchDriver
{
public:
    Legend() {}     // Calls COleDispatchDriver default constructor
    Legend(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Legend(const Legend& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetFont();
    LPDISPATCH LegendEntries(const VARIANT& Index);
    long GetPosition();
    void SetPosition(long nNewValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    VARIANT Clear();
    double GetHeight();
    void SetHeight(double newValue);
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    double GetLeft();
    void SetLeft(double newValue);
    double GetTop();
    void SetTop(double newValue);
    double GetWidth();
    void SetWidth(double newValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// ErrorBars wrapper class

class ErrorBars : public COleDispatchDriver
{
public:
    ErrorBars() {}      // Calls COleDispatchDriver default constructor
    ErrorBars(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ErrorBars(const ErrorBars& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    VARIANT ClearFormats();
    long GetEndStyle();
    void SetEndStyle(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// DataTable wrapper class

class DataTable : public COleDispatchDriver
{
public:
    DataTable() {}      // Calls COleDispatchDriver default constructor
    DataTable(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DataTable(const DataTable& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetShowLegendKey();
    void SetShowLegendKey(BOOL bNewValue);
    BOOL GetHasBorderHorizontal();
    void SetHasBorderHorizontal(BOOL bNewValue);
    BOOL GetHasBorderVertical();
    void SetHasBorderVertical(BOOL bNewValue);
    BOOL GetHasBorderOutline();
    void SetHasBorderOutline(BOOL bNewValue);
    LPDISPATCH GetBorder();
    LPDISPATCH GetFont();
    void Select();
    void Delete();
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// Phonetic wrapper class

class Phonetic : public COleDispatchDriver
{
public:
    Phonetic() {}       // Calls COleDispatchDriver default constructor
    Phonetic(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Phonetic(const Phonetic& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    long GetCharacterType();
    void SetCharacterType(long nNewValue);
    long GetAlignment();
    void SetAlignment(long nNewValue);
    LPDISPATCH GetFont();
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Shape wrapper class

class Shape : public COleDispatchDriver
{
public:
    Shape() {}      // Calls COleDispatchDriver default constructor
    Shape(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Shape(const Shape& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Apply();
    void Delete();
    LPDISPATCH Duplicate();
    void Flip(long FlipCmd);
    void IncrementLeft(float Increment);
    void IncrementRotation(float Increment);
    void IncrementTop(float Increment);
    void PickUp();
    void RerouteConnections();
    void ScaleHeight(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
    void ScaleWidth(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
    void Select(const VARIANT& Replace);
    void SetShapesDefaultProperties();
    LPDISPATCH Ungroup();
    void ZOrder(long ZOrderCmd);
    LPDISPATCH GetAdjustments();
    LPDISPATCH GetTextFrame();
    long GetAutoShapeType();
    void SetAutoShapeType(long nNewValue);
    LPDISPATCH GetCallout();
    long GetConnectionSiteCount();
    long GetConnector();
    LPDISPATCH GetConnectorFormat();
    LPDISPATCH GetFill();
    LPDISPATCH GetGroupItems();
    float GetHeight();
    void SetHeight(float newValue);
    long GetHorizontalFlip();
    float GetLeft();
    void SetLeft(float newValue);
    LPDISPATCH GetLine();
    long GetLockAspectRatio();
    void SetLockAspectRatio(long nNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetNodes();
    float GetRotation();
    void SetRotation(float newValue);
    LPDISPATCH GetPictureFormat();
    LPDISPATCH GetShadow();
    LPDISPATCH GetTextEffect();
    LPDISPATCH GetThreeD();
    float GetTop();
    void SetTop(float newValue);
    long GetType();
    long GetVerticalFlip();
    VARIANT GetVertices();
    long GetVisible();
    void SetVisible(long nNewValue);
    float GetWidth();
    void SetWidth(float newValue);
    long GetZOrderPosition();
    LPDISPATCH GetHyperlink();
    long GetBlackWhiteMode();
    void SetBlackWhiteMode(long nNewValue);
    CString GetOnAction();
    void SetOnAction(LPCTSTR lpszNewValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    LPDISPATCH GetTopLeftCell();
    LPDISPATCH GetBottomRightCell();
    long GetPlacement();
    void SetPlacement(long nNewValue);
    void Copy();
    void Cut();
    void CopyPicture(const VARIANT& Appearance, const VARIANT& Format);
    LPDISPATCH GetControlFormat();
    LPDISPATCH GetLinkFormat();
    LPDISPATCH GetOLEFormat();
    long GetFormControlType();
    CString GetAlternativeText();
    void SetAlternativeText(LPCTSTR lpszNewValue);
    LPDISPATCH GetScript();
    LPDISPATCH GetDiagramNode();
    long GetHasDiagramNode();
    LPDISPATCH GetDiagram();
    long GetHasDiagram();
    long GetChild();
    LPDISPATCH GetParentGroup();
    long GetId();
};
/////////////////////////////////////////////////////////////////////////////
// Shapes wrapper class

class Shapes : public COleDispatchDriver
{
public:
    Shapes() {}     // Calls COleDispatchDriver default constructor
    Shapes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Shapes(const Shapes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH _Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH AddCallout(long Type, float Left, float Top, float Width, float Height);
    LPDISPATCH AddConnector(long Type, float BeginX, float BeginY, float EndX, float EndY);
    LPDISPATCH AddCurve(const VARIANT& SafeArrayOfPoints);
    LPDISPATCH AddLabel(long Orientation, float Left, float Top, float Width, float Height);
    LPDISPATCH AddLine(float BeginX, float BeginY, float EndX, float EndY);
    LPDISPATCH AddPicture(LPCTSTR Filename, long LinkToFile, long SaveWithDocument, float Left, float Top, float Width, float Height);
    LPDISPATCH AddPolyline(const VARIANT& SafeArrayOfPoints);
    LPDISPATCH AddShape(long Type, float Left, float Top, float Width, float Height);
    LPDISPATCH AddTextEffect(long PresetTextEffect, LPCTSTR Text, LPCTSTR FontName, float FontSize, long FontBold, long FontItalic, float Left, float Top);
    LPDISPATCH AddTextbox(long Orientation, float Left, float Top, float Width, float Height);
    LPDISPATCH BuildFreeform(long EditingType, float X1, float Y1);
    LPDISPATCH GetRange(const VARIANT& Index);
    void SelectAll();
    LPDISPATCH AddFormControl(long Type, long Left, long Top, long Width, long Height);
    LPDISPATCH AddOLEObject(const VARIANT& ClassType, const VARIANT& Filename, const VARIANT& Link, const VARIANT& DisplayAsIcon, const VARIANT& IconFileName, const VARIANT& IconIndex, const VARIANT& IconLabel, const VARIANT& Left,
                            const VARIANT& Top, const VARIANT& Width, const VARIANT& Height);
    LPDISPATCH AddDiagram(long Type, float Left, float Top, float Width, float Height);
};
/////////////////////////////////////////////////////////////////////////////
// ShapeRange wrapper class

class ShapeRange : public COleDispatchDriver
{
public:
    ShapeRange() {}     // Calls COleDispatchDriver default constructor
    ShapeRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ShapeRange(const ShapeRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH _Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    void Align(long AlignCmd, long RelativeTo);
    void Apply();
    void Delete();
    void Distribute(long DistributeCmd, long RelativeTo);
    LPDISPATCH Duplicate();
    void Flip(long FlipCmd);
    void IncrementLeft(float Increment);
    void IncrementRotation(float Increment);
    void IncrementTop(float Increment);
    LPDISPATCH Group();
    void PickUp();
    void RerouteConnections();
    LPDISPATCH Regroup();
    void ScaleHeight(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
    void ScaleWidth(float Factor, long RelativeToOriginalSize, const VARIANT& Scale);
    void Select(const VARIANT& Replace);
    void SetShapesDefaultProperties();
    LPDISPATCH Ungroup();
    void ZOrder(long ZOrderCmd);
    LPDISPATCH GetAdjustments();
    LPDISPATCH GetTextFrame();
    long GetAutoShapeType();
    void SetAutoShapeType(long nNewValue);
    LPDISPATCH GetCallout();
    long GetConnectionSiteCount();
    long GetConnector();
    LPDISPATCH GetConnectorFormat();
    LPDISPATCH GetFill();
    LPDISPATCH GetGroupItems();
    float GetHeight();
    void SetHeight(float newValue);
    long GetHorizontalFlip();
    float GetLeft();
    void SetLeft(float newValue);
    LPDISPATCH GetLine();
    long GetLockAspectRatio();
    void SetLockAspectRatio(long nNewValue);
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetNodes();
    float GetRotation();
    void SetRotation(float newValue);
    LPDISPATCH GetPictureFormat();
    LPDISPATCH GetShadow();
    LPDISPATCH GetTextEffect();
    LPDISPATCH GetThreeD();
    float GetTop();
    void SetTop(float newValue);
    long GetType();
    long GetVerticalFlip();
    VARIANT GetVertices();
    long GetVisible();
    void SetVisible(long nNewValue);
    float GetWidth();
    void SetWidth(float newValue);
    long GetZOrderPosition();
    long GetBlackWhiteMode();
    void SetBlackWhiteMode(long nNewValue);
    CString GetAlternativeText();
    void SetAlternativeText(LPCTSTR lpszNewValue);
    LPDISPATCH GetDiagramNode();
    long GetHasDiagramNode();
    LPDISPATCH GetDiagram();
    long GetHasDiagram();
    long GetChild();
    LPDISPATCH GetParentGroup();
    long GetId();
};
/////////////////////////////////////////////////////////////////////////////
// GroupShapes wrapper class

class GroupShapes : public COleDispatchDriver
{
public:
    GroupShapes() {}        // Calls COleDispatchDriver default constructor
    GroupShapes(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    GroupShapes(const GroupShapes& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH _Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH GetRange(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// TextFrame wrapper class

class TextFrame : public COleDispatchDriver
{
public:
    TextFrame() {}      // Calls COleDispatchDriver default constructor
    TextFrame(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    TextFrame(const TextFrame& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    float GetMarginBottom();
    void SetMarginBottom(float newValue);
    float GetMarginLeft();
    void SetMarginLeft(float newValue);
    float GetMarginRight();
    void SetMarginRight(float newValue);
    float GetMarginTop();
    void SetMarginTop(float newValue);
    long GetOrientation();
    void SetOrientation(long nNewValue);
    LPDISPATCH Characters(const VARIANT& Start, const VARIANT& Length);
    long GetHorizontalAlignment();
    void SetHorizontalAlignment(long nNewValue);
    long GetVerticalAlignment();
    void SetVerticalAlignment(long nNewValue);
    BOOL GetAutoSize();
    void SetAutoSize(BOOL bNewValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    BOOL GetAutoMargins();
    void SetAutoMargins(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ConnectorFormat wrapper class

class ConnectorFormat : public COleDispatchDriver
{
public:
    ConnectorFormat() {}        // Calls COleDispatchDriver default constructor
    ConnectorFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ConnectorFormat(const ConnectorFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void BeginConnect(LPDISPATCH ConnectedShape, long ConnectionSite);
    void BeginDisconnect();
    void EndConnect(LPDISPATCH ConnectedShape, long ConnectionSite);
    void EndDisconnect();
    long GetBeginConnected();
    LPDISPATCH GetBeginConnectedShape();
    long GetBeginConnectionSite();
    long GetEndConnected();
    LPDISPATCH GetEndConnectedShape();
    long GetEndConnectionSite();
    long GetType();
    void SetType(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// FreeformBuilder wrapper class

class FreeformBuilder : public COleDispatchDriver
{
public:
    FreeformBuilder() {}        // Calls COleDispatchDriver default constructor
    FreeformBuilder(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    FreeformBuilder(const FreeformBuilder& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void AddNodes(long SegmentType, long EditingType, float X1, float Y1, const VARIANT& X2, const VARIANT& Y2, const VARIANT& X3, const VARIANT& Y3);
    LPDISPATCH ConvertToShape();
};
/////////////////////////////////////////////////////////////////////////////
// ControlFormat wrapper class

class ControlFormat : public COleDispatchDriver
{
public:
    ControlFormat() {}      // Calls COleDispatchDriver default constructor
    ControlFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ControlFormat(const ControlFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void AddItem(LPCTSTR Text, const VARIANT& Index);
    void RemoveAllItems();
    void RemoveItem(long Index, const VARIANT& Count);
    long GetDropDownLines();
    void SetDropDownLines(long nNewValue);
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    long GetLargeChange();
    void SetLargeChange(long nNewValue);
    CString GetLinkedCell();
    void SetLinkedCell(LPCTSTR lpszNewValue);
    VARIANT List(const VARIANT& Index);
    long GetListCount();
    void SetListCount(long nNewValue);
    CString GetListFillRange();
    void SetListFillRange(LPCTSTR lpszNewValue);
    long GetListIndex();
    void SetListIndex(long nNewValue);
    BOOL GetLockedText();
    void SetLockedText(BOOL bNewValue);
    long GetMax();
    void SetMax(long nNewValue);
    long GetMin();
    void SetMin(long nNewValue);
    long GetMultiSelect();
    void SetMultiSelect(long nNewValue);
    BOOL GetPrintObject();
    void SetPrintObject(BOOL bNewValue);
    long GetSmallChange();
    void SetSmallChange(long nNewValue);
    long Get_Default();
    void Set_Default(long nNewValue);
    long GetValue();
    void SetValue(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// OLEFormat wrapper class

class OLEFormat : public COleDispatchDriver
{
public:
    OLEFormat() {}      // Calls COleDispatchDriver default constructor
    OLEFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OLEFormat(const OLEFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Activate();
    LPDISPATCH GetObject();
    CString GetProgID();
    void Verb(const VARIANT& Verb);
};
/////////////////////////////////////////////////////////////////////////////
// LinkFormat wrapper class

class LinkFormat : public COleDispatchDriver
{
public:
    LinkFormat() {}     // Calls COleDispatchDriver default constructor
    LinkFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    LinkFormat(const LinkFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetAutoUpdate();
    void SetAutoUpdate(BOOL bNewValue);
    BOOL GetLocked();
    void SetLocked(BOOL bNewValue);
    void Update();
};
/////////////////////////////////////////////////////////////////////////////
// PublishObjects wrapper class

class PublishObjects : public COleDispatchDriver
{
public:
    PublishObjects() {}     // Calls COleDispatchDriver default constructor
    PublishObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PublishObjects(const PublishObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(long SourceType, LPCTSTR Filename, const VARIANT& Sheet, const VARIANT& Source, const VARIANT& HtmlType, const VARIANT& DivID, const VARIANT& Title);
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    void Delete();
    void Publish();
};
/////////////////////////////////////////////////////////////////////////////
// OLEDBError wrapper class

class OLEDBError : public COleDispatchDriver
{
public:
    OLEDBError() {}     // Calls COleDispatchDriver default constructor
    OLEDBError(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OLEDBError(const OLEDBError& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetSqlState();
    CString GetErrorString();
    long GetNative();
    long GetNumber();
    long GetStage();
};
/////////////////////////////////////////////////////////////////////////////
// OLEDBErrors wrapper class

class OLEDBErrors : public COleDispatchDriver
{
public:
    OLEDBErrors() {}        // Calls COleDispatchDriver default constructor
    OLEDBErrors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    OLEDBErrors(const OLEDBErrors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(long Index);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Phonetics wrapper class

class Phonetics : public COleDispatchDriver
{
public:
    Phonetics() {}      // Calls COleDispatchDriver default constructor
    Phonetics(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Phonetics(const Phonetics& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    long GetStart();
    long GetLength();
    BOOL GetVisible();
    void SetVisible(BOOL bNewValue);
    long GetCharacterType();
    void SetCharacterType(long nNewValue);
    long GetAlignment();
    void SetAlignment(long nNewValue);
    LPDISPATCH GetFont();
    LPDISPATCH GetItem(long Index);
    void Delete();
    void Add(long Start, long Length, LPCTSTR Text);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    LPDISPATCH Get_Default(long Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// PivotLayout wrapper class

class PivotLayout : public COleDispatchDriver
{
public:
    PivotLayout() {}        // Calls COleDispatchDriver default constructor
    PivotLayout(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotLayout(const PivotLayout& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetPivotTable();
};
/////////////////////////////////////////////////////////////////////////////
// DisplayUnitLabel wrapper class

class DisplayUnitLabel : public COleDispatchDriver
{
public:
    DisplayUnitLabel() {}       // Calls COleDispatchDriver default constructor
    DisplayUnitLabel(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    DisplayUnitLabel(const DisplayUnitLabel& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    VARIANT Select();
    LPDISPATCH GetBorder();
    VARIANT Delete();
    LPDISPATCH GetInterior();
    LPDISPATCH GetFill();
    CString GetCaption();
    void SetCaption(LPCTSTR lpszNewValue);
    LPDISPATCH GetCharacters(const VARIANT& Start, const VARIANT& Length);
    LPDISPATCH GetFont();
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    double GetLeft();
    void SetLeft(double newValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    BOOL GetShadow();
    void SetShadow(BOOL bNewValue);
    CString GetText();
    void SetText(LPCTSTR lpszNewValue);
    double GetTop();
    void SetTop(double newValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    long GetReadingOrder();
    void SetReadingOrder(long nNewValue);
    VARIANT GetAutoScaleFont();
    void SetAutoScaleFont(const VARIANT& newValue);
};
/////////////////////////////////////////////////////////////////////////////
// CellFormat wrapper class

class CellFormat : public COleDispatchDriver
{
public:
    CellFormat() {}     // Calls COleDispatchDriver default constructor
    CellFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CellFormat(const CellFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetBorders();
    void SetRefBorders(LPDISPATCH newValue);
    LPDISPATCH GetFont();
    void SetRefFont(LPDISPATCH newValue);
    LPDISPATCH GetInterior();
    void SetRefInterior(LPDISPATCH newValue);
    VARIANT GetNumberFormat();
    void SetNumberFormat(const VARIANT& newValue);
    VARIANT GetNumberFormatLocal();
    void SetNumberFormatLocal(const VARIANT& newValue);
    VARIANT GetAddIndent();
    void SetAddIndent(const VARIANT& newValue);
    VARIANT GetIndentLevel();
    void SetIndentLevel(const VARIANT& newValue);
    VARIANT GetHorizontalAlignment();
    void SetHorizontalAlignment(const VARIANT& newValue);
    VARIANT GetVerticalAlignment();
    void SetVerticalAlignment(const VARIANT& newValue);
    VARIANT GetOrientation();
    void SetOrientation(const VARIANT& newValue);
    VARIANT GetShrinkToFit();
    void SetShrinkToFit(const VARIANT& newValue);
    VARIANT GetWrapText();
    void SetWrapText(const VARIANT& newValue);
    VARIANT GetLocked();
    void SetLocked(const VARIANT& newValue);
    VARIANT GetFormulaHidden();
    void SetFormulaHidden(const VARIANT& newValue);
    VARIANT GetMergeCells();
    void SetMergeCells(const VARIANT& newValue);
    void Clear();
};
/////////////////////////////////////////////////////////////////////////////
// UsedObjects wrapper class

class UsedObjects : public COleDispatchDriver
{
public:
    UsedObjects() {}        // Calls COleDispatchDriver default constructor
    UsedObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    UsedObjects(const UsedObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// CustomProperties wrapper class

class CustomProperties : public COleDispatchDriver
{
public:
    CustomProperties() {}       // Calls COleDispatchDriver default constructor
    CustomProperties(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CustomProperties(const CustomProperties& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Name, const VARIANT& Value);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// CustomProperty wrapper class

class CustomProperty : public COleDispatchDriver
{
public:
    CustomProperty() {}     // Calls COleDispatchDriver default constructor
    CustomProperty(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CustomProperty(const CustomProperty& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    VARIANT GetValue();
    void SetValue(const VARIANT& newValue);
    VARIANT Get_Default();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// CalculatedMembers wrapper class

class CalculatedMembers : public COleDispatchDriver
{
public:
    CalculatedMembers() {}      // Calls COleDispatchDriver default constructor
    CalculatedMembers(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CalculatedMembers(const CalculatedMembers& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH Add(LPCTSTR Name, LPCTSTR Formula, const VARIANT& SolveOrder, const VARIANT& Type);
};
/////////////////////////////////////////////////////////////////////////////
// CalculatedMember wrapper class

class CalculatedMember : public COleDispatchDriver
{
public:
    CalculatedMember() {}       // Calls COleDispatchDriver default constructor
    CalculatedMember(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    CalculatedMember(const CalculatedMember& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    CString GetFormula();
    CString GetSourceName();
    long GetSolveOrder();
    BOOL GetIsValid();
    CString Get_Default();
    void Delete();
    long GetType();
};
/////////////////////////////////////////////////////////////////////////////
// Watches wrapper class

class Watches : public COleDispatchDriver
{
public:
    Watches() {}        // Calls COleDispatchDriver default constructor
    Watches(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Watches(const Watches& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Source);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// Watch wrapper class

class Watch : public COleDispatchDriver
{
public:
    Watch() {}      // Calls COleDispatchDriver default constructor
    Watch(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Watch(const Watch& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    VARIANT GetSource();
};
/////////////////////////////////////////////////////////////////////////////
// PivotCell wrapper class

class PivotCell : public COleDispatchDriver
{
public:
    PivotCell() {}      // Calls COleDispatchDriver default constructor
    PivotCell(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotCell(const PivotCell& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetPivotCellType();
    LPDISPATCH GetPivotTable();
    LPDISPATCH GetDataField();
    LPDISPATCH GetPivotField();
    LPDISPATCH GetPivotItem();
    LPDISPATCH GetRowItems();
    LPDISPATCH GetColumnItems();
    LPDISPATCH GetRange();
    long GetCustomSubtotalFunction();
};
/////////////////////////////////////////////////////////////////////////////
// Graphic wrapper class

class Graphic : public COleDispatchDriver
{
public:
    Graphic() {}        // Calls COleDispatchDriver default constructor
    Graphic(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Graphic(const Graphic& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    float GetBrightness();
    void SetBrightness(float newValue);
    long GetColorType();
    void SetColorType(long nNewValue);
    float GetContrast();
    void SetContrast(float newValue);
    float GetCropBottom();
    void SetCropBottom(float newValue);
    float GetCropLeft();
    void SetCropLeft(float newValue);
    float GetCropRight();
    void SetCropRight(float newValue);
    float GetCropTop();
    void SetCropTop(float newValue);
    CString GetFilename();
    void SetFilename(LPCTSTR lpszNewValue);
    float GetHeight();
    void SetHeight(float newValue);
    long GetLockAspectRatio();
    void SetLockAspectRatio(long nNewValue);
    float GetWidth();
    void SetWidth(float newValue);
};
/////////////////////////////////////////////////////////////////////////////
// AutoRecover wrapper class

class AutoRecover : public COleDispatchDriver
{
public:
    AutoRecover() {}        // Calls COleDispatchDriver default constructor
    AutoRecover(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AutoRecover(const AutoRecover& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    long GetTime();
    void SetTime(long nNewValue);
    CString GetPath();
    void SetPath(LPCTSTR lpszNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// ErrorCheckingOptions wrapper class

class ErrorCheckingOptions : public COleDispatchDriver
{
public:
    ErrorCheckingOptions() {}       // Calls COleDispatchDriver default constructor
    ErrorCheckingOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ErrorCheckingOptions(const ErrorCheckingOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetBackgroundChecking();
    void SetBackgroundChecking(BOOL bNewValue);
    long GetIndicatorColorIndex();
    void SetIndicatorColorIndex(long nNewValue);
    BOOL GetEvaluateToError();
    void SetEvaluateToError(BOOL bNewValue);
    BOOL GetTextDate();
    void SetTextDate(BOOL bNewValue);
    BOOL GetNumberAsText();
    void SetNumberAsText(BOOL bNewValue);
    BOOL GetInconsistentFormula();
    void SetInconsistentFormula(BOOL bNewValue);
    BOOL GetOmittedCells();
    void SetOmittedCells(BOOL bNewValue);
    BOOL GetUnlockedFormulaCells();
    void SetUnlockedFormulaCells(BOOL bNewValue);
    BOOL GetEmptyCellReferences();
    void SetEmptyCellReferences(BOOL bNewValue);
    BOOL GetListDataValidation();
    void SetListDataValidation(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Errors wrapper class

class Errors : public COleDispatchDriver
{
public:
    Errors() {}     // Calls COleDispatchDriver default constructor
    Errors(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Errors(const Errors& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
};
/////////////////////////////////////////////////////////////////////////////
// Error wrapper class

class Error : public COleDispatchDriver
{
public:
    Error() {}      // Calls COleDispatchDriver default constructor
    Error(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Error(const Error& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetValue();
    BOOL GetIgnore();
    void SetIgnore(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// SmartTagAction wrapper class

class SmartTagAction : public COleDispatchDriver
{
public:
    SmartTagAction() {}     // Calls COleDispatchDriver default constructor
    SmartTagAction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTagAction(const SmartTagAction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetName();
    void Execute();
    CString Get_Default();
    long GetType();
    BOOL GetPresentInPane();
    BOOL GetExpandHelp();
    void SetExpandHelp(BOOL bNewValue);
    BOOL GetCheckboxState();
    void SetCheckboxState(BOOL bNewValue);
    CString GetTextboxText();
    void SetTextboxText(LPCTSTR lpszNewValue);
    long GetListSelection();
    void SetListSelection(long nNewValue);
    long GetRadioGroupSelection();
    void SetRadioGroupSelection(long nNewValue);
    LPDISPATCH GetActiveXControl();
};
/////////////////////////////////////////////////////////////////////////////
// SmartTagActions wrapper class

class SmartTagActions : public COleDispatchDriver
{
public:
    SmartTagActions() {}        // Calls COleDispatchDriver default constructor
    SmartTagActions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTagActions(const SmartTagActions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// SmartTag wrapper class

class SmartTag : public COleDispatchDriver
{
public:
    SmartTag() {}       // Calls COleDispatchDriver default constructor
    SmartTag(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTag(const SmartTag& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString GetDownloadURL();
    CString GetName();
    CString Get_Default();
    CString GetXml();
    LPDISPATCH GetRange();
    void Delete();
    LPDISPATCH GetSmartTagActions();
    LPDISPATCH GetProperties();
};
/////////////////////////////////////////////////////////////////////////////
// SmartTags wrapper class

class SmartTags : public COleDispatchDriver
{
public:
    SmartTags() {}      // Calls COleDispatchDriver default constructor
    SmartTags(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTags(const SmartTags& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR SmartTagType);
    long GetCount();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// SmartTagRecognizer wrapper class

class SmartTagRecognizer : public COleDispatchDriver
{
public:
    SmartTagRecognizer() {}     // Calls COleDispatchDriver default constructor
    SmartTagRecognizer(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTagRecognizer(const SmartTagRecognizer& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    BOOL GetEnabled();
    void SetEnabled(BOOL bNewValue);
    CString GetProgID();
    CString Get_Default();
    CString GetFullName();
};
/////////////////////////////////////////////////////////////////////////////
// SmartTagRecognizers wrapper class

class SmartTagRecognizers : public COleDispatchDriver
{
public:
    SmartTagRecognizers() {}        // Calls COleDispatchDriver default constructor
    SmartTagRecognizers(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTagRecognizers(const SmartTagRecognizers& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    BOOL GetRecognize();
    void SetRecognize(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// SmartTagOptions wrapper class

class SmartTagOptions : public COleDispatchDriver
{
public:
    SmartTagOptions() {}        // Calls COleDispatchDriver default constructor
    SmartTagOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SmartTagOptions(const SmartTagOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetDisplaySmartTags();
    void SetDisplaySmartTags(long nNewValue);
    BOOL GetEmbedSmartTags();
    void SetEmbedSmartTags(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// SpellingOptions wrapper class

class SpellingOptions : public COleDispatchDriver
{
public:
    SpellingOptions() {}        // Calls COleDispatchDriver default constructor
    SpellingOptions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    SpellingOptions(const SpellingOptions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long GetDictLang();
    void SetDictLang(long nNewValue);
    CString GetUserDict();
    void SetUserDict(LPCTSTR lpszNewValue);
    BOOL GetIgnoreCaps();
    void SetIgnoreCaps(BOOL bNewValue);
    BOOL GetSuggestMainOnly();
    void SetSuggestMainOnly(BOOL bNewValue);
    BOOL GetIgnoreMixedDigits();
    void SetIgnoreMixedDigits(BOOL bNewValue);
    BOOL GetIgnoreFileNames();
    void SetIgnoreFileNames(BOOL bNewValue);
    BOOL GetGermanPostReform();
    void SetGermanPostReform(BOOL bNewValue);
    BOOL GetKoreanCombineAux();
    void SetKoreanCombineAux(BOOL bNewValue);
    BOOL GetKoreanUseAutoChangeList();
    void SetKoreanUseAutoChangeList(BOOL bNewValue);
    BOOL GetKoreanProcessCompound();
    void SetKoreanProcessCompound(BOOL bNewValue);
    long GetHebrewModes();
    void SetHebrewModes(long nNewValue);
    long GetArabicModes();
    void SetArabicModes(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Speech wrapper class

class Speech : public COleDispatchDriver
{
public:
    Speech() {}     // Calls COleDispatchDriver default constructor
    Speech(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Speech(const Speech& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    void Speak(LPCTSTR Text, const VARIANT& SpeakAsync, const VARIANT& SpeakXML, const VARIANT& Purge);
    long GetDirection();
    void SetDirection(long nNewValue);
    BOOL GetSpeakCellOnEnter();
    void SetSpeakCellOnEnter(BOOL bNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// Protection wrapper class

class Protection : public COleDispatchDriver
{
public:
    Protection() {}     // Calls COleDispatchDriver default constructor
    Protection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Protection(const Protection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    BOOL GetAllowFormattingCells();
    BOOL GetAllowFormattingColumns();
    BOOL GetAllowFormattingRows();
    BOOL GetAllowInsertingColumns();
    BOOL GetAllowInsertingRows();
    BOOL GetAllowInsertingHyperlinks();
    BOOL GetAllowDeletingColumns();
    BOOL GetAllowDeletingRows();
    BOOL GetAllowSorting();
    BOOL GetAllowFiltering();
    BOOL GetAllowUsingPivotTables();
    LPDISPATCH GetAllowEditRanges();
};
/////////////////////////////////////////////////////////////////////////////
// PivotItemList wrapper class

class PivotItemList : public COleDispatchDriver
{
public:
    PivotItemList() {}      // Calls COleDispatchDriver default constructor
    PivotItemList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    PivotItemList(const PivotItemList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long GetCount();
    LPDISPATCH Item(const VARIANT& Index);
    LPDISPATCH Get_Default(const VARIANT& Field);
    LPUNKNOWN _NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// Tab wrapper class

class Tab : public COleDispatchDriver
{
public:
    Tab() {}        // Calls COleDispatchDriver default constructor
    Tab(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Tab(const Tab& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    VARIANT GetColor();
    void SetColor(const VARIANT& newValue);
    long GetColorIndex();
    void SetColorIndex(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// AllowEditRanges wrapper class

class AllowEditRanges : public COleDispatchDriver
{
public:
    AllowEditRanges() {}        // Calls COleDispatchDriver default constructor
    AllowEditRanges(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AllowEditRanges(const AllowEditRanges& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Title, LPDISPATCH Range, const VARIANT& Password);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// AllowEditRange wrapper class

class AllowEditRange : public COleDispatchDriver
{
public:
    AllowEditRange() {}     // Calls COleDispatchDriver default constructor
    AllowEditRange(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    AllowEditRange(const AllowEditRange& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    CString GetTitle();
    void SetTitle(LPCTSTR lpszNewValue);
    LPDISPATCH GetRange();
    void SetRefRange(LPDISPATCH newValue);
    void ChangePassword(LPCTSTR Password);
    void Delete();
    void Unprotect(const VARIANT& Password);
    LPDISPATCH GetUsers();
};
/////////////////////////////////////////////////////////////////////////////
// UserAccessList wrapper class

class UserAccessList : public COleDispatchDriver
{
public:
    UserAccessList() {}     // Calls COleDispatchDriver default constructor
    UserAccessList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    UserAccessList(const UserAccessList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long GetCount();
    LPDISPATCH GetItem(const VARIANT& Index);
    LPDISPATCH Add(LPCTSTR Name, BOOL AllowEdit);
    void DeleteAll();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// UserAccess wrapper class

class UserAccess : public COleDispatchDriver
{
public:
    UserAccess() {}     // Calls COleDispatchDriver default constructor
    UserAccess(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    UserAccess(const UserAccess& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    CString GetName();
    BOOL GetAllowEdit();
    void SetAllowEdit(BOOL bNewValue);
    void Delete();
};
/////////////////////////////////////////////////////////////////////////////
// RTD wrapper class

class RTD : public COleDispatchDriver
{
public:
    RTD() {}        // Calls COleDispatchDriver default constructor
    RTD(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    RTD(const RTD& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    long GetThrottleInterval();
    void SetThrottleInterval(long nNewValue);
    void RefreshData();
    void RestartServers();
};
/////////////////////////////////////////////////////////////////////////////
// Diagram wrapper class

class Diagram : public COleDispatchDriver
{
public:
    Diagram() {}        // Calls COleDispatchDriver default constructor
    Diagram(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    Diagram(const Diagram& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetNodes();
    long GetType();
    long GetAutoLayout();
    void SetAutoLayout(long nNewValue);
    long GetReverse();
    void SetReverse(long nNewValue);
    long GetAutoFormat();
    void SetAutoFormat(long nNewValue);
    void Convert(long Type);
    void FitText();
};
/////////////////////////////////////////////////////////////////////////////
// ListObjects wrapper class

class ListObjects : public COleDispatchDriver
{
public:
    ListObjects() {}        // Calls COleDispatchDriver default constructor
    ListObjects(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListObjects(const ListObjects& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(long SourceType, const VARIANT& Source, const VARIANT& LinkSource, long XlListObjectHasHeaders, const VARIANT& Destination);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH GetItem(const VARIANT& Index);
    long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// ListObject wrapper class

class ListObject : public COleDispatchDriver
{
public:
    ListObject() {}     // Calls COleDispatchDriver default constructor
    ListObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListObject(const ListObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    CString Publish(const VARIANT& Target, BOOL LinkSource);
    void Refresh();
    void Unlink();
    void Unlist();
    void UpdateChanges(long iConflictType);
    void Resize(LPDISPATCH Range);
    CString Get_Default();
    BOOL GetActive();
    LPDISPATCH GetDataBodyRange();
    BOOL GetDisplayRightToLeft();
    LPDISPATCH GetHeaderRowRange();
    LPDISPATCH GetInsertRowRange();
    LPDISPATCH GetListColumns();
    LPDISPATCH GetListRows();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetQueryTable();
    LPDISPATCH GetRange();
    BOOL GetShowAutoFilter();
    void SetShowAutoFilter(BOOL bNewValue);
    BOOL GetShowTotals();
    void SetShowTotals(BOOL bNewValue);
    long GetSourceType();
    LPDISPATCH GetTotalsRowRange();
    CString GetSharePointURL();
    LPDISPATCH GetXmlMap();
};
/////////////////////////////////////////////////////////////////////////////
// ListColumns wrapper class

class ListColumns : public COleDispatchDriver
{
public:
    ListColumns() {}        // Calls COleDispatchDriver default constructor
    ListColumns(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListColumns(const ListColumns& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Position);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH GetItem(const VARIANT& Index);
    long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// ListColumn wrapper class

class ListColumn : public COleDispatchDriver
{
public:
    ListColumn() {}     // Calls COleDispatchDriver default constructor
    ListColumn(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListColumn(const ListColumn& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    CString Get_Default();
    LPDISPATCH GetListDataFormat();
    long GetIndex();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    LPDISPATCH GetRange();
    long GetTotalsCalculation();
    void SetTotalsCalculation(long nNewValue);
    LPDISPATCH GetXPath();
    CString GetSharePointFormula();
};
/////////////////////////////////////////////////////////////////////////////
// ListRows wrapper class

class ListRows : public COleDispatchDriver
{
public:
    ListRows() {}       // Calls COleDispatchDriver default constructor
    ListRows(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListRows(const ListRows& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(const VARIANT& Position);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPUNKNOWN Get_NewEnum();
    LPDISPATCH GetItem(const VARIANT& Index);
    long GetCount();
};
/////////////////////////////////////////////////////////////////////////////
// ListRow wrapper class

class ListRow : public COleDispatchDriver
{
public:
    ListRow() {}        // Calls COleDispatchDriver default constructor
    ListRow(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListRow(const ListRow& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    void Delete();
    long GetIndex();
    BOOL GetInvalidData();
    LPDISPATCH GetRange();
};
/////////////////////////////////////////////////////////////////////////////
// XmlNamespace wrapper class

class XmlNamespace : public COleDispatchDriver
{
public:
    XmlNamespace() {}       // Calls COleDispatchDriver default constructor
    XmlNamespace(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlNamespace(const XmlNamespace& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString Get_Default();
    CString GetUri();
    CString GetPrefix();
};
/////////////////////////////////////////////////////////////////////////////
// XmlNamespaces wrapper class

class XmlNamespaces : public COleDispatchDriver
{
public:
    XmlNamespaces() {}      // Calls COleDispatchDriver default constructor
    XmlNamespaces(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlNamespaces(const XmlNamespaces& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    long GetCount();
    CString GetValue();
    LPUNKNOWN Get_NewEnum();
    void InstallManifest(LPCTSTR Path, const VARIANT& InstallForAllUsers);
};
/////////////////////////////////////////////////////////////////////////////
// XmlDataBinding wrapper class

class XmlDataBinding : public COleDispatchDriver
{
public:
    XmlDataBinding() {}     // Calls COleDispatchDriver default constructor
    XmlDataBinding(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlDataBinding(const XmlDataBinding& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString Get_Default();
    long Refresh();
    void LoadSettings(LPCTSTR Url);
    void ClearSettings();
    CString GetSourceUrl();
};
/////////////////////////////////////////////////////////////////////////////
// XmlSchema wrapper class

class XmlSchema : public COleDispatchDriver
{
public:
    XmlSchema() {}      // Calls COleDispatchDriver default constructor
    XmlSchema(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlSchema(const XmlSchema& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH GetNamespace();
    CString GetXml();
    CString GetName();
};
/////////////////////////////////////////////////////////////////////////////
// XmlSchemas wrapper class

class XmlSchemas : public COleDispatchDriver
{
public:
    XmlSchemas() {}     // Calls COleDispatchDriver default constructor
    XmlSchemas(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlSchemas(const XmlSchemas& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    long GetCount();
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// XmlMap wrapper class

class XmlMap : public COleDispatchDriver
{
public:
    XmlMap() {}     // Calls COleDispatchDriver default constructor
    XmlMap(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlMap(const XmlMap& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString Get_Default();
    CString GetName();
    void SetName(LPCTSTR lpszNewValue);
    BOOL GetIsExportable();
    BOOL GetShowImportExportValidationErrors();
    void SetShowImportExportValidationErrors(BOOL bNewValue);
    BOOL GetSaveDataSourceDefinition();
    void SetSaveDataSourceDefinition(BOOL bNewValue);
    BOOL GetAdjustColumnWidth();
    void SetAdjustColumnWidth(BOOL bNewValue);
    BOOL GetPreserveColumnFilter();
    void SetPreserveColumnFilter(BOOL bNewValue);
    BOOL GetPreserveNumberFormatting();
    void SetPreserveNumberFormatting(BOOL bNewValue);
    BOOL GetAppendOnImport();
    void SetAppendOnImport(BOOL bNewValue);
    CString GetRootElementName();
    LPDISPATCH GetRootElementNamespace();
    LPDISPATCH GetSchemas();
    LPDISPATCH GetDataBinding();
    void Delete();
    long Import(LPCTSTR Url, const VARIANT& Overwrite);
    long ImportXml(LPCTSTR XmlData, const VARIANT& Overwrite);
    long Export(LPCTSTR Url, const VARIANT& Overwrite);
    long ExportXml(BSTR* Data);
};
/////////////////////////////////////////////////////////////////////////////
// XmlMaps wrapper class

class XmlMaps : public COleDispatchDriver
{
public:
    XmlMaps() {}        // Calls COleDispatchDriver default constructor
    XmlMaps(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XmlMaps(const XmlMaps& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    LPDISPATCH Add(LPCTSTR Schema, const VARIANT& RootElementName);
    LPDISPATCH Get_Default(const VARIANT& Index);
    LPDISPATCH GetItem(const VARIANT& Index);
    long GetCount();
    LPUNKNOWN Get_NewEnum();
};
/////////////////////////////////////////////////////////////////////////////
// ListDataFormat wrapper class

class ListDataFormat : public COleDispatchDriver
{
public:
    ListDataFormat() {}     // Calls COleDispatchDriver default constructor
    ListDataFormat(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    ListDataFormat(const ListDataFormat& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    long Get_Default();
    VARIANT GetChoices();
    long GetDecimalPlaces();
    VARIANT GetDefaultValue();
    BOOL GetIsPercent();
    long GetLcid();
    long GetMaxCharacters();
    VARIANT GetMaxNumber();
    VARIANT GetMinNumber();
    BOOL GetRequired();
    long GetType();
    BOOL GetReadOnly();
    BOOL GetAllowFillIn();
};
/////////////////////////////////////////////////////////////////////////////
// XPath wrapper class

class XPath : public COleDispatchDriver
{
public:
    XPath() {}      // Calls COleDispatchDriver default constructor
    XPath(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
    XPath(const XPath& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
    LPDISPATCH GetApplication();
    long GetCreator();
    LPDISPATCH GetParent();
    CString Get_Default();
    CString GetValue();
    LPDISPATCH GetMap();
    void SetValue(LPDISPATCH Map, LPCTSTR XPath, const VARIANT& SelectionNamespace, const VARIANT& Repeating);
    void Clear();
    BOOL GetRepeating();
};
