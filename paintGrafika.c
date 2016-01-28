    #include <stdlib.h>
    #include <stdint.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include <linux/fb.h>
    #include <sys/mman.h>
    #include <sys/ioctl.h>
    #include "font8x8_basic.h"
     
    typedef struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } Color32;
     
    int fbfd = 0;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    long int screensize = 0;
    char *fbp = 0, *bbp = 0;
    int x = 0, y = 0;
    long int location = 0;
     
    void setPixel(int x, int y, Color32 color){
        location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) + (y+vinfo.yoffset) * finfo.line_length;
        if(location >= 0 && location < screensize){
            *((uint32_t*)(fbp + location)) = (color.r<<vinfo.red.offset) | (color.g<<vinfo.green.offset) | (color.b<<vinfo.blue.offset) | (color.a<<vinfo.transp.offset);
        } else {
            //Out of bound
        }
    }
     
    void drawChar(char c, int x, int y, int scale, Color32 color){
        int i,j,a,b;
        int set;
        for (j=0; j < 8; j++) {
            for (i=0; i < 8; i++) {
                set = font8x8_basic[(int)c][j] & 1 << i;
                if(set){
                    for(b=0; b<scale; b++){
                        for(a=0; a<scale; a++){
                            setPixel(x+i*scale+a, y+j*scale+b, color);
                        }
                    }
                }
            }
        }
    }
     
    void drawString(char* str, int x, int y, int scale, Color32 color){
        int i = 0;
        while(*str){
            char c = *str++;
            drawChar(c, x+(i*8*scale), y, scale, color);
            i++;
        }
    }
     
    void drawRectangle(int startX, int startY, int width, int height, int red, int green, int blue){
        for (y = startY; y < startY + height; y++){
            for (x = startX; x < startX + width; x++) {
                location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
                           (y+vinfo.yoffset) * finfo.line_length;
     
                if (vinfo.bits_per_pixel == 32) {
                    *(fbp + location) = blue;        // Some blue
                    *(fbp + location + 1) = green;     // A little green
                    *(fbp + location + 2) = red;    // A lot of red
                    *(fbp + location + 3) = 0;      // No transparency
            //location += 4;
                } else  { //assume 16bpp
                    int b = blue;
                    int g = green;     // A little green
                    int r = red;    // A lot of red
                    unsigned short int t = r<<11 | g << 5 | b;
                    *((unsigned short int*)(fbp + location)) = t;
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
     
        // Figure out where in memory to put the pixel
        // void drawRectangle(int startX, int startY, int width, int height, int red, int green, int blue)
     
        // Clear Screen
        drawRectangle(0, 0, vinfo.xres, vinfo.yres, 0, 0, 0);
     
        int i = 0;
        int j = 0;
     
        char* names[6] = {"Grafika", "Vincent Theophilus Ciputra", "Gerry Kastogi", "David", "Vincent Sebastian The", "Randi Chilyon Alfianto"};
        int temp = 3;
     
        for (i=vinfo.yres-1;i>=0;i--){
            for (j=0;j<6;j++){
    		drawString(names[j], 200, i + 30*j, temp, (Color32){255*i/vinfo.yres,255-(255*i/vinfo.yres),255,255});
            }
     
    	usleep(10000);
     
    	for (j=0;j<6;j++){
    		drawString(names[j], 200, i + 30*j, temp, (Color32){0,0,0,0});
            }
        }
     
        munmap(fbp, screensize);
        close(fbfd);
        return 0;
    }