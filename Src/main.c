/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"

/* USER CODE BEGIN Includes */
#include "LiquidCrystal.h"
#include "stdlib.h"
#include "stdio.h"
#include <string.h> 
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim8;

UART_HandleTypeDef huart3;

PCD_HandleTypeDef hpcd_USB_FS;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
	typedef unsigned char byte;
//	/////////////////////////////////////////////
	byte airplanes[] = {
	0x00,
	0x00,
	0x04,
	0x12,
	0x0F,
	0x12,
	0x04,
	0x00
};
	byte barrier11[] = {
	0x00,
	0x00,
	0x1F,
	0x11,
	0x1F,
	0x11,
	0x1F,
	0x00
};
	byte barrier22[] = {
	0x00,
	0x00,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x1F,
	0x00
};
	byte fuel_source[] = {
	0x00,
	0x04,
	0x0A,
	0x15,
	0x0A,
	0x04,
	0x00,
	0x00
};
	
//	byte filled[] = { 
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F
//};
//	
//	byte down_square[] = { 
//	0x00,
//	0x00,
//	0x00,
//	0x00,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F
//};
//	
//	byte up_square [] = { 
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x00,
//	0x00,
//	0x00,
//	0x00
//	};
//	
//	byte trapeze[] = { 
//	0x01,
//	0x03,
//	0x07,
//	0x0F,
//	0x1F,
//	0x1F,
//	0x1F,
//	0x1F};
	
	int adc_counter = 0; 
	int speedSum = 0; 
	
//	byte up[] = {0x00,0x00,0x04,0x0A,0x11,0x00,0x00,0x00};//z add
//	byte down[] = {0x00,0x00,0x11,0x0A,0x04,0x00,0x00,0x00};//z add
	/////////////////////////////////////////////
	byte selected[]= {0x00,0x00,0x04,0x02,0x1F,0x02,0x04,0x00}; //selected & bonus
	
	
	////////////////////////////////////////////////////////////////////////////////important
	char idchar;
	int test=0;
	int flagID=0;
	char idstr[20]="";
	int idcurser=5;
	int step=0;
	int speed=60;
	int needClear=0;
	int map[20][4] = {0}; //"0" shows empty, "1, 2, 3" shows a barrier, 
	//and "-1, -2, -3, -4" shoes an enenmy, "-5" for cursor, "10" for bonus
	
	int option; // "0" for NEW GAME, "1" for LOAD GAME, "2" for ABOUT
		
	int mode; //"-1" for Showing "Alien Fighters", "0" for Showing MENU, "1" for before starting GAME, 
	//"2" during the GAME, and "3" About, "4" for game over, "5" for winner , "6" load game,"7" store game
	
	char strvol[20];
	int time_counter = 0;
	int time = 0;
	
	int direction = -1;
	
	typedef struct barrierrs { 
	int timeToBirth;
  int type;
	int x, y;
	int isExist;
} barriers;
	
	typedef struct airplane{ 
  int fuel;
  int planeSpeed;
	int x, y;
	int accident;
} airplane;
	
	typedef struct source_fuel{ 
  int value;
	int x, y;
	int timeTobBirth; 
	int isExist;
} source_fuel;
	
	int barriers1[5]={8,6,4,2,0} ;
	int barriers2[5]={0,2,4,6,8};
	int source_fuel_num[5] = {5,3,2,1,0};
	airplane plane1;
	
	barriers brr[5][8];
	source_fuel fuel;
	
	int num_step=8;
	int  total_step=5;
	int counterMode2=0;
	int length=20;
	int score = 0;
	
	int max_speed_step[]={90,200,300,400,500};
	int max_time_step[]={100,80,60,40,20};	///using in calculate speed
	
	
	////////////////////////////////////////////////////////////////////////////////
  ///////////////////////////OUR METHODS/////////////////////////////////////////
//	void effects(){
//		clear();
//		for(int i=0;i<20;i++){
//			for(int j=0;j<4;j++){
//			setCursor(i,j);
//			write(7);
//			}
//		}
//		HAL_Delay(1500);
//		for(int i=6;i<12;i++){
//			for(int j=0;j<4;j++){
//			setCursor(i,j);
//			 print(" ");
//				if((i==7 || i==11) && (j==0 || j==3)){
//					setCursor(i,j);
//					write(7);
//				}
//			}
//		}
//		HAL_Delay(1500);
//		for(int i=1;i<17;i++){
//			for(int j=0;j<4;j++){
//			setCursor(i,j);
//			 print(" ");
//				if((i==1 || i==17) && (j==0 || j==3)){
//					setCursor(i,j);
//					write(7);
//				}
//			}
//		}
//		HAL_Delay(1500);
//		clear();
//		//3
//		for(int i=8;i<12;i++){
//			for(int j=0;j<4;j++){
//			setCursor(i,j);
//			 print(" ");
//				if((j==0 || j==3)){
//					setCursor(i,j);
//					write(7);
//				}
//				else if(j==1){
//					setCursor(i,j);
//					write(8);
//				}
//				else {
//					setCursor(i,j);
//					write(9);
//				}
//			}
//		}
//		setCursor(13,1);
//		write(7);
//		setCursor(13,2);
//		write(7);
//		//3
//		HAL_Delay(1500);
//		clear();
//		//2
//		for(int i=8;i<10;i++){
//			for(int j=0;j<4;j++){
//			setCursor(i,j);
//			 print(" ");
//				if((j==0 || j==3)){
//					setCursor(i,j);
//					write(7);
//				}
//				else if(j==1){
//					setCursor(i,j);
//					write(8);
//				}
//				else {
//					setCursor(i,j);
//					write(9);
//				}
//			}
//		}
//		setCursor(7,3);
//		write(7);
//		setCursor(11,3);
//		write(8);
//		setCursor(11,0);
//		write(7);
//		setCursor(7,0);
//		write(9);
//		setCursor(7,1);
//		write(7);
//		setCursor(11,2);
//		write(7);	
//		//2
//		HAL_Delay(1500);
//		//1
//		clear();
//		setCursor(8,0);
//		write(6);
//		setCursor(9,0);
//		write(7);
//		setCursor(9,1);
//		write(7);
//		setCursor(9,2);
//		write(7);
//		setCursor(9,3);
//		write(7);
//		setCursor(7,3);
//		write(7);	
//		setCursor(8,3);
//		write(7);	
//		setCursor(10,3);
//		write(7);	
//		setCursor(11,3);
//		write(7);			
//		//1
//		HAL_Delay(1500);
//		clear();
//	}
	
	
	void new_barrier(barriers b){
		//check if there is another item in that position via map
			setCursor(b.y,b.x);
			write(2);
	}
	
	int calculateSpeed(int volume,int step){
			int speed=volume*(max_speed_step[step]/63);
		  
	}
		int calculateTime(int speed){
			int time=max_time_step[step]*(max_speed_step[step]/speed);
		  
	}
		
	void move(barriers b){
		int x = b.x;
		int y = b.y;
		
		if(y-1==0) {
			b.isExist = 0;
			setCursor(y,x);
			print(" ");
			map[y][x] = 0;
		}
		else if(y-1 == plane1.y && x==plane1.x){
			if(b.type==1){
				speed -= 10;
				plane1.fuel -= 10;
			}
			else {
					plane1.accident = 1;
			}
		}
		
		else if(map[y-1][x] == 0) {
			setCursor(y,x);
			print(" ");
			setCursor(y-1,x);
			write(b.type);
			b.y= y-1;
			b.x=x;
			map[y][x] = 0;
			map[y-1][x] = -1 * (b.type);
		}
	else{
		for(int i=0;i<total_step;i++){
			for(int j=0;j<num_step;j++){
				if(brr[i][j].isExist && y-1 == brr[i][j].y){
					 break;
				}
			}
		}
	}

	}
	
	
	void GameInit(){
		HAL_Delay(100);
		  clear();
		  HAL_Delay(100);
		  step = 0;
			plane1.fuel=100;
			plane1.x=rand()%4;
		  plane1.y=0;
		  plane1.accident=0;
		  map[plane1.y][plane1.x] = 1;		
		for(int i=0;i<total_step;i++){
			for(int j=0;j<num_step;j++){
				brr[i][j].isExist = 1;
			}
		}		
			setCursor(plane1.y,plane1.x);
			//print("*");
	    write(3);
		  mode=1;
	}
	
	
	
	int isHereBarrier(int y,int x){
		for(int i=0;i<8;i++){
			if(brr[step][i].x == x && brr[step][i].y== y) {
				return i;
			}
		}
		return -1;
	}
	
	void crash(airplane p){
		if(map[p.y][p.x] == -1){
			p.fuel -=10;
			speed -= 10;
			if(isHereBarrier(p.y , p.x)!=-1) {brr[step][isHereBarrier(p.y , p.x)].isExist = 0;}
		}
		if(map[p.y][p.x] == -2){
			p.accident = 1;
			if(isHereBarrier(p.y , p.x)!=-1) {brr[step][isHereBarrier(p.y , p.x)].isExist = 0;}
		}
		if(map[p.y][p.x] == 2) {
			p.fuel += 10;
			fuel.isExist = 0;
		}
	}
	
	void moveAP(int dir){
		switch(dir){
			case 0:{ //up
				if(plane1.y <=18){
				map[plane1.y][plane1.x] = 0;
				setCursor(plane1.y , plane1.x);
				print(" ");
				plane1.y = plane1.y + 1;
				crash(plane1);
				map[plane1.y][plane1.x] = 1;
				setCursor(plane1.y , plane1.x);
				write(3);
				}
				break;
			}
			case 1:{ //down
				if(plane1.y >=1){
				map[plane1.y][plane1.x] = 0;
				setCursor(plane1.y , plane1.x);
				print(" ");
				plane1.y = plane1.y - 1;
				crash(plane1);
				map[plane1.y][plane1.x] = 1;
				setCursor(plane1.y , plane1.x);
				write(3);
				}
				break;
			}
			case 2:{ //right
				if(plane1.x <=2){
					map[plane1.y][plane1.x] = 0;
				setCursor(plane1.y , plane1.x);
				print(" ");
				  plane1.x = plane1.x + 1;
					crash(plane1);
					map[plane1.y][plane1.x] = 1;
				setCursor(plane1.y , plane1.x);
				write(3);
				}
				break;
			}
			case 3:{ //left
				if(plane1.x >= 1 ){
					map[plane1.y][plane1.x] = 0;
				setCursor(plane1.y , plane1.x);
				print(" ");
				  plane1.x = plane1.x - 1;
					crash(plane1);
					map[plane1.y][plane1.x] = 1;
				setCursor(plane1.y , plane1.x);
				write(3);
				}
				break;
			}
			default: {break;} //direction = -1
		}
	}
	

	double averageSpeed(){
		return speedSum/adc_counter;
	}
	
		////////////////////////////////////////////////////////////////////////////////
	
//	int chance;
//	int counter_001second;
//	int counter_1second;
//	
//	typedef struct enemyy { 
//  int type;
//  int health;
//	int x, y;
//	int timeToBirth;	
//	int isExist;
//} enemy;
//	
//	int enemy_energy[4]= {1, 2, 3, 4};
//	int max_enemy[8] = {4, 6, 8, 10, 12, 14, 16, 18};
//	int numOfCurrentEnemy;
//	enemy enemies[5];
//	int enemies1[8]= {4, 3, 3, 3, 2, 1, 0, 0};
//	int enemies2[8]= {0, 3, 5, 5, 4, 4, 4, 4};
//	int enemies3[8]= {0, 0, 0, 2, 5, 7, 8, 6};
//	int enemies4[8]= {0, 0, 0, 0, 1, 2, 4, 10};
//	int enemies_onStep;
//	int numOfCurrentEnemy1;
//	int numOfCurrentEnemy2;
//	int numOfCurrentEnemy3;
//	int numOfCurrentEnemy4;

//	typedef struct barrierr { 
//  int type;
//  int health;
//	int x, y;
//	int isExist;
//} barrier;	

  int barrier_energy[3]= {1, 2, 3};
	int remain_time_toCraete_barrier[3]= {0, 0, 0};	
	//barrier barriers[20][4];
	int numOfBarrierBeforeStart;
	
	int cx=3;
	int cy=2;
	int pre_cx;
	int pre_cy;
	
	int s_rand=0;
	int random;

	
	int last_lost_chance;
	
	int remain_time_toDisappear;
	int bonus_TP;//type
	int bonus_x;
	int bonus_y;
	
	int ldr;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_SPI1_Init(void);
static void MX_USB_PCD_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM8_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART3_UART_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

//enables one of 4
void enableSegment(int x){
	
			if(x==0){ //7-seg is off
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);}
	
	else if(x==1){ //7-seg first port
		
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);}
		
	else if (x==2){	
		
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3,1);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);}
	
	else if (x==3){

							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 0);}
	
	else {						
		
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
							HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);}
	
}

//enable digits on ic
void print1Digit(int y){
	
	if (y==0){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);	}
	
	else if (y==1){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	}
	
	else if (y==2){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);	}
	
	else if (y==3){	
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	}
	
	else if (y==4){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);	}
	
	else if (y==5){					
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	}			
	
	else if (y==6){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);	}
	
  else if (y==7){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	}
	
  else if (y==8){
		
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);   }
	
	else if (y==9){
						
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0);	
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);	}

}

//shows stages and speed/ blinking at the first of starting the program
void print4digit(int y, int x){
	
	int x1= x%10;
	int x2= ((x - x1)/10)%10;
	int x3= ((x-(x2*10)-x1)/100)%10;
	
	int x4= y;
	
	enableSegment(4);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 0);
	print1Digit(x4);
	HAL_Delay(1);
	
	enableSegment(3);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
	print1Digit(x3);
	HAL_Delay(1);

	enableSegment(2);
	print1Digit(x2);
	HAL_Delay(1);
	
	enableSegment(1);
	print1Digit(x1);
	HAL_Delay(1);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_PIN){
		
//	if(GPIO_PIN== GPIO_PIN_11 && mode==0){
//		if(option>0)option--;//z add
//	}
	
	if(GPIO_PIN== GPIO_PIN_11){
		if(mode==0){if(option>0)option--;}//z add
		else if(mode==2) {//z add
			    mode=7;
			    clear();
					setCursor(3, 1); 
					print("Enter your name:"); 	
		}
	}
	//z add
	else if(GPIO_PIN== GPIO_PIN_12 ){
			if(mode==0){if(option<2)option++;}
			else if(mode==7){
				/////*related to uart*//////
				needClear=1;
				unsigned char data[100]="hi";
				char plane1X[10];
				sprintf(plane1X,"%d",plane1.x);
//				strncat(data,&plane1X,10);
				char plane1Y[10];
				sprintf(plane1Y,"%d",plane1.y);
				char plane1Fuel[10];
				sprintf(plane1Fuel,"%d",plane1.fuel);
				char plane1Speed[10];
				sprintf(plane1Speed,"%d",plane1.planeSpeed);
				char gameStep[5];
				sprintf(gameStep,"%d",step);
				char gameScore[5];
				sprintf(gameScore,"%d",score);
				sprintf(data," %s %s %s %s %s %s",plane1X,plane1Y,plane1Fuel,plane1Speed,gameStep,gameScore);
				//strncat(idstr, &idchar, 1);
				HAL_UART_Transmit(&huart3,data,sizeof data,1000);
				mode=2;
			}
     
	}
	//z add
	else if(GPIO_PIN== GPIO_PIN_0){
		if(mode==0){
			///for going to new game//z add
			if(option==0){ needClear=1; GameInit();}
			///for going to load game//z add
			else if(option==1){needClear=1;mode=6;/*here I shoud add sth related to uart*/}
			///for going to about//z add
			else if(option==2){needClear=1;mode=3;}
		}
		else if(mode==-1){needClear=1; mode=0;}
	}
	//z add
	else if(GPIO_PIN== GPIO_PIN_6){
		if(mode==7){
		  idchar='1';
		  setCursor(idcurser,3);
			 strncat(idstr, &idchar, 1); 
		   print(idstr);
			 
	}
		else if(mode==2){direction=0;moveAP(direction);direction=-1;}
	}
	else if(GPIO_PIN== GPIO_PIN_7){
		if(mode==7){
		   idchar='2';
		   setCursor(idcurser,3);
			 strncat(idstr, &idchar, 1); 
		   print(idstr);
	}
		else if(mode==2){direction=1;moveAP(direction);direction=-1;}
}
	else if(GPIO_PIN== GPIO_PIN_8){
		if(mode==7){
		  idchar='3';
		  setCursor(idcurser,3);
			 strncat(idstr, &idchar, 1); 
		   print(idstr);
			 
	}
		else if(mode==2){direction=2;moveAP(direction);direction=-1;}
	}
	else if(GPIO_PIN== GPIO_PIN_9){
		if(mode==7 ){
		  idchar='4';
		  setCursor(idcurser,3);
			strncat(idstr, &idchar, 1); 
		  print(idstr);			 
	}
		else if(mode==2){direction=3;moveAP(direction);direction=-1;}
	
}

} 


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	if(hadc->Instance==ADC1){
	  int volume =HAL_ADC_GetValue(&hadc1);
		speed = calculateSpeed(volume , step);
		speedSum += speed;
		adc_counter++;
		time = calculateTime(speed);
//		char strvol[20];
//		setCursor(2,2);
//		print("volume");
//		//if we had if block
//	  setCursor(10,2);
//	  sprintf(strvol,"%3d",(speed/10));
//	  print(strvol);	
	HAL_ADC_Start_IT(&hadc1);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef*huart){
//	if(huart->Instance==USART3){
//			HAL_UART_Receive_IT(&huart3,str,sizeof(str));
//}
}


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim->Instance ==TIM3){		
				s_rand++;
			  if(mode==0){
				enableSegment(0);
				if(needClear==1){
				clear();
				needClear=0;}
				
				setCursor(5, 0);
				print("New  Game"); 
				setCursor(5, 1);
				print("Load Game");
				setCursor(5, 2);
				print("About");
       				
					if(option==0)
					{ setCursor(4, 1); print(" ");
						setCursor(4, 2); print(" ");
						setCursor(4, 0); write(5);}
					else if(option==1)
					{setCursor( 4, 0); print(" ");
						setCursor(4, 2); print(" ");
						setCursor(4, 1); write(5);}
					else
					{setCursor(4, 0); print(" ");
						setCursor(4, 1); print(" ");
						setCursor(4, 2); write(5);}
			}
			  else if(mode==1){
					int f=0;
				 //if(needClear==1){clear(); needClear=0;}/*z add*/
					srand(s_rand);
				  for(int i=0;i<barriers1[step]+barriers2[step];i++){
						 
						//srand(s_rand*200);
						int birthTime=rand()%20;
						srand(s_rand);
					  int x=rand()%4;
						srand(s_rand);
					 if(i<barriers1[step]) {
					   brr[step][i].type=1;
						 brr[step][i].x=x;
						 brr[step][i].y=19;
						 brr[step][i].timeToBirth=birthTime;
					 }
					 else {
						 brr[step][i].type=2;
						 brr[step][i].x=x;
						 brr[step][i].y=19;
						 brr[step][i].timeToBirth=birthTime;
					 }		
						f++;
				 }
					
//				 for(int i=0;i<source_fuel_num[step];i++){
//					 //mesle barr ye doone arraye
//				 }
				 if(f>7)
					
				 {mode=2;}
				 counterMode2=0;
				 
				//HAL_ADC_Start_IT(&hadc1);
  		
				
				}
				  else if(mode==2){
					if(needClear==1){clear(); needClear=0;}/*z add*/
					if(counterMode2%(50*length)==0 && counterMode2!=0){
						  score += averageSpeed()*(step+1);
							speedSum = 0;
							adc_counter = 0;
							step++;
						 if (step==6) {mode=5; needClear=1;}
						 else{mode=1;}
						 if(plane1.planeSpeed ==0 && plane1.fuel == 0){
						 
						 }
					}
					counterMode2++;
					time_counter++;
					if(counterMode2%50==0){
					 for(int i=0;i<barriers1[step]+barriers2[step];i++){
						 if( brr[step][i].timeToBirth== counterMode2/50 ){
							 
								//new_barrier(brr[step][i]);
							      setCursor(brr[step][i].y,brr[step][i].x);
										write(brr[step][i].type);
										map[brr[step][i].y][brr[step][i].x] = -1*(brr[step][i].type);
							      //move(brr[step][i]);
						 }
				    }
					 for(int i=0;i<barriers1[step]+barriers2[step];i++){
						 //if(((time_counter*20) - (brr[step][i].timeToBirth * 1000))%time){
						 if(((time_counter*20) - (brr[step][i].timeToBirth * 1000))%time){
							 setCursor(brr[step][i].y,brr[step][i].x);
							 print(" ");
							 brr[step][i].y--;
							 
							 setCursor(brr[step][i].y,brr[step][i].x);
							 write(brr[step][i].type);							 
								//move(brr[step][i]);
						 }
					 }
					 if(plane1.accident==1){
						 speed/= 2;
						 //plane1.planeSpeed
					 }
					 if(fuel.isExist == 1) {
						if((fuel.timeTobBirth - counterMode2 )%100 == 0){
							fuel.isExist =0;
						}
					 }
					 if(fuel.isExist == 0 ){
					   if(source_fuel_num[step] >= 1){
						   int r = rand()%10;
							 if(r<(8-step)){
							   fuel.y = rand()%19;
								 fuel.x = rand() % 4;
								 fuel.isExist = 1;
								 fuel.timeTobBirth = counterMode2;
								 source_fuel_num[step] --;
							 }
						 }
					 }					 
					}	
				}
				
			  else if(mode==3){
				if(needClear==1){
			clear(); needClear=0;}
				setCursor(0, 1);
				print("Asiye Bahrami");
				setCursor(0, 2);
				print("Zahra Bazargani");
			}
			
			  
			  else if(mode== -1){ 
				 setCursor(2, 1); print("Alien Fighter"); //HAL_Delay(10); 
//				  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 5000);
//					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, 5000);
//					__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, 5000);	
//					__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 5000);
//					__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_2, 5000);
//					__HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_3, 5000);
				HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
					HAL_Delay(50);
				 print4digit(8, 888);
				 enableSegment(0);
				 setCursor(2,1); print("                 ");// HAL_Delay(10);

//				  __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
//					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
//					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
//					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0);
//					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0);
//					__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0);
				HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
				
					HAL_Delay(50);
				 print4digit(8, 888);
				 enableSegment(0);
			 }
				else if(mode==4){ //game over
					if(needClear==1){clear();}
					setCursor(5, 1); print("GAME OVER :(("); //HAL_Delay(10);
					char s[20]="";
					sprintf(s, "%d ", score);
					setCursor(5,2);
					print("score: ");
					print(s);					
					setCursor(5,1);
					print("                 ");
					HAL_Delay(50);
					
				}
				else if(mode==5){ //game over
					if(needClear==1){clear();}
					setCursor(5, 1); print("YOU WON! *_*"); 
					char s[20]="";
					sprintf(s, "%d ", score);
					setCursor(5,2);
					print("score: ");
					print(s);	
					setCursor(5,1);
          print("            ");
				  HAL_Delay(50);

				}	
					
				else if(mode==7){ //save 
					// flagID=1;
				}

}
	if(htim->Instance ==TIM2){
//		HAL_ADC_Start_IT(&hadc1);
		
//		print4digit(1,111);
	}

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_SPI1_Init();
  MX_USB_PCD_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM8_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);

	LiquidCrystal(GPIOD, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14);
	begin(20,4);
	
	
	
	createChar(1, barrier11);
	createChar(2, barrier22);
	createChar(3, airplanes);
	createChar(4, fuel_source);
	createChar(5, selected);
//	createChar(6,trapeze);
//	createChar(7,filled);
//	createChar(8,down_square);
//	createChar(9,up_square);
	
	
	mode=0;
	option=0;
//	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
//HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
//HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
//HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_3); 
//HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_2);
//HAL_TIM_PWM_Start(&htim8,TIM_CHANNEL_1);

  HAL_ADC_Start_IT(&hadc1);
	HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_Base_Start_IT(&htim2);
	
	HAL_TIM_Base_Start_IT(&htim1);
	HAL_TIM_Base_Start_IT(&htim8);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_USART3
                              |RCC_PERIPHCLK_I2C1|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_TIM8|RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  PeriphClkInit.Tim8ClockSelection = RCC_TIM8CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_MultiModeTypeDef multimode;
  ADC_ChannelConfTypeDef sConfig;

    /**Common config 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_6B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the ADC multi-mode 
    */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_601CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* I2C1 init function */
static void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x2000090E;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Analogue filter 
    */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure Digital filter 
    */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* SPI1 init function */
static void MX_SPI1_Init(void)
{

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_4BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 72;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 99;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 60;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 600;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 1200;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1200;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM8 init function */
static void MX_TIM8_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 72;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 99;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
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
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim8);

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* USB init function */
static void MX_USB_PCD_Init(void)
{

  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.ep0_mps = DEP0CTL_MPS_64;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin 
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin 
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin 
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin 
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PC6 PC7 PC8 PC9 
                           PC11 PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9 
                          |GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PD1 PD2 PD3 PD4 
                           PD5 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4 
                          |GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  file: The file name as string.
  * @param  line: The line in file as a number.
  * @retval None
  */
void _Error_Handler(char *file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1)
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
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
