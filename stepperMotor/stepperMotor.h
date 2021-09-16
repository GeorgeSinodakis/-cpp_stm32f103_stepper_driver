#include <stdint.h>

#define dirPin				9
#define stepPin				8
#define button1Pin			1
#define button0Pin			0
#define TIM2clock			12000000UL

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

class stepperMotor
{
public:
	volatile s8 direction;
	volatile s32 position;
	volatile u16 sps, accSTEPS, decSTEPS;
	volatile u32 regSTEPS;

	const u16 SPSmin =	185;							//min steps per second
	const u16 SPSmax = 10000;							//max steps per second
	const u16 SPSPS = 1;									//steps per second per step(acceleration)
	const u16 SPSchangingsteps	= (SPSmax-SPSmin)/SPSPS;	//how many times the speed will change
	const u16 Spr = 3200;								//steps per revolution 
	const s8 clockwise = 1, counterclockwise = -1;
public:
	stepperMotor();
	void moveToPosition(s32 newPosition);
	bool makeStep();
	bool motorStopped();
private:
	void setDirPin();
	void calculateSteps(u32 steps);
	void moveSteps(u32 steps);	
};