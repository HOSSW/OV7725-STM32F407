#include "sys.h"

#ifndef _SPI_H_
#define _SPI_H_




#define SCLK        10	//PB13--->>TFT --SCL/SCK
#define MISO        11	
#define MOSI        9	//PB15 MOSI--->>TFT --SDA/DIN

#define SPI_MOSI  PEout(MOSI)
#define SPI_SCLK  PEout(SCLK)
#define SPI_MISO  PBin(MISO)

//Òº¾§¿ØÖÆ¿ÚÖÃ1²Ù×÷Óï¾äºê¶¨Òå

#define	SPI_MOSI_SET  	SPI_MOSI=1 //LCD_CTRL->BSRR=SPI_MOSI    
#define	SPI_SCLK_SET  	SPI_SCLK=1 //LCD_CTRL->BSRR=SPI_SCLK    


//Òº¾§¿ØÖÆ¿ÚÖÃ0²Ù×÷Óï¾äºê¶¨Òå

#define	SPI_MOSI_CLR  	SPI_MOSI=0 //LCD_CTRL->BRR=SPI_MOSI    
#define	SPI_SCLK_CLR  	SPI_SCLK=0 //LCD_CTRL->BRR=SPI_SCLK    

void  SPIv_WriteData(u8 Data);


#endif
