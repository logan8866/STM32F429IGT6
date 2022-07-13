#define LED1_PIN GPIO_Pin_10
 #define LED1_GPIO_PORT GPIOH
 #define LED1_GPIO_CLK RCC_AHB1Periph_GPIOH

 #define LED2_PIN GPIO_Pin_11
 #define LED2_GPIO_PORT GPIOH
 #define LED2_GPIO_CLK RCC_AHB1Periph_GPIOH
 
 #define LED3_PIN GPIO_Pin_12
 #define LED3_GPIO_PORT GPIOH
 #define LED3_GPIO_CLK RCC_AHB1Periph_GPIOH
 
 #define LED4_PIN GPIO_Pin_11
 #define LED4_GPIO_PORT GPIOD
 #define LED4_GPIO_CLK RCC_AHB1Periph_GPIOD
 
 #define digitalHi(p,i) {p->BSRRL=i;} 
#define digitalLo(p,i) {p->BSRRH=i;} 
#define digitalToggle(p,i) {p->ODR ^=i;} 

 #define LED1_TOGGLE digitalToggle(LED1_GPIO_PORT,LED1_PIN)
 #define LED1_OFF digitalHi(LED1_GPIO_PORT,LED1_PIN)
 #define LED1_ON digitalLo(LED1_GPIO_PORT,LED1_PIN)
 
 #define LED2_TOGGLE digitalToggle(LED2_GPIO_PORT,LED2_PIN)
 #define LED2_OFF digitalHi(LED2_GPIO_PORT,LED2_PIN)
 #define LED2_ON digitalLo(LED2_GPIO_PORT,LED2_PIN)
 
 #define LED3_TOGGLE digitalToggle(LED3_GPIO_PORT,LED3_PIN)
 #define LED3_OFF digitalHi(LED3_GPIO_PORT,LED3_PIN)
 #define LED3_ON digitalLo(LED3_GPIO_PORT,LED3_PIN)
 
 #define LED4_TOGGLE digitalToggle(LED4_GPIO_PORT,LED4_PIN)
 #define LED4_OFF digitalHi(LED4_GPIO_PORT,LED4_PIN)
 #define LED4_ON digitalLo(LED4_GPIO_PORT,LED4_PIN)
 
 
 
 //?
 #define LED_RED \
 LED1_ON;\
 LED2_OFF;\
 LED3_OFF
 
 //?
 #define LED_GREEN \
 LED1_OFF;\
 LED2_ON;\
 LED3_OFF
 
 //?
 #define LED_BLUE \
 LED1_OFF;\
 LED2_OFF;\
 LED3_ON
 
 
 //?(?+?)
 #define LED_YELLOW \
 LED1_ON;\
 LED2_ON;\
 LED3_OFF
 
 void LED_GPIO_Config(void);
 