#Directs the argument calls to other python scripts

import os, sys;
import subprocess;

TOOLS_DIR = "tools"

def RunCommand(cmd):
    subprocess.call(["python", "{}/{}/{}.py".format(os.getcwd(), TOOLS_DIR, cmd)])

for i in range(1,len(sys.argv)):
    cmd = sys.argv[i]
    RunCommand(cmd);
