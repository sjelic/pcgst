import subprocess
import os

try:
    subprocess.run(["make"],check=True)
except subprocess.CalledProcessError:
    exit("Greska prilikom pozivanja make")
except FileNotFoundError:
    exit("Greska prilikom pozivanja make")

dir = "../data"
exe_file = "./izvrsni"

for filename in os.listdir(dir):
    if not filename.endswith(".stp"):
        continue
    print("Testing ",filename)
    command = [exe_file,filename]
    try:
        subprocess.run(command,check=True)
    except subprocess.CalledProcessError:
        exit("Greska prilikom pozivanja ",command )
    except FileNotFoundError:
        exit("Greska prilikom pozivanja ",command )
    print('--------------------------------------------------')

try:
    subprocess.run(["make","clean"],check=True)
except subprocess.CalledProcessError:
    exit("Greska prilikom pozivanja make clean")
except FileNotFoundError:
    exit("Greska prilikom pozivanja make clean")


