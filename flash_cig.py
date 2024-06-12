import telnetlib
import sys
import subprocess
from datetime import datetime, timedelta
from sys import platform

import time

# Printing process time function
def printProcessTime(startTime:datetime):
    elapsedTime = datetime.now() - startTime
    elapsedTime = elapsedTime.total_seconds()
    print(f"Info : Process took: {elapsedTime} seconds")

# Check the commandline arguments
if len(sys.argv)<2:
    print("Usage: python flash_cig.py [flash device (jlink/st-link)] [path/to/firmware.bin] \n Path to .bin is most likely something like '[project folder]/build/zephyr/zephyr.bin'")
    sys.exit(1)

# Define which command to use to open the GDB server depending on the flashing device
if sys.argv[1] == "jlink" or sys.argv[1] == "Jlink" or sys.argv[1] == "JLINK":
    print("Using jlink, command still needs to be made")
    sys.exit(1)
    startGdbCommand = "openocd -f interface/jlink.cfg -c 'transport select swd' -f target/stm32f4x.cfg"
elif sys.argv[1] == "st-link" or sys.argv[1] == "ST-link" or sys.argv[1] == "ST-Link" or sys.argv[1] == "ST-LINK" or sys.argv[1] == "stlink" or sys.argv[1] == "STlink" or sys.argv[1] == "STLink" or sys.argv[1] == "STLINK":
    print("Using st-link")
    startGdbCommand = "openocd -f interface/stlink-v2.cfg -f target/stm32f4x.cfg"
else:
    print("Using invalid flasher (use jlink/st-link)")
    sys.exit(1)

# Get start time
scriptStartTime = datetime.now()
subprocess_GdbServer = subprocess.Popen(startGdbCommand, shell=True)


# Start the GDB server
# if platform == "win32":
#     subprocess_GdbServer = subprocess.Popen(startGdbCommand)
# elif platform == "darwin":
#     subprocess_GdbServer = subprocess.Popen("/opt/homebrew/Cellar/open-ocd/0.12.0/bin/"+startGdbCommand)
# elif platform == "linux":
#     subprocess_GdbServer = subprocess.Popen("/usr/bin/"+startGdbCommand)
# Configure the OpenOCD server address and port
openocd_host = 'localhost'
openocd_port = 4444

# Configure the firmware binary file path and flash memory address
firmware_file = sys.argv[2]
flash_addr = '0x08000000'

# Connect to OpenOCD via telnet
tn = telnetlib.Telnet(openocd_host, openocd_port)

# Erase the flash
tn.write(b'reset halt\n')
tn.write(b'echo done "reset halt"\n')
tn.read_until(b'done "reset halt"')

print("\n=========\nErasing flash")
processStartTime = datetime.now()
tn.write(b'flash erase_sector 0 0 last\n')
tn.write(b'echo done "flash erase"\n')
tn.read_until(b'done "flash erase"')
printProcessTime(processStartTime)


# Flash the firmware
print("\n=========\nFlashing device")
processStartTime = datetime.now()
tn.write(f'flash write_image erase {firmware_file} {flash_addr}\n'.encode())
tn.write(b'echo done "flashing"\n')
tn.read_until(b'done "flashing"')
printProcessTime(processStartTime)

# Verify the flash
print("\n=========\nVerifying flash")
processStartTime = datetime.now()
tn.write(f'verify_image {firmware_file} {flash_addr}\n'.encode())
tn.write(b'echo done "verifying flash"\n')
tn.read_until(b'done "verifying flash"')
printProcessTime(processStartTime)

# Reset the target
print("\n=========\nReseting target")
processStartTime = datetime.now()
tn.write(b'reset run\n')
tn.write(b'echo done "reseting target"\n')
tn.read_until(b'done "reseting target"')
printProcessTime(processStartTime)

# Close the telnet connection
print("\n=========\nClosing GDB server")
processStartTime = datetime.now()
tn.write(b'shutdown\n')
time.sleep(1)
tn.close()

elapsedTime = (datetime.now() - scriptStartTime)
elapsedTime = elapsedTime.total_seconds()
print(f"Info : complete flashing process took: {elapsedTime} seconds")

print('Info : Flash programming completed.')
