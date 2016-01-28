#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
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

int distance, start, end;

int ymin = 300;
int ymax = 400;

void printV(int start, int end, int distance){
    //// Huruf V

    for (y = ymin; y < ymax; y++){
        if (y % 2 == 0)
            start++;
        for (x = start; x < end; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            if ((x < start + distance) || (x > end - distance)) {
                *(fbp + location) = 200;        // Some blue
                *(fbp + location + 1) = 15+(x-100)/2;     // A little green
                *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
        if (y % 2 == 0)
            end--;
    }
}

void printI(int start, int end, int distance){
    // Huruf I

    for (y = ymin; y < ymax; y++){
        for (x = start; x < end; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
}

void printN(int start, int end, int distance){
    //// Huruf N

    for (y = ymin; y < ymax; y++){
        for (x = start; x < start+20; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
    for (y = ymin; y < ymax; y++){
        if (y % 2 == 0)
            start++;
        for (x = start; x < end; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            if ((x < start + distance)) {
                *(fbp + location) = 200;        // Some blue
                *(fbp + location + 1) = 15+(x-100)/2;     // A little green
                *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
    }
    for (y = ymin; y < ymax; y++){
        for (x = end-40; x < end-20; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
}

void printC(int start, int end, int distance){
    //// Huruf C
    for (y = ymin; y < ymax; y++){
        for (x = start; x < start+20; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
    for (y = ymin; y < ymax; y++){
        for (x = start; x < end + 50; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            if (y < ymin+20 || y > ymax-20) {
                *(fbp + location) = 200;        // Some blue
                *(fbp + location + 1) = 15+(x-100)/2;     // A little green
                *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
    }
}

void printE(int start, int end, int distance){
    //// Huruf E
    for (y = ymin; y < ymax; y++){
        for (x = start; x < start+20; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
    for (y = ymin; y < ymax; y++){
        for (x = start; x < end + 50; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;

            if ((y < ymin+20) || (y > ymax-20) || (y > ymin+40 && y < ymax-40)) {
                *(fbp + location) = 200;        // Some blue
                *(fbp + location + 1) = 15+(x-100)/2;     // A little green
                *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
                *(fbp + location + 3) = 0;      // No transparency
            }
        }
    }
}

void printT(int start, int end, int distance){
    //// Huruf T
    for (y = ymin; y < ymin + 20; y++){
        for (x = start; x < end; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
    for (y = ymin; y < ymax; y++){
        for (x = start+40; x < end-40; x++) {

            location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                       (y+vinfo.yoffset) * finfo.line_length;
            *(fbp + location) = 200;        // Some blue
            *(fbp + location + 1) = 15+(x-100)/2;     // A little green
            *(fbp + location + 2) = 200-(y-100)/5;    // A lot of red
            *(fbp + location + 3) = 0;      // No transparency
        }
    }
}

void clearScreen(int ymin){
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

    

    while (ymin != -20000){
        start = 100;
        end = start + 100;
        distance = 20;
    
        printV(start, end, distance);

        start += 110;
        end = start + 20;
        printI(start, end, distance);

        start += 30;
        end = start + 100;
        printN(start, end, distance);

        start += 100;
        end = start + 30;
        printC(start, end, distance);

        start += 100;
        end = start + 30;
        printE(start, end, distance);

        start += 100;
        end = start + 100;
        printN(start, end, distance);

        start += 100;
        end = start + 100;
        printT(start, end, distance);

        usleep(10000);
        clearScreen(ymin);
        ymin -= 1;
        ymax -= 1;
    }

    munmap(fbp, screensize);
    close(fbfd);
    return 0;
}