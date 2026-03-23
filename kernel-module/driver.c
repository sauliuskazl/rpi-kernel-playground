/***************************************************************************//**
*  \file       driver.c
*
*  \details    Simple I2C driver (LCD Display Interface)
*
*  \author    sauliuskazl
*
*  \Tested with Linux 6.12.62+rpt-rpi-2712 aarch64
*
* *******************************************************************************/
#include "driver.h"
 
/*
** This function writes the data into the I2C client
**
**  Arguments:
**      buff -> buffer to be sent
**      len  -> Length of the data
**   
*/
static int I2C_Write(unsigned char *buf, unsigned int len)
{
    /*
    ** Sending Start condition, Slave address with R/W bit, 
    ** ACK/NACK and Stop condtions will be handled internally.
    */ 
    int ret = i2c_master_send(etx_i2c_client_oled, buf, len);
    
    return ret;
}
 
/*
** This function is specific to the PCF8574T with HD44780U LCD drivers.
** This function sends the command to the LCD.
**
**  Arguments:
**      cmd   -> cmd to be written
** 
*/
static void LCD_WriteCmd(char cmd)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    int ret;
    
    data_u = (cmd & 0xf0);
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C; //en=1, rs=0
    data_t[1] = data_u | 0x08; //en=0, rs=0
    data_t[2] = data_l | 0x0C; //en=1, rs=0
    data_t[3] = data_l | 0x08; //en=0, rs=0
    
    ret = I2C_Write((uint8_t *)data_t, 4);
}

/*
** This function is specific to the PCF8574T with HD44780U LCD drivers.
** This function sends the data to the LCD.
**
**  Arguments:
**      data   -> data to be written
** 
*/
static void LCD_WriteData(char data)
{
    uint8_t data_u, data_l;
    uint8_t data_t[4];
    int ret;
    
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D; //en=1, rs=1
    data_t[1] = data_u | 0x09; //en=0, rs=1
    data_t[2] = data_l | 0x0D; //en=1, rs=1
    data_t[3] = data_l | 0x09; //en=0, rs=1
    
    ret = I2C_Write((uint8_t *)data_t, 4);
}
 
/*
** This function sends the commands that need to used to Initialize the OLED.
**
**  Arguments:
**      none
** 
*/
static int LCD_DisplayInit(void)
{
	uint8_t data_t[2];
    msleep(100);               // delay
 
    /*
    ** Commands to initialize the LCD Display
    */
    data_t[0] = 0x3C; // Commands for proper restart without unpluging from power
    data_t[1] = 0x38;
    I2C_Write((uint8_t *)data_t, 2);
    msleep(4);          // delay
    I2C_Write((uint8_t *)data_t, 2);
    msleep(1);          // delay
    LCD_WriteCmd(0x32); // 8bit to 4bit mode
	LCD_WriteCmd(0x28); // set screen size
	LCD_WriteCmd(0x01); // clear screen
	msleep(1);          // delay
	LCD_WriteCmd(0x0c); // set write mode

    msleep(100);        // delay
    return 0;
}
 
/*
** This function sends text to screen.
**
**  Arguments:
**      str  -> pointer to string array
** 
*/
static void LCD_WriteString(char* str)
{
    while (*str != '\0') LCD_WriteData(*str++);
}
 
/*
** This function getting called when the slave has been found
** Note : This will be called only once when we load the driver.
*/
static int etx_oled_probe(struct i2c_client *client)
{
    LCD_DisplayInit();
    
    // show example text when device is connected
    LCD_WriteCmd(0x06);
    LCD_WriteString("labas");
 
    pr_info("LCD Probed!!!\n");
    
    return 0;
}
 
/*
** This function getting called when the slave has been removed
** Note : This will be called only once when we unload the driver.
*/
static void etx_oled_remove(struct i2c_client *client)
{   
    //fill the OLED with this data
    LCD_WriteCmd(0x01);
    
    pr_info("LCD Removed!!!\n");
}
 
/*
** Structure that has slave device id
*/
static const struct i2c_device_id etx_oled_id[] = {
        { SLAVE_DEVICE_NAME, 0 },
        { }
};
MODULE_DEVICE_TABLE(i2c, etx_oled_id);
 
/*
** I2C driver Structure that has to be added to linux
*/
static struct i2c_driver etx_oled_driver = {
        .driver = {
            .name   = SLAVE_DEVICE_NAME,
            .owner  = THIS_MODULE,
        },
        .probe          = etx_oled_probe,
        .remove         = etx_oled_remove,
        .id_table       = etx_oled_id,
};
 
/*
** I2C Board Info strucutre
*/
static struct i2c_board_info oled_i2c_board_info = {
        I2C_BOARD_INFO(SLAVE_DEVICE_NAME, LCD_SLAVE_ADDR)
    };
 
/*
** Module Init function
*/
static int __init etx_driver_init(void)
{
    int ret = -1;
    etx_i2c_adapter     = i2c_get_adapter(I2C_BUS_AVAILABLE);
    
    if( etx_i2c_adapter != NULL )
    {
        etx_i2c_client_oled = i2c_new_client_device(etx_i2c_adapter, &oled_i2c_board_info);
        
        if( etx_i2c_client_oled != NULL )
        {
            i2c_add_driver(&etx_oled_driver);
            ret = 0;
        }
        
        i2c_put_adapter(etx_i2c_adapter);
    }
    
    pr_info("Driver Added!!!\n");
    return ret;
}
 
/*
** Module Exit function
*/
static void __exit etx_driver_exit(void)
{
    i2c_unregister_device(etx_i2c_client_oled);
    i2c_del_driver(&etx_oled_driver);
    pr_info("Driver Removed!!!\n");
}
 
module_init(etx_driver_init);
module_exit(etx_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sauliuskazl");
MODULE_DESCRIPTION("Simple I2C driver (LCD Display Interface)");
MODULE_VERSION("0.1");
