	/* USER CODE BEGIN Header */
	/**
	  ******************************************************************************
	  * @file           : main.c
	  * @brief          : Main program body
	  ******************************************************************************
	  * @attention
	  *
	  * CopyRight (c) 2026 STMicroelectronics.
	  * All Rights reserved.
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
	#include<stdbool.h>
	#include<string.h>
	#include<stdio.h>
	#include<stdlib.h>


	#include "EncoderLib.h"
	#include "PCA9685.h"
	#include "BTS7960.h"
	#include "Cytron.h"

	/* USER CODE END Includes */

	/* Private typedef -----------------------------------------------------------*/
	/* USER CODE BEGIN PTD */


	/* USER CODE END PTD */

	/* Private define ------------------------------------------------------------*/
	/* USER CODE BEGIN PD */
	#define PCA_FREQ 	50


	/* USER CODE END PD */

	/* Private macro -------------------------------------------------------------*/
	/* USER CODE BEGIN PM */
	#define DATA_SIZE 7
	#define SLAVE_ADDR 8

	#define DEADZONE  20

	/* USER CODE END PM */

	/* Private variables ---------------------------------------------------------*/
	I2C_HandleTypeDef hi2c1;
	I2C_HandleTypeDef hi2c2;
	I2C_HandleTypeDef hi2c3;

	TIM_HandleTypeDef htim4;
	TIM_HandleTypeDef htim5;
	TIM_HandleTypeDef htim8;
	TIM_HandleTypeDef htim9;
	TIM_HandleTypeDef htim11;

	UART_HandleTypeDef huart3;

	/* USER CODE BEGIN PV */
	int8_t DATA[DATA_SIZE];
	char MSG[300];

	typedef struct{
		int LX;
		int LY;
		int RX;
		int RY;
		int Omega;

		bool Right;
		bool Down;
		bool Up;
		bool Left;
		bool Square;
		bool Cross;
		bool Circle;
		bool Triangle;

		bool PS;
		bool Touchpad;
		bool Options;
		bool Share;
		bool R3;
		bool L3;
		bool R1;
		bool L1;

	}PS4Data;
	PS4Data PS4 = { 0 };

	typedef struct{
		bool prevState;
		bool flag;
	}ToggleButton;

	ToggleButton KFSPlaceServo;
	ToggleButton KFSGripServo;

	ToggleButton StaffAttackServo;
	ToggleButton StaffGripServo;

	PCA_t PCA;

	typedef struct{
		TIM_HandleTypeDef *htim1;
		uint32_t LPWM;
		uint8_t bus1;

		TIM_HandleTypeDef *htim2;
		uint32_t RPWM;
		uint8_t bus2;

		uint16_t freq;
		uint8_t  maxPWM;

	} BTSMotor_t;

	typedef struct {
		TIM_HandleTypeDef *timer;
		GPIO_TypeDef *dirGPIO;

		uint32_t channel;
		uint16_t dirPIN;
		uint8_t direction;
		uint8_t  maxPWM;

	} CytronMotor_t;

	typedef struct {
		uint8_t PCA_Ch;
		uint8_t GripAngle;
		uint8_t ReleaseAngle;
	} GripServo_t;

	typedef struct {
		uint8_t PCA_Ch;
		uint8_t NormalAngle;
		uint8_t AttackAngle;
	} AttackServo_t;


	// ============================== KFS PLACING ==============================
	BTSMotor_t KFS_Place;
	AttackServo_t KFS_Flap;

	BTSMotor_t KFS_Place= {
		.htim1 		= &htim5,
		.LPWM 	= TIM_CHANNEL_2,
		.bus1 		= 2,

		.htim2 		= &htim5,
		.RPWM 	= TIM_CHANNEL_3,
		.bus2 		= 2,

		.freq = 1000
	};

	AttackServo_t KFS_Flap = {
		.PCA_Ch 		= 12,
		.NormalAngle 	= 0,
		.AttackAngle 	= 180
	};


	// ============================== KFS PICKING ==============================
	CytronMotor_t KFS_Belt;
	CytronMotor_t KFS_Turret;
	GripServo_t KFS_Grip[2];


	CytronMotor_t KFS_Belt = {
			.timer 		= &htim9,
			.dirGPIO	= GPIOB,
			.channel 	= TIM_CHANNEL_2,
			.dirPIN 	= GPIO_PIN_15,
			.direction  = 0,
			.maxPWM 	= 60
	};

	CytronMotor_t KFS_Turret = {
			.timer 		= &htim11,
			.dirGPIO	= GPIOA,
			.channel 	= TIM_CHANNEL_1,
			.dirPIN 	= GPIO_PIN_2,
			.direction  = 0,
			.maxPWM 	= 60
	};

	GripServo_t KFS_Grip[2] = {
		{.PCA_Ch = 8, .GripAngle = 60, .ReleaseAngle = 160},
		{.PCA_Ch = 9, .GripAngle = 160, .ReleaseAngle = 60}
	};

	// ============================== STAFF PICKING ==============================
	CytronMotor_t STAFF_Motor;
	GripServo_t STAFF_Grip[4];
	AttackServo_t STAFF_Attack[4];

	CytronMotor_t STAFF_Motor = {
			.timer 		= &htim8,
			.dirGPIO	= GPIOA,
			.channel 	= TIM_CHANNEL_3,
			.dirPIN 	= GPIO_PIN_3,
			.direction  = 0,
			.maxPWM 	= 40

	};

	GripServo_t STAFF_Grip[4] = {
		{ .PCA_Ch = 0, .GripAngle = 180, .ReleaseAngle = 0 },
		{ .PCA_Ch = 1, .GripAngle = 110, .ReleaseAngle = 50 },
		{ .PCA_Ch = 3, .GripAngle = 180, .ReleaseAngle = 0 },
		{ .PCA_Ch = 4, .GripAngle = 180, .ReleaseAngle = 0 }
	};

	AttackServo_t STAFF_Attack[4] = {
		{ .PCA_Ch = 4, .NormalAngle = 0,   .AttackAngle = 45 },
		{ .PCA_Ch = 5, .NormalAngle = 110, .AttackAngle = 63 },
		{ .PCA_Ch = 6, .NormalAngle = 15,  .AttackAngle = 58 },
		{ .PCA_Ch = 7, .NormalAngle = 67,  .AttackAngle = 110 }
	};


	/* USER CODE END PV */

	/* Private function prototypes -----------------------------------------------*/
	void SystemClock_Config(void);
	static void MX_GPIO_Init(void);
	static void MX_I2C1_Init(void);
	static void MX_USART3_UART_Init(void);
	static void MX_I2C2_Init(void);
	static void MX_I2C3_Init(void);
	static void MX_TIM4_Init(void);
	static void MX_TIM5_Init(void);
	static void MX_TIM8_Init(void);
	static void MX_TIM9_Init(void);
	static void MX_TIM11_Init(void);
	/* USER CODE BEGIN PFP */
	// ============================== UTILITY ==============================
	void getValue();


	/* USER CODE END PFP */

	/* Private user code ---------------------------------------------------------*/
	/* USER CODE BEGIN 0 */
	void getValue(){
		if (HAL_OK == HAL_I2C_Master_Receive(&hi2c1, SLAVE_ADDR << 1, (uint8_t*)DATA, DATA_SIZE, HAL_MAX_DELAY)){
			PS4.Up       = (DATA[0] & (1 << 0)) ? 1 : 0;
			PS4.Right    = (DATA[0] & (1 << 1)) ? 1 : 0;
			PS4.Down     = (DATA[0] & (1 << 2)) ? 1 : 0;
			PS4.Left     = (DATA[0] & (1 << 3)) ? 1 : 0;
			PS4.Triangle = (DATA[0] & (1 << 4)) ? 1 : 0;
			PS4.Circle   = (DATA[0] & (1 << 5)) ? 1 : 0;
			PS4.Cross    = (DATA[0] & (1 << 6)) ? 1 : 0;
			PS4.Square   = (DATA[0] & (1 << 7)) ? 1 : 0;

			PS4.L1       = (DATA[1] & (1 << 0)) ? 1 : 0;
			PS4.R1       = (DATA[1] & (1 << 1)) ? 1 : 0;
			PS4.L3       = (DATA[1] & (1 << 2)) ? 1 : 0;
			PS4.R3       = (DATA[1] & (1 << 3)) ? 1 : 0;
			PS4.Share    = (DATA[1] & (1 << 4)) ? 1 : 0;
			PS4.Options  = (DATA[1] & (1 << 5)) ? 1 : 0;
			PS4.Touchpad = (DATA[1] & (1 << 6)) ? 1 : 0;
			PS4.PS       = (DATA[1] & (1 << 7)) ? 1 : 0;

			PS4.Omega = DATA[6];
			PS4.RY    = DATA[5];
			PS4.RX    = DATA[4];
			PS4.LX    = DATA[2];
			PS4.LY    = DATA[3];

			if (abs(PS4.LX) < DEADZONE) PS4.LX = 0;
			if (abs(PS4.LY) < DEADZONE) PS4.LY = 0;
			if (abs(PS4.Omega) < DEADZONE) PS4.Omega = 0;

			/* -------- DEBUG PRINT -------- */
			sprintf(MSG,
					" SQ%d X %d  O %d  TRI %d  LEFT %d  DOWN %d  RIGHT %d  UP %d  PS %d  TOUCH %d  OPT %d  SHARE %d  R3 %d  L3 %d  R1 %d  L1 %d LX %d  LY %d  RX %d  RY %d  OMEGA%d \r\n",
					PS4.Square, PS4.Cross, PS4.Circle, PS4.Triangle,
					PS4.Left, PS4.Down, PS4.Right, PS4.Up,
					PS4.PS, PS4.Touchpad, PS4.Options, PS4.Share,
					PS4.R3, PS4.L3, PS4.R1, PS4.L1,
					PS4.LX, PS4.LY, PS4.RX, PS4.RY, PS4.Omega);

			HAL_UART_Transmit(&huart3, (uint8_t*)MSG, strlen(MSG), HAL_MAX_DELAY);
		}
	}

	void InitMechanisms(){
		InitBTS(KFS_Place, KFS_Place.htim1, KFS_Place.LPWM, KFS_Place.bus1, KFS_Place.htim2, KFS_Place.RPWM, KFS_Place.bus2, KFS_Place.freq);

		InitCytron(STAFF_Motor, STAFF_Motor.timer, STAFF_Motor.channel, STAFF_Motor.dirGPIO, STAFF_Motor.dirPIN, STAFF_Motor.direction);
		InitCytron(KFS_Turret, KFS_Turret.timer, KFS_Turret.channel, KFS_Turret.dirGPIO, KFS_Turret.dirPIN, KFS_Turret.direction);
		InitCytron(KFS_Belt, KFS_Belt.timer, KFS_Belt.channel, KFS_Belt.dirGPIO, KFS_Belt.dirPIN, KFS_Belt.direction);

		InitPCA(&PCA, &hi2c2, PCA_FREQ);
	}


	//============================== KFS PLACING ==============================
	void KFS_Place_Motor(){
		if(PS4.L1){
			RotateMotor(KFS_Place,  KFS_Place.maxPWM);
		}
		else if(PS4.R1){
			RotateMotor(KFS_Place, -KFS_Place.maxPWM);
		}
		else{
			StopMotor(KFS_Place);
		}
	}

	void KFS_Place_Servo(){
		if(PS4.Cross && !KFSPlaceServo.prevState){
			KFSPlaceServo.prevState = true;
			KFSPlaceServo.flag = !KFSPlaceServo.flag;

			if(KFSPlaceServo.flag){
				RotateServo(&PCA, KFS_Flap.PCA_Ch, KFS_Flap.NormalAngle);
			}
			else{
				RotateServo(&PCA, KFS_Flap.PCA_Ch, KFS_Flap.AttackAngle);
			}
		}
		else if(!PS4.Cross){
			KFSPlaceServo.prevState = false;
		}

	}

	void KFS_Place_Motion(){
		KFS_Place_Motor();
		KFS_Place_Servo();
	}

	//============================== KFS PICKING ==============================
	void KFS_Pick_Motor(){
		// BELT-MOTOR
		if(PS4.Up){
			RotateCytron(KFS_Belt,  KFS_Belt.maxPWM);
		}
		else if(PS4.Down){
			RotateCytron(KFS_Belt, -KFS_Belt.maxPWM);
		}
		else{
			StopCytron(KFS_Belt);
		}


		// TURRET_MOTOR
		if(PS4.Up){
			RotateCytron(KFS_Turret,  KFS_Turret.maxPWM);
		}
		else if(PS4.Down){
			RotateCytron(KFS_Turret, -KFS_Turret.maxPWM);
		}
		else{
			StopCytron(KFS_Turret);
		}

	}

	void KFS_Pick_Servo(){
		if(PS4.Circle && !KFSGripServo.prevState){
			KFSGripServo.prevState = true;
			KFSGripServo.flag 		= !KFSGripServo.flag;

				if(KFSGripServo.flag){
					RotateServo(&PCA, KFS_Grip[1].PCA_Ch, KFS_Grip[1].GripAngle);
					RotateServo(&PCA, KFS_Grip[2].PCA_Ch, KFS_Grip[2].GripAngle);
				}
				else{
					RotateServo(&PCA, KFS_Grip[1].PCA_Ch, KFS_Grip[1].ReleaseAngle);
					RotateServo(&PCA, KFS_Grip[2].PCA_Ch, KFS_Grip[2].ReleaseAngle);
				}
			}
			else if (!PS4.Circle){
				KFSGripServo.prevState = false;
			}
	}

	void KFS_Pick_Motion(){
		KFS_Pick_Motor();
		KFS_Pick_Servo();
	}


	//============================== KFS PICKING ==============================
	void STAFF_Motor(){
		if(PS4.RY){
			int16_t beltSpeed = (int16_t)constrain(PS4.RY, -MAX_STAFF_PWM, MAX_STAFF_PWM);
			RotateCytron(&STAFF_MOTOR, beltSpeed);
		}
		else{
			StopCytron(STAFF_Motor);
		}
	}

	void STAFF_Servo(){
		// GRIP SERVO
		if(PS4.Square && !StaffGripServo.prevState){
			StaffGripServo.prevState = true;
			StaffGripServo.flag 		= !StaffGripServo.flag;

			if(StaffGripServo.flag){
				RotateServo(&PCA, STAFF_Grip[1].PCA_Ch, STAFF_Grip[1].GripAngle);
				RotateServo(&PCA, STAFF_Grip[2].PCA_Ch, STAFF_Grip[2].GripAngle);
				RotateServo(&PCA, STAFF_Grip[3].PCA_Ch, STAFF_Grip[3].GripAngle);
				RotateServo(&PCA, STAFF_Grip[4].PCA_Ch, STAFF_Grip[4].GripAngle);
			}
			else{
				RotateServo(&PCA, STAFF_Grip[1].PCA_Ch, STAFF_Grip[1].ReleaseAngle);
				RotateServo(&PCA, STAFF_Grip[2].PCA_Ch, STAFF_Grip[2].ReleaseAngle);
				RotateServo(&PCA, STAFF_Grip[3].PCA_Ch, STAFF_Grip[3].ReleaseAngle);
				RotateServo(&PCA, STAFF_Grip[4].PCA_Ch, STAFF_Grip[4].ReleaseAngle);
			}
		}
		else if (!PS4.Square){
			StaffGripServo.prevState = false;
		}


		// ATTACK SERVO
		if(PS4.Triangle && !AttackServo.prevState){
			AttackServo.prevState = true;
			AttackServo.flag 	  = !AttackServo.flag;

			if(AttackServo.flag){
				RotateServo(&PCA, STAFF_Attack[1].PCA_Ch, STAFF_Attack[1].NormalAngle);
				RotateServo(&PCA, STAFF_Attack[2].PCA_Ch, STAFF_Attack[2].NormalAngle);
				RotateServo(&PCA, STAFF_Attack[3].PCA_Ch, STAFF_Attack[3].NormalAngle);
				RotateServo(&PCA, STAFF_Attack[4].PCA_Ch, STAFF_Attack[4].NormalAngle);
			}
			else{
			}
		}
		else if(!PS4.Triangle){
			RotateServo(&PCA, STAFF_Attack[1].PCA_Ch, STAFF_Attack[1].AttackAngle);
			RotateServo(&PCA, STAFF_Attack[2].PCA_Ch, STAFF_Attack[2].AttackAngle);
			RotateServo(&PCA, STAFF_Attack[3].PCA_Ch, STAFF_Attack[3].AttackAngle);
			RotateServo(&PCA, STAFF_Attack[4].PCA_Ch, STAFF_Attack[4].AttackAngle);
		}
	}

	void STAFF_Motion(){
		STAFF_Motion();
		STAFF_Servo();
	}




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
	  MX_I2C1_Init();
	  MX_USART3_UART_Init();
	  MX_I2C2_Init();
	  MX_I2C3_Init();
	  MX_TIM4_Init();
	  MX_TIM5_Init();
	  MX_TIM8_Init();
	  MX_TIM9_Init();
	  MX_TIM11_Init();
	  /* USER CODE BEGIN 2 */

	  InitMechanisms();

	  /* USER CODE END 2 */

	  /* Infinite loop */
	  /* USER CODE BEGIN WHILE */
	  while (1)
	  {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		  getValue();
		  KFS_Place_Motion();
		  KFS_Pick_Motion();
		  STAFF_Motion();

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
	  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	  /** Initializes the RCC Oscillators according to the specified parameters
	  * in the RCC_OscInitTypeDef structure.
	  */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	  {
		Error_Handler();
	  }

	  /** Initializes the CPU, AHB and APB buses clocks
	  */
	  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
								  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	  {
		Error_Handler();
	  }
	}

	/**
	  * @brief I2C1 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_I2C1_Init(void)
	{

	  /* USER CODE BEGIN I2C1_Init 0 */

	  /* USER CODE END I2C1_Init 0 */

	  /* USER CODE BEGIN I2C1_Init 1 */

	  /* USER CODE END I2C1_Init 1 */
	  hi2c1.Instance = I2C1;
	  hi2c1.Init.ClockSpeed = 100000;
	  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	  hi2c1.Init.OwnAddress1 = 0;
	  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c1.Init.OwnAddress2 = 0;
	  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN I2C1_Init 2 */

	  /* USER CODE END I2C1_Init 2 */

	}

	/**
	  * @brief I2C2 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_I2C2_Init(void)
	{

	  /* USER CODE BEGIN I2C2_Init 0 */

	  /* USER CODE END I2C2_Init 0 */

	  /* USER CODE BEGIN I2C2_Init 1 */

	  /* USER CODE END I2C2_Init 1 */
	  hi2c2.Instance = I2C2;
	  hi2c2.Init.ClockSpeed = 100000;
	  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
	  hi2c2.Init.OwnAddress1 = 0;
	  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c2.Init.OwnAddress2 = 0;
	  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN I2C2_Init 2 */

	  /* USER CODE END I2C2_Init 2 */

	}

	/**
	  * @brief I2C3 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_I2C3_Init(void)
	{

	  /* USER CODE BEGIN I2C3_Init 0 */

	  /* USER CODE END I2C3_Init 0 */

	  /* USER CODE BEGIN I2C3_Init 1 */

	  /* USER CODE END I2C3_Init 1 */
	  hi2c3.Instance = I2C3;
	  hi2c3.Init.ClockSpeed = 100000;
	  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
	  hi2c3.Init.OwnAddress1 = 0;
	  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  hi2c3.Init.OwnAddress2 = 0;
	  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN I2C3_Init 2 */

	  /* USER CODE END I2C3_Init 2 */

	}

	/**
	  * @brief TIM4 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_TIM4_Init(void)
	{

	  /* USER CODE BEGIN TIM4_Init 0 */

	  /* USER CODE END TIM4_Init 0 */

	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};

	  /* USER CODE BEGIN TIM4_Init 1 */

	  /* USER CODE END TIM4_Init 1 */
	  htim4.Instance = TIM4;
	  htim4.Init.Prescaler = 0;
	  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim4.Init.Period = 65535;
	  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM4_Init 2 */

	  /* USER CODE END TIM4_Init 2 */
	  HAL_TIM_MspPostInit(&htim4);

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

	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};

	  /* USER CODE BEGIN TIM5_Init 1 */

	  /* USER CODE END TIM5_Init 1 */
	  htim5.Instance = TIM5;
	  htim5.Init.Prescaler = 0;
	  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim5.Init.Period = 4294967295;
	  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
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
	  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM5_Init 2 */

	  /* USER CODE END TIM5_Init 2 */
	  HAL_TIM_MspPostInit(&htim5);

	}

	/**
	  * @brief TIM8 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_TIM8_Init(void)
	{

	  /* USER CODE BEGIN TIM8_Init 0 */

	  /* USER CODE END TIM8_Init 0 */

	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};
	  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	  /* USER CODE BEGIN TIM8_Init 1 */

	  /* USER CODE END TIM8_Init 1 */
	  htim8.Instance = TIM8;
	  htim8.Init.Prescaler = 0;
	  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim8.Init.Period = 65535;
	  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim8.Init.RepetitionCounter = 0;
	  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	  sBreakDeadTimeConfig.DeadTime = 0;
	  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM8_Init 2 */

	  /* USER CODE END TIM8_Init 2 */
	  HAL_TIM_MspPostInit(&htim8);

	}

	/**
	  * @brief TIM9 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_TIM9_Init(void)
	{

	  /* USER CODE BEGIN TIM9_Init 0 */

	  /* USER CODE END TIM9_Init 0 */

	  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};

	  /* USER CODE BEGIN TIM9_Init 1 */

	  /* USER CODE END TIM9_Init 1 */
	  htim9.Instance = TIM9;
	  htim9.Init.Prescaler = 0;
	  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim9.Init.Period = 65535;
	  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_Init(&htim9) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_ConfigChannel(&htim9, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM9_Init 2 */

	  /* USER CODE END TIM9_Init 2 */
	  HAL_TIM_MspPostInit(&htim9);

	}

	/**
	  * @brief TIM11 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_TIM11_Init(void)
	{

	  /* USER CODE BEGIN TIM11_Init 0 */

	  /* USER CODE END TIM11_Init 0 */

	  TIM_OC_InitTypeDef sConfigOC = {0};

	  /* USER CODE BEGIN TIM11_Init 1 */

	  /* USER CODE END TIM11_Init 1 */
	  htim11.Instance = TIM11;
	  htim11.Init.Prescaler = 0;
	  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim11.Init.Period = 65535;
	  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
	  {
		Error_Handler();
	  }
	  if (HAL_TIM_PWM_Init(&htim11) != HAL_OK)
	  {
		Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = 0;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim11, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN TIM11_Init 2 */

	  /* USER CODE END TIM11_Init 2 */
	  HAL_TIM_MspPostInit(&htim11);

	}

	/**
	  * @brief USART3 Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_USART3_UART_Init(void)
	{

	  /* USER CODE BEGIN USART3_Init 0 */

	  /* USER CODE END USART3_Init 0 */

	  /* USER CODE BEGIN USART3_Init 1 */

	  /* USER CODE END USART3_Init 1 */
	  huart3.Instance = USART3;
	  huart3.Init.BaudRate = 115200;
	  huart3.Init.WordLength = UART_WORDLENGTH_8B;
	  huart3.Init.StopBits = UART_STOPBITS_1;
	  huart3.Init.Parity = UART_PARITY_NONE;
	  huart3.Init.Mode = UART_MODE_TX_RX;
	  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	  if (HAL_UART_Init(&huart3) != HAL_OK)
	  {
		Error_Handler();
	  }
	  /* USER CODE BEGIN USART3_Init 2 */

	  /* USER CODE END USART3_Init 2 */

	}

	/**
	  * @brief GPIO Initialization Function
	  * @param None
	  * @retval None
	  */
	static void MX_GPIO_Init(void)
	{
	  /* USER CODE BEGIN MX_GPIO_Init_1 */

	  /* USER CODE END MX_GPIO_Init_1 */

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOE_CLK_ENABLE();
	  __HAL_RCC_GPIOF_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	  /* USER CODE BEGIN MX_GPIO_Init_2 */

	  /* USER CODE END MX_GPIO_Init_2 */
	}

	/* USER CODE BEGIN 4 */

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
	#ifdef USE_FULL_ASSERT
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
