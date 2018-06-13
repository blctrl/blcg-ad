$ENV{HARNESS_ACTIVE} = 1;
$ENV{TOP} = 'D:/epics/ad/seq-2.2.5';
$ENV{PATH} = 'D:/epics/ad/seq-2.2.5/bin/windows-x64;d:/epics/base-3.14.12.7/bin/windows-x64;d:/epics/base-3.14.12.7/bin/windows-x64;;;;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Tools/MSVC/14.13.26128/bin/HostX64/x64;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/VC/VCPackages;C:/Program Files (x86)/Microsoft SDKs/TypeScript/2.6;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TestWindow;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/CommonExtensions/Microsoft/TeamFoundation/Team Explorer;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/MSBuild/15.0/bin/Roslyn;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools/x64;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Team Tools/Performance Tools;C:/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools//x64;C:/Program Files (x86)/Microsoft Visual Studio/Shared/Common/VSPerfCollectionTools/;C:/Program Files (x86)/Microsoft SDKs/Windows/v10.0A/bin/NETFX 4.6.1 Tools/x64/;C:/Program Files (x86)/Windows Kits/10/bin/10.0.16299.0/x64;C:/Program Files (x86)/Windows Kits/10/bin/x64;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community//MSBuild/15.0/bin;C:/Windows/Microsoft.NET/Framework64/v4.0.30319;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/;C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/Tools/;D:/epics/bin;C:/Strawberry/perl/bin;C:/WINDOWS/system32;C:/WINDOWS;C:/WINDOWS/SYSTEM32/Wbem;D:/epics/base-3.14.12.7/bin/windows-x64;D:/epics/base-3.14.12.7/../extensions/bin/windows-x64';
$ENV{EPICS_CA_SERVER_PORT} = 10000 + $$ % 30000;
#only for debugging:
#print STDERR "port=$ENV{EPICS_CA_SERVER_PORT}\n";
require Win32::Process;
my $child_proc;
Win32::Process::Create($child_proc, "./reassign.exe", "reassign.exe -S -d ../reassign.db", 0, 0, ".") || die "Win32::Process::Create() failed: $!";
my $pid = $child_proc->GetProcessID();
system("./reassign.exe -S -t");
kill 9, $pid or die "kill failed: $!";
