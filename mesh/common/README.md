Common module for building, debugging and flashing the safetycore and dashcore. Moreover it will include generic modules for the serial protocol and uart under libs.

## Building

This module builds the project using "west build" of the zephyr project, for this the zephyr project needs to be cloned, this is done in common.mk with the given ZEPHYR_VERSION. Cloning the repo to /root/zephyr/ZEPPHYR_VERSION, this way the project only needs to be cloned once and the version is still configurable per project.

## Flashing

The stm32 micro controller can be flashed using the same method as the Dual Inventive products. Namely the running a openocd session locally and connecting to the gdb port of the session from within the container.

## Debugging

In order to debug the OnTheMove applications an openOCD server must be started. The folder `openocd-templates` contains the templates for the F4 (DashCore) and F1 (SafetyCore). You can start an openOCD server by giving the correct file:

E.g. for the DashCore:
```
openocd -f openocd-f4.cfg
```

*Note:* Change `adapter serial` to your correct serial number or remove the line (but it is then not possible to program both at once).

When the openOCD server is started. Run `make gdb` Or use the `launch.json` to launch GDB in VS-code.

### VS-code limitations
VS code contains some known bugs. One bug is that it doesn't fully implement GDB commands. So doesn't show VS-code the thread names. Thread names are supported in the GDB command line:
```
Start address 0x08001fac, load size 47557
Transfer rate: 13 KB/sec, 2972 bytes/write.
Unable to match requested speed 2000 kHz, using 1800 kHz
Unable to match requested speed 2000 kHz, using 1800 kHz
[stm32f4x.cpu] halted due to debug-request, current mode: Thread
xPSR: 0x01000000 pc: 0x08001fac msp: 0x20000dc0, semihosting
Unable to match requested speed 8000 kHz, using 4000 kHz
Unable to match requested speed 8000 kHz, using 4000 kHz
(gdb) c
Continuing.
^C
Thread 2 "idle" received signal SIGINT, Interrupt.
[Switching to Thread 536871648]
arch_cpu_idle () at /root/zephyr/v3.3.0/zephyr/arch/arm/core/aarch32/cpu_idle.S:105
105             cpsie   i
(gdb) info threads
  Id   Target Id                                                             Frame
  1    Thread 536871280 "Uart thread" (Name: Uart thread, prio:7,useropts:0) arch_irq_unlock (key=0) at /root/zephyr/v3.3.0/zephyr/include/zephyr/arch/arm/aarch32/asm_inline_gcc.h:110
* 2    Thread 536871648 "idle" (Name: idle, prio:15,useropts:1)              arch_cpu_idle () at /root/zephyr/v3.3.0/zephyr/arch/arm/core/aarch32/cpu_idle.S:105
(gdb)
```

