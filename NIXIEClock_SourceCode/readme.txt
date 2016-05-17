在开发程序的时候先把此模块拷贝到对应的工程文件夹中.
改写MAIN文件,如果还有其它的C语言函数,放入USER文件夹中.

STM32位带地址转换公式：	#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))

例：#define LED1 *((__O uint32_t *)(BITBAND((GPIOA_BASE+12), 0)))
