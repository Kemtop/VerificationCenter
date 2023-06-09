; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "���� �������"
#define MyAppVersion "1.4"
#define MyAppPublisher "��������������� ���"
#define MyAppURL "http://kaznalnr.ru"
#define CRKAppName "���"
#define CRKAppExeName "KeyDistributionCenter.exe"
#define KeyGenAppName "��������� ������"
#define KeyGenAppExeName "KeyGen.exe"
#define CryptAppName "������� ����������"
#define CryptAppExeName "GranitCrypt.exe"


[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{96A63BF8-5A27-4344-8760-555A4860EC90}
AppName={#CryptAppName} {#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\GranitK
DefaultGroupName=�������
DisableWelcomePage=no
OutputDir=.
OutputBaseFilename=Crypt_setup
;Password=1
Compression=lzma
LicenseFile=.\InstallerFiles\license.rtf
;Compression=lzma2/ultra
SolidCompression=yes
SetupIconFile=.\GranitCrypt\Image\app.ico
WizardImageFile=.\InstallerFiles\WizardImage.bmp
WizardSmallImageFile=.\InstallerFiles\WizardImageSmall.bmp

[Languages]
Name: "russian"; MessagesFile: "compiler:Languages\Russian.isl"

[Messages]
WelcomeLabel2=��������� ��������� [name/ver] �� ��� ���������.%n%n������������� ������� ��� ������ ���������� ����� ���, ��� ����������.%n%n� ������ ������������� ������ ���������� � ������������.

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkedonce

[Types]
Name: "Crypt"; Description: "��������� ������� ����������"; Flags: iscustom

[Components]
Name: "Crypt"; Description: "������� ������������ ����������"; Types: Crypt; Flags: fixed

[Files]
; ����� �������������
Source: ".\GranitCrypt\*"; DestDir: "{app}\GranitCrypt"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\iconengines\*"; DestDir: "{app}\GranitCrypt\iconengines"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\Image\*"; DestDir: "{app}\GranitCrypt\Image"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\imageformats\*"; DestDir: "{app}\GranitCrypt\imageformats"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\platforms\*"; DestDir: "{app}\GranitCrypt\platforms"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\sqldrivers\*"; DestDir: "{app}\GranitCrypt\sqldrivers"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\styles\*"; DestDir: "{app}\GranitCrypt\styles"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ".\GranitCrypt\translations\*"; DestDir: "{app}\GranitCrypt\translations"; Components: Crypt; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\ {#CryptAppName}"; Filename: "{app}\GranitCrypt\{#CryptAppExeName}"; IconFilename: "{app}\GranitCrypt\Image\app.ico"; Components: Crypt
Name: "{group}\�������� {#CryptAppName} {#MyAppName}"; Filename: "{uninstallexe}"
Name: "{commondesktop}\{#CryptAppName}"; Filename: "{app}\GranitCrypt\{#CryptAppExeName}"; IconFilename: "{app}\GranitCrypt\Image\app.ico"; Components: Crypt;  Tasks: desktopicon

[Code]
#IFDEF UNICODE
  #DEFINE AW "W"
#ELSE
  #DEFINE AW "A"
#ENDIF

procedure ExitProcess(exitCode:integer);
  external 'ExitProcess@kernel32.dll stdcall';

procedure ExitInstall(exitCode:integer);
begin
  DelTree(ExpandConstant('{app}'), True, True, True);
  ExitProcess(exitCode);
end;

function SetFocus(hWnd: HWND): HWND;
  external 'SetFocus@user32.dll stdcall';
function OpenClipboard(hWndNewOwner: HWND): BOOL;
  external 'OpenClipboard@user32.dll stdcall';
function GetClipboardData(uFormat: UINT): THandle;
  external 'GetClipboardData@user32.dll stdcall';
function CloseClipboard: BOOL;
  external 'CloseClipboard@user32.dll stdcall';
function GlobalLock(hMem: THandle): PAnsiChar;
  external 'GlobalLock@kernel32.dll stdcall';
function GlobalUnlock(hMem: THandle): BOOL;
  external 'GlobalUnlock@kernel32.dll stdcall';

var
  SerialPage: TWizardPage;
  SerialEdits: array of TEdit;

const
  CF_TEXT = 1;
  VK_BACK = 8;
  SC_EDITCOUNT = 5;
  SC_CHARCOUNT = 10;
  SC_DELIMITER = '-';

function GetClipboardText: string;
var
  Data: THandle;
begin
  Result := '';
  if OpenClipboard(0) then
  try
    Data := GetClipboardData(CF_TEXT);
    if Data <> 0 then
      Result := String(GlobalLock(Data));
  finally
    if Data <> 0 then
      GlobalUnlock(Data);
    CloseClipboard;
  end;
end;

function GetSerialNumber(ADelimiter: Char): string;
var
  I: Integer;
begin
  Result := '';
  for I := 0 to GetArrayLength(SerialEdits) - 1 do
    Result := Result + SerialEdits[I].Text + ADelimiter;
  Delete(Result, Length(Result), 1);
end;

function IsValidInput: Boolean;
var
  I: Integer;
begin
  Result:=True;
  for I := 0 to GetArrayLength(SerialEdits) - 1 do
    if Length(SerialEdits[I].Text) < SC_CHARCOUNT then
    begin
      Result := False;
      Break;
    end;
  Result :=  GetSerialNumber(SC_DELIMITER) = '1111111111-1111111111-1111111111-1111111111-1111111111'
  end;

function TrySetSerialNumber(const ASerialNumber: string; ADelimiter: Char): Boolean;
var
  I: Integer;
  J: Integer;
begin
  Result := False;

  if Length(ASerialNumber) = ((SC_EDITCOUNT * SC_CHARCOUNT) + (SC_EDITCOUNT - 1)) then
  begin
    for I := 1 to SC_EDITCOUNT - 1 do
      if ASerialNumber[(I * SC_CHARCOUNT) + I] <> ADelimiter then
        Exit;

    for I := 0 to GetArrayLength(SerialEdits) - 1 do
    begin
      J := (I * SC_CHARCOUNT) + I + 1;
      SerialEdits[I].Text := Copy(ASerialNumber, J, SC_CHARCOUNT);
    end;

    Result := True;
  end;
end;

function TryPasteSerialNumber: Boolean;
begin
  Result := TrySetSerialNumber(GetClipboardText, SC_DELIMITER);
end;

procedure OnSerialEditChange(Sender: TObject);
begin
  WizardForm.NextButton.Enabled := IsValidInput;
end;

procedure OnSerialEditKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
var
  Edit: TEdit;
  EditIndex: Integer;
begin
  Edit := TEdit(Sender);
  EditIndex := Edit.TabOrder - SerialEdits[0].TabOrder;
  if (EditIndex = 0) and (Key = Ord('V')) and (Shift = [ssCtrl]) then
  begin
    if TryPasteSerialNumber then
      Key := 0;
  end
  else
  if (Key >= 32) and (Key <= 255) then
  begin
    if Length(Edit.Text) = SC_CHARCOUNT - 1 then
    begin
      if EditIndex < GetArrayLength(SerialEdits) - 1 then
        SetFocus(SerialEdits[EditIndex + 1].Handle)
      else
        SetFocus(WizardForm.NextButton.Handle);
    end;
  end
  else
  if Key = VK_BACK then
    if (EditIndex > 0) and (Edit.Text = '') and (Edit.SelStart = 0) then
      SetFocus(SerialEdits[EditIndex - 1].Handle);
end;

procedure CreateSerialNumberPage;
var
  I: Integer;
  Edit: TEdit;
  DescLabel: TLabel;
  EditWidth: Integer;
begin
  SerialPage := CreateCustomPage(wpLicense, '�������� ��������� ������',
    '������� �������� ����� ��������');

  DescLabel := TLabel.Create(SerialPage);
  DescLabel.Top := 16;
  DescLabel.Left := 0;
  DescLabel.Parent := SerialPage.Surface;
  DescLabel.Caption := '�������� �����:';
  DescLabel.Font.Style := [fsBold];

  SetArrayLength(SerialEdits, SC_EDITCOUNT);
  EditWidth := (SerialPage.SurfaceWidth - ((SC_EDITCOUNT - 1) * 8)) div SC_EDITCOUNT;

  for I := 0 to SC_EDITCOUNT - 1 do
  begin
    Edit := TEdit.Create(SerialPage);
    Edit.Top := 40;
    Edit.Left := I * (EditWidth + 8);
    Edit.Width := EditWidth;
    Edit.CharCase := ecUpperCase;
    Edit.MaxLength := SC_CHARCOUNT;
    Edit.Parent := SerialPage.Surface;
    Edit.OnChange := @OnSerialEditChange;
    Edit.OnKeyDown := @OnSerialEditKeyDown;
    SerialEdits[I] := Edit;
  end;
end;

procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = SerialPage.ID then
    WizardForm.NextButton.Enabled := IsValidInput;  
end;


procedure InitializeWizard;
begin
  CreateSerialNumberPage;
end;

function IsWindowsVersionOrNewer(Major, Minor: Integer): Boolean;
var
  Version: TWindowsVersion;
begin
  GetWindowsVersionEx(Version);
  Result :=
    (Version.Major > Major) or
    ((Version.Major = Major) and (Version.Minor >= Minor));
end;



function InitializeSetup: Boolean;
begin
  if not IsWindowsVersionOrNewer(6, 1) then
  begin 
    MsgBox(
      '��� ��������� ���������� ���������� �� Windows 7 ��� ����� ������� ������',
      mbCriticalError, MB_OK);
    Result:=False;
    Exit;
  end;
  Result:=True;
end;
