#include <ch.h>
#include <hal.h>

#define GPIOD_LED_ORANGE GPIOD_LED3 
#define GPIOD_LED_RED    GPIOD_LED4 
#define GPIOD_LED_BLUE   GPIOD_LED5 
#define GPIOD_LED_GREEN  GPIOD_LED6 

#define THD_PRIO NORMALPRIO
#define STACK_SIZE (32)
static WORKING_AREA(orangeStack, STACK_SIZE);
static WORKING_AREA(redStack, STACK_SIZE);
static WORKING_AREA(blueStack, STACK_SIZE);
static WORKING_AREA(greenStack, STACK_SIZE);
Thread *orangeTP, *redTP, *blueTP, *greenTP;

static msg_t ledThd(void *arg);

typedef struct {
	GPIO_TypeDef *ledPort;
	int ledPin;
} LedCfg;

int main(void)
{
	halInit();
	chSysInit();

	{
		LedCfg cfg = {GPIOD, GPIOD_LED_ORANGE};
		orangeTP = chThdCreateStatic(orangeStack, sizeof(orangeStack), THD_PRIO, ledThd, &cfg);
	}
	{
		LedCfg cfg = {GPIOD, GPIOD_LED_RED};
		redTP = chThdCreateStatic(redStack, sizeof(redStack), THD_PRIO, ledThd, &cfg);
	}
	{
		LedCfg cfg = {GPIOD, GPIOD_LED_BLUE};
		blueTP = chThdCreateStatic(blueStack, sizeof(blueStack), THD_PRIO, ledThd, &cfg);
	}
	{
		LedCfg cfg = {GPIOD, GPIOD_LED_GREEN};
		greenTP = chThdCreateStatic(greenStack, sizeof(greenStack), THD_PRIO, ledThd, &cfg);
	}

	chThdExit(1);

	return 0;
}

static msg_t ledThd(void *arg)
{
	LedCfg *cfg = (LedCfg*)arg;
	palSetPadMode(cfg->ledPort, cfg->ledPin, PAL_MODE_OUTPUT_PUSHPULL);

	static int d = 0;
	while (1) {
		palTogglePad(cfg->ledPort, cfg->ledPin);
		chThdSleep(d+=d++);
		d %= 250;
	}

	return 0;
}

