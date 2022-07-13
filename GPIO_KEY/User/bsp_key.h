#define KEY1_PIN GPIO_Pin_0
#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_CLK RCC_AHB1Periph_GPIOA

#define KEY2_PIN GPIO_Pin_13
#define KEY2_GPIO_PORT GPIOC
#define KEY2_GPIO_CLK RCC_AHB1Periph_GPIOC

void Key_GPIO_Config(void);
