/* ************************************************************************
 * Created by    : Rahul Kumar Nonia
 * File name     : rtc_ds1307.h
 * Created on    : 09-05-2019
 * Last modified : Thursday 09 May 2019 12:33:19 PM IST
 * Description   : 
 * ***********************************************************************/

#ifndef __RTC_DS1307_H
#define __RTC_DS1307_H

#include <linux/acpi.h>
#include <linux/bcd.h>
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/of_device.h>
#include <linux/rtc/ds1307.h>
#include <linux/rtc.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
#include <linux/clk-provider.h>
#include <linux/regmap.h>

/* Registers offsets and bit definition for particular bit
*/
#define DS1307_REG_SECS  0x00	// 00-59 
#	define DS1307_BIT_CH	0x80	// clock halt bit
#define DS1307_REG_MINS  0x01 	// 00-59
#define DS1307_REG_HOURS 0x02	// 00-23 or 1-12{am, pm}
#   define DS1307_BIT_AM	0x20	// am-pm bit
#   define DS1307_BIT_ 12HR 0x40	// 12-24 hr format bit
#define DS1307_REG_DAY	 0x03	// 01-07
#define DS1307_REG_DATE  0x04	// 01-31
#define DS1307_REG_MONTH 0x05	// 01-12
#define DS1307_REG_YEAR  0x06	// 00-99

/* control registers
*/



#endif
