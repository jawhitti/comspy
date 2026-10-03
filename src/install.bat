echo off
echo registering control spy hook dll...
regsvr32 /s ctlspysrv.dll

echo registering proxy/stub dlls...
regsvr32 /s ctlspysrvps.dll
regsvr32 /s debuggerps.dll

echo registering hook agent...
agent /RegServer

start .
