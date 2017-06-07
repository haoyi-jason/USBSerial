/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Setup for the STMicroelectronics STM3210E-EVAL evaluation board.
 */

/*
 * Board identifier.
 */
#define BOARD_ST_STM3210E_EVAL
#define BOARD_NAME              "USB_SERIAL"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            32768
#define STM32_HSECLK            8000000

/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 * Note: Older board revisions should define STM32F10X_HD instead, please
 *       verify the STM32 model mounted on your board. The change also
 *       affects your linker script.
 */
#define STM32F207xx
#define STM32F2XX

#define STM32_VDD 330U

/*
  IO Pin Assignnment
*/

#define GPIOA_0         	 0U
#define GPIOA_LOG_LED_O 	 1U
#define GPIOA_PWR_LBT_I 	 2U
#define GPIOA_3    	    	 3U    
#define GPIOA_4         	 4U
#define GPIOA_5         	 5U
#define GPIOA_6         	 6U
#define GPIOA_POR_CHG_I 	 7U
#define GPIOA_LOG_STA_I 	 8U
#define GPIOA_UART1_TX  	 9U
#define GPIOA_USB_PLUG  	10U
#define GPIOA_USB_DM 	11U
#define GPIOA_USB_DP 	12U
#define GPIOA_SWDIO     	13U
#define GPIOA_SWCLK     	14U
#define GPIOA_USB_EN  	15U

#define GPIOB_0      		0U
#define GPIOB_1      		1U
#define GPIOB_2       		2U
#define GPIOB_BT_STA_O      3U
#define GPIOB_4     		4U
#define GPIOB_BT_MOD_I     	5U
#define GPIOB_I2C1_SCL      6U
#define GPIOB_I2C1_SDA      7U
#define GPIOB_8      		8U
#define GPIOB_9             9U
#define GPIOB_10      		10U
#define GPIOB_11      		11U
#define GPIOB_ACC_CS_O      12U
#define GPIOB_ACC_SCK_O     13U
#define GPIOB_ACC_SDO_O     14U
#define GPIOB_ACC_SDI_I     15U
                            
#define GPIOC_CLC_POL_I 	0U
#define GPIOC_CLC_CLR_O     1U
#define GPIOC_CLC_INT_I     2U
#define GPIOC_CLC_SHDN_O    3U
#define GPIOC_BAT_CHG_I     4U
#define GPIOC_SD_CD     	5U
#define GPIOC_ACC_INT_I     6U
#define GPIOC_ACC_FSYNC_I   7U
#define GPIOC_SD_D0         8U
#define GPIOC_SD_D1         9U
#define GPIOC_SD_D2      	10U
#define GPIOC_SD_D3     	11U
#define GPIOC_SD_CLK     	12U
#define GPIOC_13         	13U
#define GPIOC_OSC32_IN      14U
#define GPIOC_OSC32_OUT     15U

#define GPIOD_SDIO_CMD  2U

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))


/*
 * GPIOA setup:
 *
 * PA0  - GPIOA_0                  (AF.11)
 * PA1  - GPIOA_LOG_LED_O          (AF.11)
 * PA2  - GPIOA_PWR_LBT_I          (AF.11)
 * PA3  - GPIOA_3    	             (AF.11)
 * PA4  - GPIOA_4                  (OUT.PP)
 * PA5  - GPIOA_5                  (IN.PU)
 * PA6  - GPIOA_6                  (IN.PU)
 * PA7  - GPIOA_POR_CHG_I          (AF.11)
 * PA8  - GPIOA_LOG_STA_I          (OUT.PP)
 * PA9  - GPIOA_UART1_TX           (AF.7)
 * PA10 - GPIOA_UART1_RX           (AF.7)
 * PA11 - GPIOA_UART1_RTS          (AF.9)
 * PA12 - GPIOA_UART1_CTS          (AF.9)
 * PA13 - GPIOA_SWDIO              (AF.0)
 * PA14 - GPIOA_SWCLK              (AF.0)
 * PA15 - GPIOA_BT_RST_O           (IN.FLT)
 */

#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_0        )		| \
                                     PIN_MODE_OUTPUT(GPIOA_LOG_LED_O)       | \
                                     PIN_MODE_INPUT(GPIOA_PWR_LBT_I)        | \
                                     PIN_MODE_INPUT(GPIOA_3    	   )        | \
                                     PIN_MODE_INPUT(GPIOA_4        )        | \
                                     PIN_MODE_INPUT(GPIOA_5        )        | \
                                     PIN_MODE_INPUT(GPIOA_6        )        | \
                                     PIN_MODE_INPUT(GPIOA_POR_CHG_I)        | \
                                     PIN_MODE_INPUT(GPIOA_LOG_STA_I)        | \
                                     PIN_MODE_ALTERNATE(GPIOA_UART1_TX )    | \
                                     PIN_MODE_INPUT(GPIOA_USB_PLUG )    | \
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DM)    | \
                                     PIN_MODE_ALTERNATE(GPIOA_USB_DP)    | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO    )        | \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK    )        | \
                                     PIN_MODE_OUTPUT(GPIOA_USB_EN ))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_0        ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LOG_LED_O) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PWR_LBT_I) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_3    	   ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_4        ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_5        ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_6        ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_POR_CHG_I) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LOG_STA_I) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_UART1_TX ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_PLUG ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DM) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_DP) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO    ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK    ) | \
                                     PIN_OTYPE_PUSHPULL(GPIOA_USB_EN ))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_HIGH(GPIOA_0           ) |\
                                     PIN_OSPEED_HIGH(GPIOA_LOG_LED_O   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_PWR_LBT_I   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_3    	   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_4           ) |\
                                     PIN_OSPEED_HIGH(GPIOA_5           ) |\
                                     PIN_OSPEED_HIGH(GPIOA_6           ) |\
                                     PIN_OSPEED_HIGH(GPIOA_POR_CHG_I   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_LOG_STA_I   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_UART1_TX    ) |\
                                     PIN_OSPEED_HIGH(GPIOA_USB_PLUG    ) |\
                                     PIN_OSPEED_HIGH(GPIOA_USB_DM   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_USB_DP   ) |\
                                     PIN_OSPEED_HIGH(GPIOA_SWDIO       ) |\
                                     PIN_OSPEED_HIGH(GPIOA_SWCLK       ) |\
                                     PIN_OSPEED_HIGH(GPIOA_USB_EN    ))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_0        ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_LOG_LED_O) |\
                                     PIN_PUPDR_FLOATING(GPIOA_PWR_LBT_I) |\
                                     PIN_PUPDR_FLOATING(GPIOA_3    	   ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_4        ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_5        ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_6        ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_POR_CHG_I) |\
                                     PIN_PUPDR_FLOATING(GPIOA_LOG_STA_I) |\
                                     PIN_PUPDR_FLOATING(GPIOA_UART1_TX ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_PLUG ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DM) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_DP) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO    ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK    ) |\
                                     PIN_PUPDR_FLOATING(GPIOA_USB_EN ))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_0        ) |\
                                     PIN_ODR_HIGH(GPIOA_LOG_LED_O) |\
                                     PIN_ODR_HIGH(GPIOA_PWR_LBT_I) |\
                                     PIN_ODR_HIGH(GPIOA_3    	 ) |\
                                     PIN_ODR_HIGH(GPIOA_4        ) |\
                                     PIN_ODR_HIGH(GPIOA_5        ) |\
                                     PIN_ODR_HIGH(GPIOA_6        ) |\
                                     PIN_ODR_HIGH(GPIOA_POR_CHG_I) |\
                                     PIN_ODR_HIGH(GPIOA_LOG_STA_I) |\
                                     PIN_ODR_HIGH(GPIOA_UART1_TX ) |\
                                     PIN_ODR_HIGH(GPIOA_USB_PLUG ) |\
                                     PIN_ODR_HIGH(GPIOA_USB_DM) |\
                                     PIN_ODR_HIGH(GPIOA_USB_DP) |\
                                     PIN_ODR_HIGH(GPIOA_SWDIO    ) |\
                                     PIN_ODR_HIGH(GPIOA_SWCLK    ) |\
                                     PIN_ODR_HIGH(GPIOA_USB_EN ))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_0        ,0) |\
                                     PIN_AFIO_AF(GPIOA_LOG_LED_O,0) |\
                                     PIN_AFIO_AF(GPIOA_PWR_LBT_I,0) |\
                                     PIN_AFIO_AF(GPIOA_3    	,0) |\
                                     PIN_AFIO_AF(GPIOA_4        ,0) |\
                                     PIN_AFIO_AF(GPIOA_5        ,0) |\
                                     PIN_AFIO_AF(GPIOA_6        ,0) |\
                                     PIN_AFIO_AF(GPIOA_POR_CHG_I,0))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_LOG_STA_I,0) |\
                                     PIN_AFIO_AF(GPIOA_UART1_TX ,7) |\
                                     PIN_AFIO_AF(GPIOA_USB_PLUG ,7) |\
                                     PIN_AFIO_AF(GPIOA_USB_DM,10) |\
                                     PIN_AFIO_AF(GPIOA_USB_DP,10) |\
                                     PIN_AFIO_AF(GPIOA_SWDIO    ,0) |\
                                     PIN_AFIO_AF(GPIOA_SWCLK    ,0) |\
                                     PIN_AFIO_AF(GPIOA_USB_PLUG ,0))
/*
 * GPIOB setup:
 *
 * PB0  - GPIOB_0      	       (AF.11)
 * PB1  - GPIOB_1      	       (AF.11)
 * PB2  - GPIOB_2       	       (OUT.PP)
 * PB3  - GPIOB_BT_STA_O        (AF.5)
 * PB4  - GPIOB_4     	       (AF.5)
 * PB5  - GPIOB_BT_MOD_I        (AF.5)
 * PB6  - GPIOB_I2C1_SCL        (AF.4)
 * PB7  - GPIOB_I2C1_SDA        (AF.4)
 * PB8  - GPIOB_8      	       (AF.11)
 * PB9  - GPIOB_9               (IN.FLT)
 * PB10 - GPIOB_10      	       (AF.11)
 * PB11 - GPIOB_11      	       (AF.11)
 * PB12 - GPIOB_ACC_CS_O        (AF.11)
 * PB13 - GPIOB_ACC_SCK_O       (AF.11)
 * PB14 - GPIOB_ACC_SDO_O       (OUT.PP)
 * PB15 - GPIOB_ACC_SDI_I       (OUT.PP)
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_0        ) |\
                                     PIN_MODE_INPUT(GPIOB_1        ) |\
                                     PIN_MODE_INPUT(GPIOB_2        ) |\
                                     PIN_MODE_INPUT(GPIOB_BT_STA_O ) |\
                                     PIN_MODE_INPUT(GPIOB_4        ) |\
                                     PIN_MODE_INPUT(GPIOB_BT_MOD_I ) |\
                                     PIN_MODE_INPUT(GPIOB_I2C1_SCL ) |\
                                     PIN_MODE_INPUT(GPIOB_I2C1_SDA ) |\
                                     PIN_MODE_INPUT(GPIOB_8        ) |\
                                     PIN_MODE_INPUT(GPIOB_9        ) |\
                                     PIN_MODE_INPUT(GPIOB_10       ) |\
                                     PIN_MODE_INPUT(GPIOB_11       ) |\
                                     PIN_MODE_INPUT(GPIOB_ACC_CS_O ) |\
                                     PIN_MODE_INPUT(GPIOB_ACC_SCK_O) |\
                                     PIN_MODE_INPUT(GPIOB_ACC_SDO_O) |\
                                     PIN_MODE_INPUT(GPIOB_ACC_SDI_I))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_0      	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_1      	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_2       	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BT_STA_O  ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_4     	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BT_MOD_I  ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SCL  ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_I2C1_SDA  ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_8      	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_9         ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_10      	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_11      	) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_ACC_CS_O  ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_ACC_SCK_O ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_ACC_SDO_O ) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_ACC_SDI_I ))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_HIGH(GPIOB_0      	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_1      	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_2       	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_BT_STA_O )          |\
                                     PIN_OSPEED_HIGH(GPIOB_4     	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_BT_MOD_I )          |\
                                     PIN_OSPEED_HIGH(GPIOB_I2C1_SCL )          |\
                                     PIN_OSPEED_HIGH(GPIOB_I2C1_SDA )          |\
                                     PIN_OSPEED_HIGH(GPIOB_8      	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_9        )          |\
                                     PIN_OSPEED_HIGH(GPIOB_10      	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_11      	)          |\
                                     PIN_OSPEED_HIGH(GPIOB_ACC_CS_O )          |\
                                     PIN_OSPEED_HIGH(GPIOB_ACC_SCK_O)          |\
                                     PIN_OSPEED_HIGH(GPIOB_ACC_SDO_O)          |\
                                     PIN_OSPEED_HIGH(GPIOB_ACC_SDI_I))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_FLOATING(GPIOB_0      	)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_1      	)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_2       	)       |\
                                     PIN_PUPDR_PULLDOWN(GPIOB_BT_STA_O )       |\
                                     PIN_PUPDR_PULLUP(GPIOB_4     	)       |\
                                     PIN_PUPDR_PULLDOWN(GPIOB_BT_MOD_I )       |\
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SCL )       |\
                                     PIN_PUPDR_FLOATING(GPIOB_I2C1_SDA )       |\
                                     PIN_PUPDR_FLOATING(GPIOB_8      	)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_9        )       |\
                                     PIN_PUPDR_FLOATING(GPIOB_10      	)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_11      	)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_ACC_CS_O )       |\
                                     PIN_PUPDR_FLOATING(GPIOB_ACC_SCK_O)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_ACC_SDO_O)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_ACC_SDI_I))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_0      	)             |\
                                     PIN_ODR_HIGH(GPIOB_1      	)             |\
                                     PIN_ODR_HIGH(GPIOB_2       	)             |\
                                     PIN_ODR_HIGH(GPIOB_BT_STA_O )             |\
                                     PIN_ODR_HIGH(GPIOB_4     	)             |\
                                     PIN_ODR_HIGH(GPIOB_BT_MOD_I )             |\
                                     PIN_ODR_HIGH(GPIOB_I2C1_SCL )             |\
                                     PIN_ODR_HIGH(GPIOB_I2C1_SDA )             |\
                                     PIN_ODR_HIGH(GPIOB_8      	)             |\
                                     PIN_ODR_HIGH(GPIOB_9        )             |\
                                     PIN_ODR_HIGH(GPIOB_10      	)             |\
                                     PIN_ODR_HIGH(GPIOB_11      	)             |\
                                     PIN_ODR_HIGH(GPIOB_ACC_CS_O )             |\
                                     PIN_ODR_HIGH(GPIOB_ACC_SCK_O)             |\
                                     PIN_ODR_HIGH(GPIOB_ACC_SDO_O)             |\
                                     PIN_ODR_HIGH(GPIOB_ACC_SDI_I))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_0      	,0 )           |\
                                     PIN_AFIO_AF(GPIOB_1      	,0 )           |\
                                     PIN_AFIO_AF(GPIOB_2       ,0)            |\
                                     PIN_AFIO_AF(GPIOB_BT_STA_O,0)            |\
                                     PIN_AFIO_AF(GPIOB_4     	,0)            |\
                                     PIN_AFIO_AF(GPIOB_BT_MOD_I,0)            |\
                                     PIN_AFIO_AF(GPIOB_I2C1_SCL,4)            |\
                                     PIN_AFIO_AF(GPIOB_I2C1_SDA,4))           
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_8      	,0  )           |\
                                     PIN_AFIO_AF(GPIOB_9        ,0)            |\
                                     PIN_AFIO_AF(GPIOB_10       ,0)           |\
                                     PIN_AFIO_AF(GPIOB_11       ,0)           |\
                                     PIN_AFIO_AF(GPIOB_ACC_CS_O ,0 )           |\
                                     PIN_AFIO_AF(GPIOB_ACC_SCK_O,5 )           |\
                                     PIN_AFIO_AF(GPIOB_ACC_SDO_O,5)            |\
                                     PIN_AFIO_AF(GPIOB_ACC_SDI_I,5))

/*
 * GPIOC setup:
 *
 * PC0  - GPIOC_CLC_POL_I             (IN.PU)
 * PC1  - GPIOC_CLC_CLR_O              (AF.11)
 * PC2  - GPIOC_CLC_INT_I              (AF.11)
 * PC3  - GPIOC_CLC_SHDN_O             (AF.11)
 * PC4  - GPIOC_BAT_CHG_I              (AF.11)
 * PC5  - GPIOC_SD_CD                 (AF.11)
 * PC6  - GPIOC_ACC_INT_I              (IN.PU)
 * PC7  - GPIOC_ACC_FSYNC_I            (IN.PU)
 * PC8  - GPIOC_SD_D0                  (IN.PU)
 * PC9  - GPIOC_SD_D1                  (OUT.PP)
 * PC10 - GPIOC_SD_D2                  (AF.5)
 * PC11 - GPIOC_SD_D3                 (AF.5)
 * PC12 - GPIOC_SD_CLK                 (AF.5)
 * PC13 - GPIOC_13                     (IN.PU)
 * PC14 - GPIOC_OSC32_IN               (IN)
 * PC15 - GPIOC_OSC32_OUT              (IN)
 */
 
#define VAL_GPIOC_MODER             (PIN_MODE_ALTERNATE(GPIOC_CLC_POL_I )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_CLC_CLR_O  )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_CLC_INT_I  )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_CLC_SHDN_O )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_BAT_CHG_I  )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_CD     )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_ACC_INT_I  )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_ACC_FSYNC_I)        |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D0      )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D1      )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D2      )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_D3     )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_CLK     )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_13         )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_OSC32_IN   )        |\
                                     PIN_MODE_ALTERNATE(GPIOC_OSC32_OUT  ))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_CLC_POL_I )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CLC_CLR_O  )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CLC_INT_I  )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_CLC_SHDN_O )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_BAT_CHG_I  )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CD     )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_ACC_INT_I  )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_ACC_FSYNC_I)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D0      )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D1      )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D2      )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_D3     )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CLK     )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_13         )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_IN   )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_OSC32_OUT  ))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_CLC_POL_I )           |\
                                     PIN_OSPEED_HIGH(GPIOC_CLC_CLR_O  )           |\
                                     PIN_OSPEED_HIGH(GPIOC_CLC_INT_I  )           |\
                                     PIN_OSPEED_HIGH(GPIOC_CLC_SHDN_O )           |\
                                     PIN_OSPEED_HIGH(GPIOC_BAT_CHG_I  )           |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_CD     )           |\
                                     PIN_OSPEED_HIGH(GPIOC_ACC_INT_I  )           |\
                                     PIN_OSPEED_HIGH(GPIOC_ACC_FSYNC_I)           |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D0      )           |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D1      )           |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D2      )           |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_D3     )           |\
                                     PIN_OSPEED_HIGH(GPIOC_SD_CLK     )           |\
                                     PIN_OSPEED_HIGH(GPIOC_13         )           |\
                                     PIN_OSPEED_HIGH(GPIOC_OSC32_IN   )           |\
                                     PIN_OSPEED_HIGH(GPIOC_OSC32_OUT  ))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_CLC_POL_I )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_CLC_CLR_O  )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_CLC_INT_I  )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_CLC_SHDN_O )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_BAT_CHG_I  )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CD     )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_ACC_INT_I  )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_ACC_FSYNC_I)        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D0      )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D1      )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D2      )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_D3     )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CLK     )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_13         )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_IN   )        |\
                                     PIN_PUPDR_FLOATING(GPIOC_OSC32_OUT  ))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_CLC_POL_I )              |\
                                     PIN_ODR_HIGH(GPIOC_CLC_CLR_O  )              |\
                                     PIN_ODR_HIGH(GPIOC_CLC_INT_I  )              |\
                                     PIN_ODR_HIGH(GPIOC_CLC_SHDN_O )              |\
                                     PIN_ODR_HIGH(GPIOC_BAT_CHG_I  )              |\
                                     PIN_ODR_HIGH(GPIOC_SD_CD     )              |\
                                     PIN_ODR_HIGH(GPIOC_ACC_INT_I  )              |\
                                     PIN_ODR_HIGH(GPIOC_ACC_FSYNC_I)              |\
                                     PIN_ODR_HIGH(GPIOC_SD_D0      )              |\
                                     PIN_ODR_HIGH(GPIOC_SD_D1      )              |\
                                     PIN_ODR_HIGH(GPIOC_SD_D2      )              |\
                                     PIN_ODR_HIGH(GPIOC_SD_D3     )              |\
                                     PIN_ODR_HIGH(GPIOC_SD_CLK     )              |\
                                     PIN_ODR_HIGH(GPIOC_13         )              |\
                                     PIN_ODR_HIGH(GPIOC_OSC32_IN   )              |\
                                     PIN_ODR_HIGH(GPIOC_OSC32_OUT  ))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_CLC_POL_I ,0)               |\
                                     PIN_AFIO_AF(GPIOC_CLC_CLR_O ,0  )               |\
                                     PIN_AFIO_AF(GPIOC_CLC_INT_I ,0  )               |\
                                     PIN_AFIO_AF(GPIOC_CLC_SHDN_O,0)               |\
                                     PIN_AFIO_AF(GPIOC_BAT_CHG_I ,0  )               |\
                                     PIN_AFIO_AF(GPIOC_SD_CD     ,0)                |\
                                     PIN_AFIO_AF(GPIOC_ACC_INT_I ,0 )               |\
                                     PIN_AFIO_AF(GPIOC_ACC_FSYNC_I,0))              
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SD_D0     ,12)               |\
                                     PIN_AFIO_AF(GPIOC_SD_D1     ,12)               |\
                                     PIN_AFIO_AF(GPIOC_SD_D2     ,12)               |\
                                     PIN_AFIO_AF(GPIOC_SD_D3     ,12)               |\
                                     PIN_AFIO_AF(GPIOC_SD_CLK    ,12)               |\
                                     PIN_AFIO_AF(GPIOC_13        ,0)               |\
                                     PIN_AFIO_AF(GPIOC_OSC32_IN  ,0)               |\
                                     PIN_AFIO_AF(GPIOC_OSC32_OUT ,0))


#define VAL_GPIOD_MODER             (PIN_MODE_ALTERNATE(0 )        |\
                                     PIN_MODE_ALTERNATE(1  )        |\
                                     PIN_MODE_ALTERNATE(GPIOD_SDIO_CMD  )        |\
                                     PIN_MODE_ALTERNATE(3 )        |\
                                     PIN_MODE_ALTERNATE(4  )        |\
                                     PIN_MODE_ALTERNATE(5     )        |\
                                     PIN_MODE_ALTERNATE(6  )        |\
                                     PIN_MODE_ALTERNATE(7)        |\
                                     PIN_MODE_ALTERNATE(8      )        |\
                                     PIN_MODE_ALTERNATE(9      )        |\
                                     PIN_MODE_ALTERNATE(10      )        |\
                                     PIN_MODE_ALTERNATE(11     )        |\
                                     PIN_MODE_ALTERNATE(12     )        |\
                                     PIN_MODE_ALTERNATE(13         )        |\
                                     PIN_MODE_ALTERNATE(14   )        |\
                                     PIN_MODE_ALTERNATE(15  ))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(0 )        |\
                                     PIN_OTYPE_PUSHPULL(1  )        |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SDIO_CMD  )        |\
                                     PIN_OTYPE_PUSHPULL(3 )        |\
                                     PIN_OTYPE_PUSHPULL(4  )        |\
                                     PIN_OTYPE_PUSHPULL(5     )        |\
                                     PIN_OTYPE_PUSHPULL(6  )        |\
                                     PIN_OTYPE_PUSHPULL(7)        |\
                                     PIN_OTYPE_PUSHPULL(8      )        |\
                                     PIN_OTYPE_PUSHPULL(9      )        |\
                                     PIN_OTYPE_PUSHPULL(10      )        |\
                                     PIN_OTYPE_PUSHPULL(11     )        |\
                                     PIN_OTYPE_PUSHPULL(12     )        |\
                                     PIN_OTYPE_PUSHPULL(13         )        |\
                                     PIN_OTYPE_PUSHPULL(14   )        |\
                                     PIN_OTYPE_PUSHPULL(15  ))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(0 )           |\
                                     PIN_OSPEED_HIGH(1  )           |\
                                     PIN_OSPEED_HIGH(GPIOD_SDIO_CMD  )           |\
                                     PIN_OSPEED_HIGH(3 )           |\
                                     PIN_OSPEED_HIGH(4  )           |\
                                     PIN_OSPEED_HIGH(5     )           |\
                                     PIN_OSPEED_HIGH(6  )           |\
                                     PIN_OSPEED_HIGH(7)           |\
                                     PIN_OSPEED_HIGH(8      )           |\
                                     PIN_OSPEED_HIGH(9      )           |\
                                     PIN_OSPEED_HIGH(10      )           |\
                                     PIN_OSPEED_HIGH(11     )           |\
                                     PIN_OSPEED_HIGH(12     )           |\
                                     PIN_OSPEED_HIGH(13         )           |\
                                     PIN_OSPEED_HIGH(14   )           |\
                                     PIN_OSPEED_HIGH(15  ))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(0 )        |\
                                     PIN_PUPDR_FLOATING(1  )        |\
                                     PIN_PUPDR_FLOATING(GPIOD_SDIO_CMD  )        |\
                                     PIN_PUPDR_FLOATING(3 )        |\
                                     PIN_PUPDR_FLOATING(4  )        |\
                                     PIN_PUPDR_FLOATING(5     )        |\
                                     PIN_PUPDR_FLOATING(6  )        |\
                                     PIN_PUPDR_FLOATING(7)        |\
                                     PIN_PUPDR_FLOATING(8      )        |\
                                     PIN_PUPDR_FLOATING(9      )        |\
                                     PIN_PUPDR_FLOATING(10      )        |\
                                     PIN_PUPDR_FLOATING(11     )        |\
                                     PIN_PUPDR_FLOATING(12     )        |\
                                     PIN_PUPDR_FLOATING(13         )        |\
                                     PIN_PUPDR_FLOATING(14   )        |\
                                     PIN_PUPDR_FLOATING(15  ))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(0 )              |\
                                     PIN_ODR_HIGH(1  )              |\
                                     PIN_ODR_HIGH(GPIOD_SDIO_CMD  )              |\
                                     PIN_ODR_HIGH(3 )              |\
                                     PIN_ODR_HIGH(4  )              |\
                                     PIN_ODR_HIGH(5     )              |\
                                     PIN_ODR_HIGH(6  )              |\
                                     PIN_ODR_HIGH(7)              |\
                                     PIN_ODR_HIGH(8      )              |\
                                     PIN_ODR_HIGH(9      )              |\
                                     PIN_ODR_HIGH(10      )              |\
                                     PIN_ODR_HIGH(11     )              |\
                                     PIN_ODR_HIGH(12     )              |\
                                     PIN_ODR_HIGH(13         )              |\
                                     PIN_ODR_HIGH(14   )              |\
                                     PIN_ODR_HIGH(15  ))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(0 ,0)               |\
                                     PIN_AFIO_AF(1 ,0  )               |\
                                     PIN_AFIO_AF(GPIOD_SDIO_CMD ,12  )               |\
                                     PIN_AFIO_AF(3,0)               |\
                                     PIN_AFIO_AF(4 ,0  )               |\
                                     PIN_AFIO_AF(5     ,0)                |\
                                     PIN_AFIO_AF(6 ,0 )               |\
                                     PIN_AFIO_AF(7,0))              
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(8     ,12)               |\
                                     PIN_AFIO_AF(9     ,12)               |\
                                     PIN_AFIO_AF(10     ,12)               |\
                                     PIN_AFIO_AF(11     ,12)               |\
                                     PIN_AFIO_AF(12    ,12)               |\
                                     PIN_AFIO_AF(13        ,0)               |\
                                     PIN_AFIO_AF(14  ,0)               |\
                                     PIN_AFIO_AF(15 ,0))




#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
