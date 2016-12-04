unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

  ITest = interface
    procedure DoIt;
  end;

  TTestObj = class(TObject, ITest)
  private
    FRef: Integer;
  public
    constructor Create;
    destructor Destroy;override;
    procedure DoIt;
    //implement functions of IInterface
    function QueryInterface(const IID: TGUID; out Obj): HResult; stdcall;
    function _AddRef: Integer; stdcall;
    function _Release: Integer; stdcall;
  end;
var
  Form1: TForm1;

implementation

{$R *.dfm}

{ TTestObj }

constructor TTestObj.Create;
begin

end;

destructor TTestObj.Destroy;
begin

  inherited;
end;

procedure TTestObj.DoIt;
begin

end;

var
  G: Integer;
procedure TForm1.Button1Click(Sender: TObject);
var
  IT: ITest;
  Obj: TTestObj;
begin
  Obj := TTestObj.Create;
  IT := Obj;
  IT.DoIt;
  Obj.Free;
  G := 1;
end;
  
function TTestObj._AddRef: Integer;
begin
  FRef := FRef + 1;
  Result := FRef;
end;

function TTestObj._Release: Integer;
begin
  FRef := FRef - 1;
  Result := FRef;
  if FRef = 0 then
    Free;
end;

function TTestObj.QueryInterface(const IID: TGUID; out Obj): HResult;
begin
  if GetInterface(IID, Obj) then
    Result := 0
  else
    Result := E_NOINTERFACE;
end;

end.
