Did not try yet to make buit-in module, only build and tested using command ran on Raspberry pi cm5:
1. 'sudo make'
2. 'sudo insmod driver.ko'
3. 'sudo rmmod driver'

***

* Only I2C was somewhat succsesfully tested (logic analyser shows correct data sent to the LCD display)
* When trying to run examples using kobject/sysfs at build can't get past this error:
"
In file included from <command-line>:
/usr/src/linux-headers-6.12.75+rpt-common-rpi/include/linux/kconfig.h:5:10: fatal error: generated/autoconf.h: No such file or directory
    5 | #include <generated/autoconf.h>
      |          ^~~~~~~~~~~~~~~~~~~~~~
compilation terminated.
"

