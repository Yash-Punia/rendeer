#Calls the premake executable to generate make file and use make to generate the build

import subprocess, os

cmd = "premake\premake5.exe gmake"

subprocess.call(["powershell", "-Command", cmd])
subprocess.call(["powershell", "-Command", "make"])
