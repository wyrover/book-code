unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, SyncObjs;

const
  READ_NUM = 1023;
  LOOP_NUM_GET_OUTPUT = 10;
  LOOP_MS_GET_OUTPUT = 100;

type
  TCommThd = class(TThread)
  private
    Fpi: _Process_Information;
    readHnd: THandle;
    writeHnd: THandle;
    FMsg: PChar;
    FMsgBlock: PChar;
    FBlockLen: Integer;
    FHasCmd: TEvent;
    procedure WriteMsg(Msg: PChar);
    procedure WriteBlock(Msg: PChar; Len: Integer);
    procedure Log;
    procedure LogBlock;
    function RunTelnet: Boolean;
    function printReturnMsg: Boolean;
    procedure GetCmd;
    function BuildNewUnicode(Msg: String; var ByteLen: Integer): PChar;
  protected
    procedure Execute;override;
  public
    constructor Create;
    destructor Destroy;override;
    procedure SetCmd(Cmd: String);
    procedure Close;
  end;

  TForm1 = class(TForm)
    Memo1: TMemo;
    Edit1: TEdit;
    procedure Edit1KeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure FormCreate(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
  private
    { Private declarations }
    FThd: TCommThd;
  public
    { Public declarations }
  end;

  function inject(hProcess: THandle; Dll: PChar): Integer;cdecl; external 'inject.dll';

var
  Form1: TForm1;

implementation

{$R *.dfm}

var
  gCmd: String;
{ TCommThd }

function TCommThd.BuildNewUnicode(Msg: String; var ByteLen: Integer): PChar;
var
  unicodeLen, Len: Integer;
  pUnicode: PChar;
begin
  unicodeLen := MultiByteToWideChar( CP_ACP,
		0,
		@Msg[1],
		-1,
		nil,
		0 );     //返回长度为包含了NULL字符的字符长度
  Len := (unicodeLen)* 2;
  GetMem(pUnicode, Len);
  fillchar(pUnicode^,Len, 0);
  MultiByteToWideChar( CP_ACP,
		0,
		@Msg[1],
		-1,
		PWideChar(pUnicode),
		unicodeLen);
  Result := pUnicode;
  ByteLen := Len;
end;

procedure TCommThd.Close;
begin
  TerminateProcess(FPi.hProcess, 0);
end;

constructor TCommThd.Create;
begin
  inherited Create(false);
  FHasCmd := TEvent.Create(nil, False, False, '');
end;

destructor TCommThd.Destroy;
begin
  FHasCmd.Free;
  inherited;
end;

procedure TCommThd.Execute;
var
  n: Cardinal;
  CmdLen: Integer;
  uniCmd: PChar;
  uniCmdByteLen: Integer;
begin
  if (not RunTelnet)then
  begin
    Exit;
  end;
  printReturnMsg;
  while (true) do
  begin
    GetCmd;
    if gCmd = 'exit' then
    begin
      TerminateProcess(Fpi.hProcess, 0);
      Exit;
    end;
    CmdLen := Length(gCmd);
    gCmd := gCmd + #10;
    uniCmd := BuildNewUnicode(gCmd, uniCmdByteLen);
    if (not WriteFile(writeHnd, uniCmd[0], uniCmdByteLen, n, nil)) then
    begin
      WriteMsg('cannot write to pipe to telnet');
    end;
    FreeMem(uniCmd);
    if (not printReturnMsg) then
    begin
      TerminateProcess(Fpi.hProcess, 0);
      Exit;
    end;
  end;
end;

procedure TCommThd.GetCmd;
begin
  FHasCmd.WaitFor(INFINITE);
end;

procedure TCommThd.Log;
begin
  Form1.Memo1.Lines.Add(FMsg);
end;

procedure TCommThd.LogBlock;
begin
  Form1.Memo1.Lines.Add(FMsgBlock);
  FreeMem(FMsgBlock);
end;

function TCommThd.printReturnMsg: Boolean;
var
  output: array[0..1023] of char;
  n, n2: Cardinal;
  I: Integer;
begin
   for i := 0 to LOOP_NUM_GET_OUTPUT - 1 do//循环几次将剩余的数据读完
   begin
     while (true) do
     begin
     	if (not PeekNamedPipe(readHnd, nil, 0, nil, @n2, nil)) then
     	begin
       	  WriteMsg('cannot get msg');
	  Result := false;
          Exit;
	end;
	if (n2 = 0) then
	  break;
	if (not ReadFile(readHnd, output[0], READ_NUM, n2, nil)) then
	begin
	  WriteMsg('cannot read file');
	  continue;
	end;
        WriteBlock(output, n2);
     end;
     Sleep(LOOP_MS_GET_OUTPUT);//休眠一下等待剩余的数据
   end;
end;

function TCommThd.RunTelnet: Boolean;
var
  rh1, wh1, rh2, wh2: THANDLE;
  si: _StartupInfoA;
  sa: TSECURITYATTRIBUTES;
begin
	sa.nLength := sizeof(sa);
	sa.lpSecurityDescriptor := nil;
	sa.bInheritHandle := true;
	if (not CreatePipe(rh1, wh1, @sa, 1024)) then
	begin
	   WriteMsg('cannot create pipe');
	   Result := false;
           Exit;
	end;
        readHnd := rh1;
	if (not CreatePipe(rh2, wh2, @sa, 1024)) then
	begin
	   WriteMsg('cannot create pipe2');
	   Result := false;
           Exit;
	end;
       writeHnd := wh2;
	fillchar(si, sizeof(si), 0);

	si.dwFlags := STARTF_USESHOWWINDOW or STARTF_USESTDHANDLES;
	si.wShowWindow := SW_HIDE;
	si.hStdInput := rh2;
	si.hStdOutput := wh1;
	si.hStdError := wh1;
	if (not CreateProcessA(nil, 'telnet.exe', nil, nil, true, CREATE_SUSPENDED
		, nil, nil, si, Fpi)) then
	begin
   	  WriteMsg('cannot create cmd');
	  Result := false;
          Exit;
	end;
	Sleep(500);
	//Log("inject dll\n");
        if (inject(Fpi.hProcess, 'hooker.dll') = 0) then
        begin
          WriteMsg('cannot inject');
          Result := False;
          Exit;
        end;
	{if (!DetourContinueProcessWithDllA(pi->hProcess, "hooker.dll")) then
        begin
		printf("inject: DetourContinueProcessWithDll(%s) failed: %d",
			"hooker.dll", GetLastError());
		return 3;
	end;}
	ResumeThread(Fpi.hThread);

	CloseHandle(rh2);
	CloseHandle(wh1);
end;

procedure TCommThd.SetCmd(Cmd: String);
begin
  gCmd := Cmd;
  FHasCmd.SetEvent;
end;

procedure TCommThd.WriteBlock(Msg: PChar; Len: Integer);
var
  iTextLen: Integer;
  pElementText: PChar;
  tem: PChar;
begin
  FBlockLen := Len;
  {//handle unicode,
  GetMem(tem, Len + 2);
  FillChar(tem^, Len + 2, 0);
  Move(Msg^, Tem^, Len);
  iTextLen := WideCharToMultiByte(CP_ACP, 0, PWideChar(Tem), -1, nil, 0, nil, nil);
  GetMem(pElementText, iTextLen + 1);
  fillchar(pElementText^, sizeof( char ) * ( iTextLen + 1 ) , 0);
  WideCharToMultiByte(CP_ACP,0, PWideChar(Tem),-1,pElementText,iTextLen,nil,nil);
  FreeMem(tem);
  }
  GetMem(tem, Len + 1);
  FillChar(tem^, Len + 1, 0);
  Move(Msg^, Tem^, Len);
  FMsgBlock := Tem;
  Synchronize(LogBlock);
end;

procedure TCommThd.WriteMsg(Msg: PChar);
begin
  FMsg := Msg;
  Synchronize(Log);
end;

procedure TForm1.Edit1KeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  if Key = 13 then
  begin
    Self.Memo1.Lines[Self.Memo1.Lines.Count - 1] := Self.Memo1.Lines[Self.Memo1.Lines.Count - 1] + Edit1.Text; 
    FThd.SetCmd(Edit1.Text);
    Edit1.Text := ''; 
  end;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  FThd := TCommThd.Create;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  FThd.Close;
  TerminateThread(FThd.Handle, 0);
end;

end.

