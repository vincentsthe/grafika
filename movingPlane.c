#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include <stdio.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

long bulletSpeed = 8;

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
int startturret = 100;
int endturret = 108;

int lx1, lx2, ly1, ly2;
int ix1, ix2, iy1, iy2;
int drawLine;

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

void printTurret(int startturret, int endturret){
    for (y = ymin + 430; y < ymin + 438; y++){
        for (x = startturret; x < endturret; x++) {

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

void drawLineFunc(int yi, int xi, int yn, int xn) {
	int dx = abs(xn - xi);
	int sx = (xi < xn) ? 1 : -1;
	int dy = abs(yn - yi);
	int sy = (yi < yn) ? 1 : -1;
	int err = (dx > dy ? dx : -dy)/2;
	int e2;
	
	while (true) {
		location = (xi+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (yi+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 0;
            *(fbp + location + 1) = 0;
            *(fbp + location + 2) = 255;
            *(fbp + location + 3) = 0;
         
         if ((xi == xn) && (yi == yn)) {
         	break;
         }
         
         e2 = err;
         if (e2 > -dx) {
         	err -= dy;
         	xi += sx;
         }
         
         if (e2 < dy) {
         	err += dx;
         	yi += sy;
         }
	}
}

void* animatePlane(void* arg) {
	while (ymin != -20000) {
        printShooter(startshooter,endshooter);
        printTurret(startturret,endturret);
        printPlane(start,end);
        //printf("%d %d\n", startturret, endturret);
        
        int dx = abs(ix2 - ix1);
		int sx = (ix1 < ix2) ? bulletSpeed : -bulletSpeed;
		int dy = abs(iy2 - iy1);
		int sy = (iy1 < iy2) ? bulletSpeed : -bulletSpeed;
		int err = (dx > dy ? dx : -dy)/2;
		int e2;
        if (drawLine) {    	
        	if ((ly1 < 0) || (ly1 > 600) || (lx1 < 0) || (lx1 > 800) || (ly2 < 0) || (ly2 > 600) || (lx2 < 0) || (lx2 > 800)) {
        		drawLine = 0;
        	} else {
	        	drawLineFunc(ly1, lx1, ly2, lx2);
        	}
        	
        	e2 = err;
        	if (e2 > -dx) {
		     	err -= dy;
		     	lx1 += sx;
		     	lx2 += sx;
		     }
		     
		     if (e2 < dy) {
		     	err += dx;
		     	ly1 += sy;
		     	ly2 += sy;
		     }
        }

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
	drawLine = 0;

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
    
    ly1 = 200;
    ly2 = 300;
    lx1 = 30;
    lx2 = 60;

	pthread_create(&tid, NULL, &animatePlane, NULL);
	char c;
	while (1) {
		scanf("%c", &c);
		if (c == 'a') {
			startturret -= 4;
			endturret -= 4;
		} else if (c == 'd') {
			startturret += 4;
			endturret += 4;
		} else if (c == ' ') {
			ly1 = iy1 = (yminshooter + ymaxshooter) / 2;
			lx1 = ix1 = (startshooter + endshooter) / 2;
			
			ly2 = iy2 = ymin + 434;
			lx2 = ix2 = (startturret + endturret) / 2;			
			drawLine = 1;
		}
	}

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}
