SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\NVIDIA Corporation\PhysX\Common;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\TortoiseSVN\bin;C:\Program Files (x86)\Git\cmd;C:\Program Files (x86)\Git\bin;c:\bin\;C:\Program Files\MATLAB\R2013b\runtime\win64;C:\Program Files\MATLAB\R2013b\bin;c:\Program Files (x86)\Microsoft SQL Server\90\Tools\binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files\Microsoft SQL Server\110\Tools\Binn\;C:\Program Files (x86)\Nordic Semiconductor\nrf51\bin\;C:\Program Files\TortoiseGit\bin;C:\Cadence\SPB_16.6\openaccess\bin\win32\opt;C:\Cadence\SPB_16.6\tools\capture;C:\Cadence\SPB_16.6\tools\pspice;C:\Cadence\SPB_16.6\tools\specctra\bin;C:\Cadence\SPB_16.6\tools\fet\bin;C:\Cadence\SPB_16.6\tools\libutil\bin;C:\Cadence\SPB_16.6\tools\bin;C:\Cadence\SPB_16.6\tools\pcb\bin;
SET CPU_TYPE=nRF51422_xxAA
SET CPU_VENDOR=Nordic Semiconductor
SET UV2_TARGET=nrf51422_xxAA
SET CPU_CLOCK=0x00F42400
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\flashapp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\leds.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\s210_ll.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\system_error_callbacks.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\system_adc_ll.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\adcsampling.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\slld.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\slld_hal.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\bsp.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\_build\arm_startup_nrf51._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\system_nrf51.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\nrf_delay.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\spi_master.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\app_button.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\app_error.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\app_gpiote.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\app_scheduler.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\app_timer.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\_build\nrf_assert.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\_build\nrf51422_xxAA_S210.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\_build\nrf51422_xxAA_S210.axf" --i32combined --output ".\_build\nrf51422_xxAA_S210.hex"
