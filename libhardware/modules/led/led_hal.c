
/*1.实现一个名为HMI的hw_module_t结构体*/

/*2.实现一个open函数，它返回led_device_t结构体*/

/*3.实现led_device_t结构体*/


#include <hardware/led_hal.h>
#include <hardware/hardware.h>

#include <cutils/log.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <utils/Log.h>

#define LOG_TAG "LedHal"

static int fd1;
static int fd2;
static int fd3;
static int fd4;

char const *const LED1_FILE = "/sys/class/leds/led1/brightness";
char const *const LED2_FILE = "/sys/class/leds/led2/brightness";
char const *const LED3_FILE = "/sys/class/leds/led3/brightness";
char const *const LED4_FILE = "/sys/class/leds/led4/brightness";

static int led_open(struct led_device_t* dev)
{
	
	fd1 = open(LED1_FILE,O_RDWR);
	if(fd1 < 0){
		ALOGI("led open led1 errno!");
		return -1;
	}
	fd2 = open(LED2_FILE,O_RDWR);
	if(fd2 < 0){
		ALOGI("led open led2 errno!");
		return -1;
	}
	fd3 = open(LED3_FILE,O_RDWR);
	if(fd3 < 0){
		ALOGI("led open led3 errno!");
		return -1;
	}
	fd4 = open(LED4_FILE,O_RDWR);
	if(fd4 < 0){
		ALOGI("led open led4 errno!");
		return -1;
	}
	ALOGI("led open success!");
	return 0;
}
static int led_ctrl(struct led_device_t* dev, int which, int status)
{
	int fd = 0;
	int ret = 0;
	char buf[1] = {0};

	if(status){
		buf[0] = '1';
	}else{
		buf[0] = '0';
	}
	switch (which) {
		case 1:
			fd = fd1;
			break;
		case 2:
			fd = fd2;
			break;
		case 3:
			fd = fd3;
			break;
		case 4:
			fd = fd4;
			break;
		default:
			ALOGI("led ctrl led:%d status:%d is invaild",which,status);
			return -1;
	}
	ret = write(fd,buf,sizeof(buf));
	if(ret < 0){
		ALOGI("led ctrl led:%d status:%d write fail!!!",which,status);
	}
	ALOGI("led ctrl led:%d status:%d",which,status);

	return 0;
}

static int led_close(struct hw_device_t* device)
{
	close(fd1);
	close(fd2);
	close(fd3);
	close(fd4);
	ALOGI("led close ...");

	return 0;
}


static struct led_device_t led_dev = {
	.common = {
		.tag   = HARDWARE_DEVICE_TAG,
		.close = led_close,
	},
	.led_open = led_open,
	.led_ctrl = led_ctrl,
};


static int led_device_open(const struct hw_module_t* module, const char* id,
            struct hw_device_t** device)
{
	*device = &led_dev;
	return 0;
}



static struct hw_module_methods_t led_module_methods = {
    .open = led_device_open,
};

struct hw_module_t HAL_MODULE_INFO_SYM = {
	.tag = HARDWARE_MODULE_TAG,
    .id = "led",
    .methods = &led_module_methods,
};

