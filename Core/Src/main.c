/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal.h"
#include "stdbool.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim5;

UART_HandleTypeDef huart6;

/* USER CODE BEGIN PV */
int i=500;
uint8_t a;
char rx_buffer[50],tx_buffer[50];
 bool goingforward,buttonone,buttontwo,buttonthree,clockwise=true;
 bool led_state=false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART6_UART_Init(void);
static void MX_TIM5_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int min(int a,int b);
int max(int x,int y);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_USART6_UART_Init();
  MX_TIM5_Init();
  /* USER CODE BEGIN 2 */

  HAL_GPIO_WritePin(GPIOA,A1_Pin,GPIO_PIN_SET);   // Start first motor clock wise rotation
  HAL_GPIO_WritePin(GPIOA,A2_Pin,GPIO_PIN_RESET);
  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,100);
  //HAL_GPIO_WritePin(GPIOA,A3_Pin,GPIO_PIN_SET);   //Start second motor clock wise rotation
  //HAL_GPIO_WritePin(GPIOA,A4_Pin,GPIO_PIN_RESET);
  HAL_TIM_Base_Start(&htim2);               //Initialize stm32 timer 3
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim5,TIM_CHANNEL_1);
  //HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2	);  //PB1 Start pwm first motor  100% duty cycle//PB0 Start pwm second motor 100% duty cycle
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //Change direction of motors anti clock wise
HAL_UART_Receive(&huart6, (uint8_t*)rx_buffer, 50, 500);
if(rx_buffer[0] == 'F')
{

if(!goingforward)
{
	goingforward=true;
	HAL_GPIO_TogglePin(GPIOA,A1_Pin);
	HAL_Delay(100); //Delay for 3 seconds to stop motor properly
	HAL_GPIO_TogglePin(GPIOA,A2_Pin);
}

HAL_UART_Transmit(&huart6,(uint8_t*)tx_buffer,sprintf(tx_buffer,"Led is on\n"),500);

}
else if(rx_buffer[0] == 'B')
	  {
	if(goingforward)
	{
	HAL_GPIO_TogglePin(GPIOA,A1_Pin);
	HAL_Delay(100); //Delay for 3 seconds to stop motor properly
	HAL_GPIO_TogglePin(GPIOA,A2_Pin);

	goingforward=false;
	}
	  HAL_UART_Transmit(&huart6,(uint8_t*)tx_buffer,sprintf(tx_buffer,"Led is off\n"),500);

}

else if(rx_buffer[0] == 'L')
{
	 //for (i=500;i<=2500;i++)
	//i = min(i+100,2500);

	i = min(i+200,2500);
	 //i+=200;
	//if(i>=2500)
		//  i=2500;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,i);
		  HAL_Delay(.1);

	 // }
	  if(led_state!=true)
	  HAL_UART_Transmit(&huart6,(uint8_t*)tx_buffer,sprintf(tx_buffer,"Led is on\n"),500);
	  led_state = true;
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }
}
else if(rx_buffer[0]=='R') {
	// for (i=2500;i>=500;i--)
	 // {
	  	 //i-=200;
		// i+=500;
		//if(i<=500)
			 // i=500;
	i=max(i-200,500);
		  __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,i);
		  HAL_Delay(.01);
		  //testing
	 // }
	  if(led_state!=false)
	  HAL_UART_Transmit(&huart6,(uint8_t*)tx_buffer,sprintf(tx_buffer," Led is off\n"),500);
	  led_state = false;
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

}
else if(rx_buffer[0]=='1') //Motor rotate at 75% duty cycle
{

	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,0);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

	//buttonone=false;
	//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,75); *///Second motor 75% voltage//Second motor 75% voltage

}
else if(rx_buffer[0]=='2')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,20);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }


}
else if(rx_buffer[0]=='3')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,30);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }


}
else if(rx_buffer[0]=='4')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,40);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }


}
else if(rx_buffer[0]=='5')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,50);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

}
else if(rx_buffer[0]=='6')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,60);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

}
else if(rx_buffer[0]=='7')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,70);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

}
else if(rx_buffer[0]=='8')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,80);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

}
else if(rx_buffer[0]=='9')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,90);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

}
else if(rx_buffer[0]=='q')
	  {
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	HAL_Delay(500);
	  for(a=0;a<50;a++)
	  {
		  rx_buffer[a]=0;
	  }

	  }

//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,100);
/*
if(clockwise){
	//HAL_Delay(3000); //Delay for 3 seconds to stop motor properly
	//HAL_GPIO_TogglePin(GPIOA,A1_Pin);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0);
	//clockwise = false;
}
else
{
	//HAL_Delay(3000); //Delay for 3 seconds to stop motor properly
	//HAL_GPIO_TogglePin(GPIOA,A2_Pin);
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,100);
	//clockwise = true;
}
*/

if(HAL_GPIO_ReadPin(B8_GPIO_Port,B8_Pin)&&buttonone) //Motor rotate at 75% duty cycle
{
	/*__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,100);
	HAL_Delay(1000);*/
	__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
	buttonone=false;
			//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,75); //Second motor 75% voltage//Second motor 75% voltage

}
else if(HAL_GPIO_ReadPin(B8_GPIO_Port, B8_Pin)==GPIO_PIN_RESET)
	  {
buttonone=true;


}
if(HAL_GPIO_ReadPin(B7_GPIO_Port,B7_Pin)&&buttontwo) //Motor rotate at 50% duty cycle
		{
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,0);
			buttontwo=false;//Second motor 50% voltage
			//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,50); //Second motor 50% voltage
		}
else if(HAL_GPIO_ReadPin(B7_GPIO_Port, B7_Pin)==GPIO_PIN_RESET)
	  {
buttontwo=true;


}

if(HAL_GPIO_ReadPin(B6_GPIO_Port,B6_Pin)&&buttonthree) //Motor rotate at 25% duty cycle
		{
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,100);
			HAL_Delay(1000);
			__HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,25);
			buttonthree=false;//Second motor 25% voltage
			//__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,25); //Second motor 25% voltage
		}
else if(HAL_GPIO_ReadPin(B6_GPIO_Port, B6_Pin)==GPIO_PIN_RESET)
	  {
buttonthree=true;


}
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 8399;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 99;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */

  /* USER CODE END TIM5_Init 2 */
  HAL_TIM_MspPostInit(&htim5);

}

/**
  * @brief USART6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART6_UART_Init(void)
{

  /* USER CODE BEGIN USART6_Init 0 */

  /* USER CODE END USART6_Init 0 */

  /* USER CODE BEGIN USART6_Init 1 */

  /* USER CODE END USART6_Init 1 */
  huart6.Instance = USART6;
  huart6.Init.BaudRate = 9600;
  huart6.Init.WordLength = UART_WORDLENGTH_8B;
  huart6.Init.StopBits = UART_STOPBITS_1;
  huart6.Init.Parity = UART_PARITY_NONE;
  huart6.Init.Mode = UART_MODE_TX_RX;
  huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart6.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart6) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART6_Init 2 */

  /* USER CODE END USART6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, A1_Pin|A2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : A1_Pin A2_Pin */
  GPIO_InitStruct.Pin = A1_Pin|A2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : B6_Pin B7_Pin B8_Pin */
  GPIO_InitStruct.Pin = B6_Pin|B7_Pin|B8_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
int min(int a,int b)
{
	return (a>b)?b:a;
}
int max(int x,int y)
{
	return (x<y)?y:x;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
