SET PATH=E:\Program Files (x86)\Keil\ARM\ARMCC\bin\;E:\Program Files (x86)\Python35\Scripts\;E:\Program Files (x86)\Python35\;C:\ProgramData\Oracle\Java\javapath;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\windows\system32;C:\windows;C:\windows\System32\Wbem;C:\windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files\Hewlett-Packard\SimplePass\;E:\Program Files\Java\jdk1.8.0_65\bin;E:\Program Files\Java\jdk1.8.0_65\jre\bin;E:\Program Files\MinGW\bin;E:\Program Files\MinGW\msys\1.0\bin;E:\doc\QP\qtools\bin;e:\Program Files (x86)\MATLAB\R2011b\runtime\win32;e:\Program Files (x86)\MATLAB\R2011b\bin;E:\Program Files\TortoiseGit\bin;e:\Program Files\Git\cmd;E:\doc\javadoc\comlib\maven\apache-maven-3.2.5\bin;C:\Program Files (x86)\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\Tools\Binn\;C:\Program Files\Microsoft SQL Server\100\DTS\Binn\;C:\Program Files (x86)\MySQL\MySQL Fabric 1.5.4 & MySQL Utilities 1.5.4 1.5\;C:\Program Files (x86)\MySQL\MySQL Fabric 1.5.4 & MySQL Utilities 1.5.4 1.5\Doctrine extensions for PHP\;e:\Program Files\doxygen\bin;E:\Program Files\nodejs\;E:\doc\javadoc\comlib\android-sdk-windows\tools;E:\doc\javadoc\comlib\apache-ant-1.10.1\bin;E:\doc\javadoc\comlib\android-sdk-windows\platform-tools;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;E:\Program Files\ARMLinux\bin;;e:\SysGCC\arm-eabi\bin;C:\Users\Crossing\AppData\Roaming\npm
SET CPU_TYPE=STM32F103RB
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=vlcd
SET CPU_CLOCK=0x007A1200
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmAsm" --Via "..\output\startup_stm32f10x_md._ia"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\system_stm32f10x.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\stm32_ub_led.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\stm32f10x_it.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\task.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\tsystick.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\rtcom2.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\queue.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\assert.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\except.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\adc_filter.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\cominput.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\cominputadc.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\relay.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\valve.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\rtcom1.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmCC" --Via "..\output\main.__i"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\ArmLink" --Via "..\Output\STM32-DEMO.lnp"
"E:\Program Files (x86)\Keil\ARM\ARMCC\bin\fromelf.exe" "..\Output\STM32-DEMO.axf" --i32combined --output "..\Output\STM32-DEMO.hex"
