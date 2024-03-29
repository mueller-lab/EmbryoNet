; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "FishLabeler_Eval"
#define MyAppVersion "1.5"
#define MyAppPublisher "Computer Vision Studio"
#define MyAppURL "cvfn.corporative@gmail.com"
#define PathToRelease "M:\Embryo_project_sources_and_dependences\embryo_project\build\CVStudio_sources\FishLabeler\Release"
#define MyAppExeName "FishLabeler.exe"
#define PathToIcon "M:\Embryo_installer_scripts\installerScripts\icons\EmbryoLabeler.ico"
[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{ED2DE1D4-29CA-48CC-B513-41A67748B888}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:/{#MyAppName}
DisableProgramGroupPage=yes
; Uncomment the following line to run in non administrative install mode (install for current user only.)
;PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog
OutputDir=Output/
OutputBaseFilename=EmbryoLabeler_Eval_Setup
SetupIconFile={#PathToIcon}
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#PathToRelease}\FishLabeler.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PathToRelease}\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PathToRelease}\DefaultPath.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PathToIcon}"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#PathToRelease}\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PathToRelease}\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#PathToRelease}\imageformats\*"; DestDir: "{app}\imageformats\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#PathToRelease}\platforms\*"; DestDir: "{app}\platforms\"; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon
[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

