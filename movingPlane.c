#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

int fbfd = 0;
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;
long int screensize = 0;
char *fbp = 0;
int x = 0, y = 0;
long int location = 0;

int start, end;
int left, right;

int ymin = 100;
int ymax = 150;

int yminshooter = 550;
int ymaxshooter = 600;
int startshooter = 100;
int endshooter = 150;

pthread_t tid;

void printPlane(int start, int end){
    for (y = ymin; y < ymin + 30; y++){
        for (x = start; x < end; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 255;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
}

void printShooter(int startshooter, int endshooter){
    for (y = yminshooter; y < ymaxshooter; y++){
        for (x = startshooter; x < endshooter; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 255;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
}

void clearScreen(){
    for (y = 0; y < 600; y++){
        for (x = 0; x < 1000; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 0;
            *(fbp + location + 1) = 0;
            *(fbp + location + 2) = 0;
            *(fbp + location + 3) = 0;
        }
    }
}

void* animatePlane(void* arg) {
	while (ymin != -20000) {
        printShooter(startshooter,endshooter);
        printPlane(start,end);

        usleep(15000);
        clearScreen();
        if (right == 1 ){
            start += 3;
            end += 3;
            if (end == 600){
                right = 0;
                left = 1;
            }
        } else if (left == 1){
            start -= 3;
            end -= 3;
            if (start == 100){
                right = 1;
                left = 0;
            }
        }
    }
}

int main()
{
    // Open the file for reading and writing
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error: cannot open framebuffer device");
        exit(1);
    }
    printf("The framebuffer device was opened successfully.\n");

    // Get fixed screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    // Get variable screen information
    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

    // Figure out the size of the screen in bytes
    screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

    // Map the device to memory
    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((int)fbp == -1) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
    printf("The framebuffer device was mapped to memory successfully.\n");

    right = 1;
    left = 0;
    start = 100;
    end = start + 50;

	pthread_create(&tid, NULL, &animatePlane, NULL);
//	anim();
	char c;
	scanf("%c", &c);
	printf("asdasd");

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
