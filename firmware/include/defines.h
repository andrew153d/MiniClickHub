#ifndef DEFINES_H
#define DEFINES_H

//Pin defenitions
#define SDA_PIN 42
#define SCL_PIN 41


#define FIRST_MINICLICK_ADDRESS 0x12

//MiniClick masks
#define BUTTON_PRESSED 0x80
#define BUTTON_RELEASED 0x40


//Addresses
#define miniclick_device_queue_size_address 0x50
#define miniclick_pop_queue_address 0x51
#define who_am_i_address 0x57


#endif