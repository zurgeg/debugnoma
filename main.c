#include <gccore.h>
#include <wiiuse/wpad.h>
#include <ogc/ios.h>
#include <fat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <debug.h>
#include <network.h>
static void *xfb = NULL;
static GXRModeObj *vmode = NULL;
char * url = "";

void setupVideo() {

  VIDEO_Init();
  vmode = VIDEO_GetPreferredMode(NULL);
  VIDEO_Configure(vmode);
  xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(vmode));
  console_init (xfb, 20, 20, vmode->fbWidth, vmode->xfbHeight, vmode->fbWidth*VI_DISPLAY_PIX_SZ);
  VIDEO_Configure(vmode);
  VIDEO_ClearFrameBuffer(vmode, xfb, COLOR_BLACK);
  VIDEO_SetNextFramebuffer(xfb);
  VIDEO_SetBlack(0);
  VIDEO_Flush();
  VIDEO_WaitVSync(); 
  if (vmode->viTVMode & VI_NON_INTERLACE)
    VIDEO_WaitVSync();

}

s32 launchTitle(u64 TitleID, char * url) {
	WII_Initialize();
	if (url[0] != 0) {
		//Load title with specified URL as an argument
		return WII_LaunchTitleWithArgs(TitleID, 0, url, NULL); 
	} else {
			return WII_LaunchTitle(TitleID); 
	}
}

s32 readCfg(char * path) {
	int c = 0;
	int i = 0;
	FILE *f = fopen(path, "r");
	if (f == NULL)
		return 0;
	fseek(f , 0 , SEEK_END);
	u32 size = ftell(f);
	rewind(f);
	url = (char*) malloc(sizeof(char)*size);
	while (c != EOF) {
		c = fgetc(f);
		if (31 < c && c < 127)
			url[i++] = c;
	}
	url[i] = '\0';
	fclose(f);
	return 1;
}

int main(int argc, char **argv) {
	setupVideo();
	fatInitDefault();
	s32 ret;
	printf('Some network init thing idk');
	res = net_init_async(NULL, NULL);
	if (res < 0) {
		printf('Failed to init net!');
	}
	else {
		printf('Net Init Done!');
		DEBUG_Init(100, 5656);
		printf('Debug Init Done!');
	}
	if (argc > 0) {
		char path[MAXPATHLEN];
		int len = strlen(argv[0]);
		int i = 0;
        for(i = len; argv[0][i] != '/'; i--);
	ret = launchTitle(0x000100014843494ALL, 'moment');
	if (ret < 0) {
		printf('Wii no Ma must be installed to debug...');
	}
	return ret;
}
