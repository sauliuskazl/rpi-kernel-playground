/***************************************************************************//**
*  \file       driver.h
*
*  \details    Simple I2C driver (LCD Display Interface)
*
*  \author    sauliuskazl
*
*  \Tested with Linux 6.12.62+rpt-rpi-2712 aarch64
*
* *******************************************************************************/
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
 
#define I2C_BUS_AVAILABLE   (          1 )              // I2C Bus available in our Raspberry Pi
#define SLAVE_DEVICE_NAME   ( "ETX_OLED" )              // Device and Driver Name
#define LCD_SLAVE_ADDR  (       0x27 )              // LCD Slave Address
 
static struct i2c_adapter *etx_i2c_adapter     = NULL;  // I2C Adapter Structure
static struct i2c_client  *etx_i2c_client_oled = NULL;  // I2C Cient Structure (In our case it is OLED)
