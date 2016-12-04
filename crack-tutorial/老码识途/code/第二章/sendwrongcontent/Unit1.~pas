unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, Winsock, ScktComp, StdCtrls;

type
  TForm1 = class(TForm)
    Button1: TButton;
    procedure FormCreate(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.dfm}

procedure TForm1.FormCreate(Sender: TObject);
var
  wsa: WSADATA;
begin
  WSAStartup($0101, wsa);
end;

procedure TForm1.Button1Click(Sender: TObject);
var
  sock: Cardinal;
  addr: sockaddr_in;
  pbuf: PChar;
begin
  sock := socket(AF_INET,SOCK_DGRAM,0);
  if (sock < 0) then
  begin
    Exit;
  end;
  fillchar(addr, sizeof(sockaddr_in), 0);
  addr.sin_addr.s_addr := inet_addr('127.0.0.1');
  addr.sin_family := AF_INET;
  addr.sin_port := HTONS(2000);
  GetMem(pbuf, 5);
  StrCopy(pbuf, 'hello');
  sendto(sock, pbuf, 5, 0, addr,  sizeof(sockaddr_in));
  closesocket(sock);
end;

end.
