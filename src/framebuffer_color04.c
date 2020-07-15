#include <stdio.h>
#include <stdlib.h>    
#include <string.h>    
#include <assert.h>    

#include <getopt.h>             /* getopt_long() */    

#include <fcntl.h>              /* low-level i/o */    
#include <unistd.h>             /*getpid()*/
#include <error.h>    
#include <errno.h>
#include <malloc.h>    
#include <sys/stat.h>    
#include <sys/types.h>    
#include <sys/time.h>    
#include <sys/mman.h>    
#include <sys/ioctl.h>
#include <linux/fb.h> 
#include "ASC16.h" 
#include "HZK16.h"
#include "ASC32.h" 
#include "HZK32.h"
#include "img_14.h"
unsigned int * lcd_buffer = NULL;
__u8 *fb_buf;
__u32 screensize;
void  draw8x16 (int x,int y,unsigned char ch[], int color);  
void  draw16x32 (int x,int y,unsigned char ch[], int color);

void  draw16x16 (int x,int y,unsigned char ch[], int color);
void  draw32x32 (int x,int y,unsigned char ch[], int color);

void  show(int x, int y,unsigned char str[], int len, unsigned char flag,int color);

void  multi_screen();

void  showImage(unsigned char image[],int posx, int posy,int width,int height);

int main(int argc,char *argv[])
{
   int i,fd,fbfd,txfd;
   struct fb_var_screeninfo vinfo;
   struct fb_fix_screeninfo finfo;
   
   int fb_xres,fb_yres,fb_bpp;//bits per pixel
   
   unsigned char data[60];
   int  len;
   int  n;
   

   fbfd=open("/dev/fb0",O_RDWR);
   if(fbfd<0)
   {
     printf("Error:cannot open framebuffer device!\n");
	 close(txfd);
     return -1;
   }
   //get fb_fix_screeninfo
   if(ioctl(fbfd,FBIOGET_FSCREENINFO,&finfo))
   {
     printf("Error reading fixed information!\n");
     return -1;
   }
  //get fb_var_screeninfo
   if(ioctl(fbfd,FBIOGET_VSCREENINFO,&vinfo))
   {
     printf("Error reading variable information!\n");
     return -1;
   }
  printf("%dx,%dy,%dbpp\n",vinfo.xres,vinfo.yres,vinfo.bits_per_pixel);
  fb_xres=vinfo.xres;
  fb_yres=vinfo.yres;
  fb_bpp=vinfo.bits_per_pixel;
  
   screensize=vinfo.xres*vinfo.yres*vinfo.bits_per_pixel/8;
   fb_buf=(char *)mmap(0,screensize,PROT_READ|PROT_WRITE,MAP_SHARED,fbfd,0);
   if((int)(fb_buf)==-1)
   {
      printf("Error:failed to map framebuffer device to memory!\n");
     // close(fbfd);
      return -1;
   }  
  memset(fb_buf,0,screensize); 
    
  lcd_buffer = (unsigned int *)fb_buf;
  
  /*
         showImage(image0,0,0,240,136);
	 showImage(image0,240,0,240,136);
	 showImage(image0,0,136,240,136);
	 showImage(image0,240,136,240,136);
  */

  //multi_screen();
 
  printf("ummap framebuffer device to memory!\n");
  
  munmap(fb_buf,screensize);
  close(fbfd);
  
  return 0;
 }  
   
void  draw8x16 (int x,int y,unsigned char ch[], int color)
{
	int i,j,index =0;
	for (i=0;i <16;i++,index++)
	{
		for (j=00;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+j)] = 0x00;
			}
			
		}
	}	
}
void  draw16x32 (int x,int y,unsigned char ch[], int color){
	int i,j,index =0;
	for (i=0;i <32;i++)
	{
		for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+j)] = 0x00;
			}
			
		}
	   index++;
	   for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+8+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+8+j)] = 0x00;
			}
			
		}
	  index++;	
	}	
}
void  draw16x16 (int x,int y,unsigned char ch[], int color)
{
	int i,j,index =0;
	for (i=0;i <16;i++)
	{
		for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+j)] = 0x00;
			}
			
		}
	   index++;
	   for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+8+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+8+j)] = 0x00;
			}
			
		}
	  index++;	
	}	
}

void  draw32x32 (int x,int y,unsigned char ch[], int color)
{
	int i,j,index =0;
	for (i=0;i <32;i++)
	{
		for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+j)] = 0x00;
			}
			
		}
	   index++;
	   for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+8+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+8+j)] = 0x00;
			}
			
		}
	  index++;	  
	  for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+16+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+16+j)] = 0x00;
			}
			
		}
	  index++;
      for (j=0;j<8;j++)
		{
			if (ch[index] & (1<<(7-j)))
			{
				lcd_buffer[(y+i)*480+(x+24+j)] =  color;
			}else{
				lcd_buffer[(y+i)*480+(x+24+j)] = 0x00;
			}
			
		}
	  index++;		  
	}	
	
}

void  show(int x, int y,unsigned char str[], int len, unsigned char flag,int color)
{
	long index1;
	int n;
	for (n=0;n<len;)
  { 
   if(str[n]>160)
    {
	   index1 = (str[n]-161)*94+(str[n+1]-161);
	   if(flag)
	   {
		   index1 *=128;
		   draw32x32(x+n*16,y,&szHZK32[index1],color);
	   }else{
		   index1 *=32;
		   draw16x16(x+n*8,y,&szHZK16[index1],color);
	   }
	   n+=2;
    }else{
		index1 = str[n];
		if (flag)
		{
			index1 *= 64;
			draw16x32(x+n*16,y,&szASC32[index1],color);		
		}else{
			index1 *= 16;
			draw8x16(x+n*8,y,&szASC16[index1],color);
		}
		n++;
	}
  	
  }  
	
}
void  multi_screen()
{
  int i,j;
  for (i=0;i< 136;i++)
  {
	 for (j=0;j<240;j++) 
	 {
		lcd_buffer[i*480+j] = 0xff0000;
	 }
	 for(j=240;j<480;j++)
	 {
		lcd_buffer[i*480+j] = 0x00ff00; 
	 }
	  
  }
  
  for (i=136;i< 272;i++)
  {
	 for (j=0;j<240;j++) 
	 {
	   lcd_buffer[i*480+j] = 0x0000ff; 
	 }
	 for(j=240;j<480;j++)
	 {
		lcd_buffer[i*480+j] = 0xff00ff; 
	 }
	  
  }
	
}
void  showImage(unsigned char image[],int x, int y,int width,int height)
{
  int i,j,index =0;
  for (i=0;i <height;i++)
	{
		for (j=0;j<width;j++)
		{		
	    lcd_buffer[(y+i)*480+(x+j)] =  image[index *3]|(image[index *3 +1] << 8)|(image[index *3 +2] << 16);
		index++;			
		}
	}
	
}
