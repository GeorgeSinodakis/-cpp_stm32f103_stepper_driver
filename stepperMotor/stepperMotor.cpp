#include "stepperMotor.h"
#include "gpio.h"
#include "nvic.h"

stepperMotor::stepperMotor()
{
    position = 0;
    accSTEPS = regSTEPS = decSTEPS = 0;
}

void stepperMotor::moveToPosition(s32 newPosition)
{
    while (!motorStopped());

    if(newPosition > position)
    {
        direction = clockwise;
        moveSteps(newPosition - position);
    }
    if(newPosition < position)
    {
        direction = counterclockwise;
        moveSteps(position - newPosition);
    }
    if(newPosition == position)
    {
        return;
    }
}

void stepperMotor::setDirPin()
{
    if (direction > 0)
    {
        GPIOB_clear(dirPin);
    }
    else
    {
        GPIOB_set(dirPin);
    }
}

void stepperMotor::calculateSteps(u32 steps)
{
    if (steps <= SPSchangingsteps * 2)
    {
        accSTEPS = steps / 2;
        decSTEPS = steps / 2;
        regSTEPS = steps % 2;
    }
    else
    {
        accSTEPS = SPSchangingsteps;
        decSTEPS = SPSchangingsteps;
        regSTEPS = steps - (SPSchangingsteps * 2);
    }
}

void stepperMotor::moveSteps(u32 steps)
{
    setDirPin();

    calculateSteps(steps);

    sps = SPSmin;
    NVIC_ISER0 |= 1<<28;
}

bool stepperMotor::makeStep()
{
    if(accSTEPS) 
    {
		GPIOB_set(stepPin);
		position += direction;
        sps += SPSPS;
        accSTEPS--;
        GPIOB_clear(stepPin);
    }
    else if (regSTEPS)
    {
		GPIOB_set(stepPin);
		position += direction;
		regSTEPS--;
        GPIOB_clear(stepPin);
    }
    else if(decSTEPS)
    {
		GPIOB_set(stepPin);
		position += direction;
        sps -= SPSPS;
        decSTEPS--;
        GPIOB_clear(stepPin);
    }
    else 
    {
        return false;
    }
	return true;
}

bool stepperMotor::motorStopped()
{
    return !(accSTEPS || regSTEPS || decSTEPS);
}