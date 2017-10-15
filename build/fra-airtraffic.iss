#define WXWIN GetEnv('WXWIN')
#define WXDLL "MSVC"

#define ApplicationVersion GetFileVersion('msvc/x64/Release/fra-airtraffic.scr')

[Setup]
; Registry key HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{AppId}
AppId=fra-airtraffic
; Wizard AppName
AppName=fra-airtraffic {cm:screensaver}
; Control Panel "Version" + Setup.exe Versioninfo "Product Version"
AppVersion={#ApplicationVersion}
; Control Panel "Name"
AppVerName=fra-airtraffic {cm:screensaver}
; Control Panel "Contact"
AppContact=Tobias Kühne
; Control Panel "Publisher"
AppPublisher=flederwiesel
; Control Panel URLs
AppPublisherURL=https://github.com/flederwiesel/fra-airtraffic
AppSupportURL=https://github.com/flederwiesel/fra-airtraffic/issues
AppUpdatesURL=https://github.com/flederwiesel/fra-airtraffic/releases
; Control Panel "Comments"
AppComments={cm:appcomments}

; Setup.exe Versioninfo "Copyright"
AppCopyright=©2017 Tobias Kühne
; Setup.exe Versioninfo "File Version"
VersionInfoVersion={#ApplicationVersion}
; Setup.exe Versioninfo "File Version" on <= Win98
VersionInfoTextVersion={#ApplicationVersion}
; Setup.exe Versioninfo "Product Name"
VersionInfoProductName=fra-airtraffic
; Setup.exe Versioninfo "Comments"
VersionInfoDescription=screensaver displaying air traffic @FRA

PrivilegesRequired=admin
ArchitecturesInstallIn64BitMode=x64

DefaultDirName={pf}/flederwiesel/fra-airtraffic
CreateAppDir=yes
UsePreviousAppDir=yes
DirExistsWarning=no
EnableDirDoesntExistWarning=no

WindowVisible=no
DisableProgramGroupPage=yes
ShowComponentSizes=no
LicenseFile=../LICENSE
SetupIconFile=../img/JA805A.ico
WizardImageFile=../img/setup_watermrk.bmp
WizardSmallImageFile=../img/setup_header.bmp
Compression=lzma/max
OutputDir=.
OutputBaseFilename=fra-airtraffic-{#ApplicationVersion}
; signtool.exe sign $p $f
SignTool=signtool /t http://time.certum.pl /n "Open Source Developer, Tobias Kühne" /d "fra-airtraffic.scr"
SignedUninstaller=yes

UninstallFilesDir=uninst
UninstallDisplayIcon={sys}/fra-airtraffic.scr

[Languages]
Name: en; MessagesFile: compiler:default.isl
Name: de; MessagesFile: compiler:languages\german.isl

[Files]
Source: msvc/Win32/Release/fra-airtraffic.scr; DestDir: {sys}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: msvc/x64/Release/fra-airtraffic.scr;   DestDir: {sys}; Flags: ignoreversion; Check:     Is64BitInstallMode

#if (WXDLL == "GCC")
; currently TDM-GCC compiled version does not work on the login screen:
; https://sourceforge.net/p/tdm-gcc/bugs/324/
Source: {#WXWIN}/lib/gcc_dll32/wxbase311u_gcc_flederwiesel.dll;     DestDir: {sys}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/gcc_dll32/wxmsw311u_core_gcc_flederwiesel.dll; DestDir: {sys}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/gcc_dll64/wxbase311u_gcc_flederwiesel.dll;     DestDir: {sys}; Flags: ignoreversion; Check:     Is64BitInstallMode
Source: {#WXWIN}/lib/gcc_dll64/wxmsw311u_core_gcc_flederwiesel.dll; DestDir: {sys}; Flags: ignoreversion; Check:     Is64BitInstallMode
#else
Source: {#WXWIN}/lib/vc_dll/wxbase311u_vc_flederwiesel.dll;             DestDir: {sys}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/vc_dll/wxmsw311u_core_vc_flederwiesel.dll;         DestDir: {sys}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/vc_x64_dll/wxbase311u_vc_x64_flederwiesel.dll;     DestDir: {sys}; Flags: ignoreversion; Check:     Is64BitInstallMode
Source: {#WXWIN}/lib/vc_x64_dll/wxmsw311u_core_vc_x64_flederwiesel.dll; DestDir: {sys}; Flags: ignoreversion; Check:     Is64BitInstallMode

Source: msvc/redist/vcredist_x86.exe; DestDir: {tmp}; Flags: deleteafterinstall; Check: NOT Is64BitInstallMode
Source: msvc/redist/vcredist_x64.exe; DestDir: {tmp}; Flags: deleteafterinstall; Check:     Is64BitInstallMode
#endif

Source: ../img/clock.png; DestDir: {app}/img; Flags: recursesubdirs ignoreversion
Source: ../img/bluemarble/1920x960/*; DestDir: {app}/img/1920x960; Flags: recursesubdirs ignoreversion; Excludes: bmng_tb
Source: ../json/schedule.json; DestDir: {commonappdata}/flederwiesel/fra-airtraffic; Flags: ignoreversion; Permissions: authusers-modify everyone-readexec; Excludes: test.json

[Registry]
; https://technet.microsoft.com/en-us/library/cc978622.aspx
; Specifies the name of the screen saver executable file.
Root: HKCU; Subkey: Control Panel\Desktop; ValueName: SCRNSAVE.EXE; ValueType: string; ValueData: %SystemRoot%\System32\fra-airtraffic.scr
; https://technet.microsoft.com/en-us/library/cc978620.aspx
; Specifies whether a screen saver is selected.
Root: HKCU; Subkey: Control Panel\Desktop; ValueName: ScreenSaveActive; ValueType: string; ValueData: 1
; https://technet.microsoft.com/en-us/library/cc978621.aspx
; Specifies the how long the system remains idle before the screen saver starts.
Root: HKCU; Subkey: Control Panel\Desktop; ValueName: ScreenSaveTimeOut; ValueType: string; ValueData: 180
; https://technet.microsoft.com/en-us/library/cc959646.aspx
; Specifies whether the screen saver is password-protected. When a screen saver is password-protected, the user must enter the correct password to end the screen saver.
Root: HKCU; Subkey: Control Panel\Desktop; ValueName: ScreenSaverIsSecure; ValueType: string; ValueData: 0

; Some sources also mention this path:
;HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\Control Panel\Desktop
;HKCU\Software\Policies\Microsoft\Control Panel

Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: SCRNSAVE.EXE; ValueType: string; ValueData: %SystemRoot%\System32\fra-airtraffic.scr
Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: ScreenSaveActive; ValueType: string; ValueData: 1
Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: ScreenSaveTimeOut; ValueType: string; ValueData: 180
Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: ScreenSaverIsSecure; ValueType: string; ValueData: 0

[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/q /norestart /q:a /c:""VCREDI~3.EXE /q:a /c:""""msiexec /i vcredist.msi /qn"""" """; WorkingDir: {tmp}; StatusMsg: "Installing VC++ 2010 Redistributable..."; Check: NOT Is64BitInstallMode And VCRedistNeedsInstallX86
Filename: "{tmp}\vcredist_x64.exe"; Parameters: "/q /norestart /q:a /c:""VCREDI~3.EXE /q:a /c:""""msiexec /i vcredist.msi /qn"""" """; WorkingDir: {tmp}; StatusMsg: "Installing VC++ 2010 Redistributable..."; Check:     Is64BitInstallMode And VCRedistNeedsInstallX64

[CustomMessages]
en.screensaver=screensaver
de.screensaver=Bildschirmschoner

en.appcomments=Screensaver displaying air traffic to/from Frankfurt International Airport.
de.appcomments=Bildschirmschoner, der Flüge von/nach Frankfurt International Airport anzeigt.

[Code]
{ https://stackoverflow.com/questions/11137424/how-to-make-vcredist-x86-reinstall-only-if-not-yet-installed#answer-11172939 }
#IFDEF UNICODE
	#DEFINE AW "W"
#ELSE
	#DEFINE AW "A"
#ENDIF
type
	INSTALLSTATE = Longint;
const
	INSTALLSTATE_DEFAULT = 5;  { The product is installed for the current user. }

	{ http://www.microsoft.com/downloads/details.aspx?FamilyID=C32F406A-F8FC-4164-B6EB-5328B8578F03 }
	VC_2010_SP1_REDIST_X86 = '{F0C3E5D1-1ADE-321E-8167-68EF0DE699A5}';
	{ http://www.microsoft.com/downloads/details.aspx?FamilyID=C68CCBB6-75EF-4C9D-A326-879EAB4FCDF8 }
	VC_2010_SP1_REDIST_X64 = '{1D8E6291-B0D5-35EC-8441-6616F567A0F7}';

function MsiQueryProductState(szProduct: string): INSTALLSTATE;
  external 'MsiQueryProductState{#AW}@msi.dll stdcall';

function VCVersionInstalled(const ProductID: string): Boolean;
begin
	Result := MsiQueryProductState(ProductID) = INSTALLSTATE_DEFAULT;
end;

function VCRedistNeedsInstallX86: Boolean;
begin
	Result := not VCVersionInstalled(VC_2010_SP1_REDIST_X86);
end;

function VCRedistNeedsInstallX64: Boolean;
begin
	Result := not VCVersionInstalled(VC_2010_SP1_REDIST_X64);
end;

procedure URLLabelOnClick(Sender: TObject);
var
	Err: Integer;
begin
	ShellExec('open', 'http://www.innosetup.com', '', '', SW_SHOW, ewNoWait, Err);
end;

procedure InitializeWizard();
var
	CancelButton: TButton;
	URLLabel: TNewStaticText;
begin
	CancelButton := WizardForm.CancelButton;

	URLLabel := TNewStaticText.Create(WizardForm);
	URLLabel.Top := CancelButton.Top + CancelButton.Height - URLLabel.Height - 2;
	URLLabel.Left := 20;
	URLLabel.Caption := 'www.innosetup.com';
	URLLabel.Font.Style := URLLabel.Font.Style + [fsUnderLine];
	URLLabel.Font.Color := clBlue;
	URLLabel.Cursor := crHand;
	URLLabel.OnClick := @URLLabelOnClick;
	URLLabel.Parent := WizardForm;
end;
