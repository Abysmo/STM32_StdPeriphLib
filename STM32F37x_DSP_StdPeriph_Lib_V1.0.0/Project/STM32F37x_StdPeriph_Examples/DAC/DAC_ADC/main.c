/**
  ******************************************************************************
  * @file    DAC/DAC_ADC/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f37x.h"

/** @addtogroup STM32F37x_StdPeriph_Examples
  * @{
  */

/** @addtogroup DAC_ADC
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void ADC_Config(void);
static void DAC_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f37x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f37x.c file
     */ 

  /* DAC configuration ---------------------------------------------*/
  DAC_Config();

  /* ADC configuration ---------------------------------------------*/
  ADC_Config();

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  Configures the DAC1 channel 1 with output buffer disabled.
  * @param  None
  * @retval None
  */
void DAC_Config(void)
{
  DAC_InitTypeDef    DAC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;

  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configure PA.04 (DAC1_OUT1) in analog mode -------------------------*/
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable DAC clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC1, ENABLE);
  
  /* DAC1 channel1 Configuration */
  DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
  DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits2_0;
  DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  DAC_Init(DAC1, DAC_Channel_1, &DAC_InitStructure);
  
  /* Enable DAC1 Channel1 */
  DAC_Cmd(DAC1, DAC_Channel_1, ENABLE);
}

/**
  * @brief  ADC1 channel5 configuration
  * @param  None
  * @retval None
  */
void ADC_Config(void)
{  
  ADC_InitTypeDef    ADC_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  /* GPIOA Peripheral clock is enabled within the above DAC_Config() function,
     otherwise the following function should be called 
     "RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);" */
  
  
  /* ADC1 Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* Configure ADC Channel5 (PA5) as analog input */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* ADC1 DeInit */
  ADC_DeInit(ADC1);
  ADC_StructInit(&ADC_InitStructure);
  
  /* Configure the ADC1 in continuous mode */
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 Regular Channel5 Config */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_28Cycles5);

  /* Enable End Of Conversion interrupt */
  ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
  
  /* Configure and enable ADC1 interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = ADC1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);     

  /* ADC1 Start Conversion */ 
  ADC_SoftwareStartConv(ADC1);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
