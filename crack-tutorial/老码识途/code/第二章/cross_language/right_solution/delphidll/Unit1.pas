unit Unit1;

interface

function Add(I1: Integer; I2: Integer; I3: Integer; I4: Integer): Integer;cdecl;

implementation

function Add(I1: Integer; I2: Integer; I3: Integer; I4: Integer): Integer;
begin
  Result := I1 + I2 + I3 + I4;
end;

end.
 