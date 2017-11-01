#define WXWIN GetEnv('WXWIN')
#define WXDLL "MSVC"

#define ApplicationVersion GetFileVersion('msvc/x64/Release/fra-airtraffic.scr')

[Setup]
; Registry key HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{AppId}_is1
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
ChangesEnvironment=yes
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
Source: {#WXWIN}/lib/gcc_dll32/wxbase311u_gcc_flederwiesel.dll;     DestDir: {app}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/gcc_dll32/wxmsw311u_core_gcc_flederwiesel.dll; DestDir: {app}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/gcc_dll64/wxbase311u_gcc_flederwiesel.dll;     DestDir: {app}; Flags: ignoreversion; Check:     Is64BitInstallMode
Source: {#WXWIN}/lib/gcc_dll64/wxmsw311u_core_gcc_flederwiesel.dll; DestDir: {app}; Flags: ignoreversion; Check:     Is64BitInstallMode
#else
Source: {#WXWIN}/lib/vc_dll/wxbase311u_vc_flederwiesel.dll;             DestDir: {app}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/vc_dll/wxmsw311u_core_vc_flederwiesel.dll;         DestDir: {app}; Flags: ignoreversion; Check: NOT Is64BitInstallMode
Source: {#WXWIN}/lib/vc_x64_dll/wxbase311u_vc_x64_flederwiesel.dll;     DestDir: {app}; Flags: ignoreversion; Check:     Is64BitInstallMode
Source: {#WXWIN}/lib/vc_x64_dll/wxmsw311u_core_vc_x64_flederwiesel.dll; DestDir: {app}; Flags: ignoreversion; Check:     Is64BitInstallMode

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

; Some sources also mention this path:
;HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\Control Panel\Desktop
;HKCU\Software\Policies\Microsoft\Control Panel

Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: SCRNSAVE.EXE; ValueType: string; ValueData: %SystemRoot%\System32\fra-airtraffic.scr
Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: ScreenSaveActive; ValueType: string; ValueData: 1
Root: HKU; Subkey: .DEFAULT\Control Panel\Desktop; ValueName: ScreenSaveTimeOut; ValueType: string; ValueData: 180

[Run]
Filename: "{tmp}\vcredist_x86.exe"; Parameters: "/q /norestart /q:a /c:""VCREDI~3.EXE /q:a /c:""""msiexec /i vcredist.msi /qn"""" """; WorkingDir: {tmp}; StatusMsg: "Installing VC++ 2010 Redistributable..."; Check: NOT Is64BitInstallMode And VCRedistNeedsInstallX86
Filename: "{tmp}\vcredist_x64.exe"; Parameters: "/q /norestart /q:a /c:""VCREDI~3.EXE /q:a /c:""""msiexec /i vcredist.msi /qn"""" """; WorkingDir: {tmp}; StatusMsg: "Installing VC++ 2010 Redistributable..."; Check:     Is64BitInstallMode And VCRedistNeedsInstallX64
Filename: "{sys}\rundll32.exe"; Parameters: "{sys}\shell32.dll,Control_RunDLL desk.cpl,fra-airtraffic,@ScreenSaver"; Description: "{cm:ControlPanel}"; Flags: postinstall skipifsilent

[CustomMessages]
en.screensaver=screensaver
de.screensaver=Bildschirmschoner

en.AppComments=Screensaver displaying air traffic to/from Frankfurt International Airport.
de.AppComments=Bildschirmschoner, der Flüge von/nach Frankfurt International Airport anzeigt.

en.finished=Setup has finished installing fra-airtraffic screensaver on your computer.
de.finished=Das Setup hat die Installation von fra-airtraffic Bildschirmschoner auf Ihrem Computer abgeschlossen.

en.ClickFinish=Click Finish to exit Setup.
de.ClickFinish=Klicken Sie auf "Fertigstellen", um das Setup zu beenden.

en.ErrorAppendPath=%nSetup was unable to modify your PATH environment variable.%nPlease add "{app}" manually, otherwise the screensaver will not start.%n
de.ErrorAppendPath=%nSetup konnte die PATH-Umgebungsvariable nicht ändern.%nBitte "{app}" manuell hinzufügen, sonst kann der Bildschirmschoner nicht gestartet werden.%n

en.ControlPanel=Lanuch control panel to adjust screensaver settings.
de.ControlPanel=Systemsteuerung aufrufen, um Bildschirmschoner-Einstellungen vorzunehmen.

[Code]
var errorinfo: String;

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
	ShellExec('open', 'https://www.innosetup.com', '', '', SW_SHOW, ewNoWait, Err);
end;

procedure InitializeWizard();
var
	URLLabel: TNewStaticText;
begin
	URLLabel := TNewStaticText.Create(WizardForm);
	URLLabel.Top := WizardForm.CancelButton.Top +
					WizardForm.CancelButton.Height - URLLabel.Height - 2;
	URLLabel.Left := 20;
	URLLabel.Caption := 'www.innosetup.com';
	URLLabel.Font.Style := URLLabel.Font.Style + [fsUnderLine];
	URLLabel.Font.Color := clBlue;
	URLLabel.Cursor := crHand;
	URLLabel.OnClick := @URLLabelOnClick;
	URLLabel.Parent := WizardForm;
end;

procedure FindPathEntry(key, location: string; var path: String; var off: Integer);
var
	compare: String;
	entry: String;
	len: integer;
	start: integer;
begin
	// get environment from registry instead of setup's environment
	if (RegQueryStringValue(HKEY_LOCAL_MACHINE,
		'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
		key, path) <> True) then
	begin
		errorinfo := ExpandConstant('{cm:ErrorAppendPath}');
		StringChangeEx(errorinfo, '{app}', location, True);
	end
	else
	begin
		// compare all lowercase, but keep original
		location := LowerCase(location);
		compare := Trim(LowerCase(path));
		len := Length(compare);
		start := 0;
		// don't regard direction of path separators
		StringChangeEx(location, '/', '\', True);
		StringChangeEx(compare, '/', '\', True);

		repeat
			off := Pos(';', compare);

			if (off = 0) then
			begin
				// end of string
				len := 0;
				entry := compare
			end
			else
			begin
				len := len - off;
				entry := Copy(compare, 1, off - 1);
			end;

			entry := Trim(entry);

			if (entry = location) then
				off := start + 1 // for ';'
			else
			begin
				if (off > 0) then
				begin
					start := start + off;
					compare := Copy(compare, off + 1, len);
				end
			end;
		until len <= 0;
	end;
end;

procedure CurPageChanged(CurPageID: Integer);
var
	text: String;
	ErrorLabel: TNewStaticText;
	ClickLabel: TNewStaticText;
begin
	if (wpFinished = CurPageID) then
	begin
		// if `errorinfo` is set, display message on the wpFinished page,
		// as this is less annoying than a message box
		if (Length(errorinfo) > 0) then
			StringChangeEx(text, '{$errorinfo}', errorinfo, true)
		else
			StringChangeEx(text, '{$errorinfo}', '', true);

		WizardForm.FinishedLabel.Caption := ExpandConstant('{cm:finished}');
		WizardForm.FinishedLabel.AutoSize := True;

		ErrorLabel := TNewStaticText.Create(WizardForm);

		ErrorLabel.Parent := WizardForm.FinishedPage;
		// this must be set before Top/Left/Width
		ErrorLabel.AutoSize := True	;
		ErrorLabel.WordWrap := True;
		ErrorLabel.Left := WizardForm.FinishedLabel.Left;
		ErrorLabel.Top := WizardForm.FinishedLabel.Top + WizardForm.FinishedLabel.Height;
		ErrorLabel.Width := WizardForm.FinishedLabel.Width;
		ErrorLabel.Font.Style := [fsBold];
		ErrorLabel.Caption := errorinfo;

		ClickLabel := TNewStaticText.Create(WizardForm);

		ClickLabel.Parent := WizardForm.FinishedPage;
		ClickLabel.Left := ErrorLabel.Left;
		ClickLabel.Top := ErrorLabel.Top + ErrorLabel.Height;
		ClickLabel.Caption := ExpandConstant('{cm:ClickFinish}');
		ClickLabel.AutoSize := True;
	end
end;

procedure CurStepChanged(CurStep: TSetupStep);
var
	location: String;
	path: String;
	off: Integer;
begin
	if (ssPostInstall = CurStep) then
	begin
		// add {app} to path, if not already done by previous installs
		location := Trim(ExpandConstant('{app}'));

		FindPathEntry('PATH', location, path, off);

		if (off = 0) then
		begin
			// location not found within path
			if (RegWriteStringValue(HKEY_LOCAL_MACHINE,
				'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
				'PATH', path + ';' + location) <> True) then
			begin
				errorinfo := ExpandConstant('{cm:ErrorAppendPath}');
				StringChangeEx(errorinfo, '{app}', location, True);
			end
		end
	end
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
var
	location: String;
	path: String;
	len: Integer;
	off: Integer;
	total: Integer;
begin
	if (usPostUninstall = CurUninstallStep) then
	begin
		// remove {app} from path
		location := Trim(ExpandConstant('{app}'));

		FindPathEntry('PATH', location, path, off);

		len := Length(location);
		total := Length(path);

		if (off > 0) then
		begin
			if (off > 1) then
				path := Copy(path, 1, off - 2) +
						Copy(path, off + len, total - off - 2)
			else
			begin
				if (len < total) then
					if (Copy(path, len + 1, 1) = ';') then
						len := len + 1;

				path := Copy(path, off + len, total - off - 2);
			end;

			RegWriteStringValue(HKEY_LOCAL_MACHINE,
				'SYSTEM\CurrentControlSet\Control\Session Manager\Environment',
				'PATH', path);
		end
	end
end;
