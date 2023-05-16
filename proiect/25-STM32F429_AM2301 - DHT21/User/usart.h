/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include <stdio.h>



//#if defined (USE_STM324xG_EVAL)
//  #include "stm324xg_eval.h"

//#elif defined (USE_STM324x7I_EVAL) 
//  #include "stm324x7i_eval.h"

//#elif defined (USE_STM324x9I_EVAL) 
//  #include "stm324x9i_eval.h"

//#else
// #error "Please select first the Evaluation board used in your application (in Project Options)"
//#endif

 #include "stm32f4xx_gpio.h"
void USART_Config(void);


/* Exported typedef ----------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
