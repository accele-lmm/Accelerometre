#include "fb_Graphic.h"

unsigned int fb_get_fb_size(fb_sess *fb)
{
	return fb->vinfo.xres * fb->vinfo.yres * (fb->vinfo.bits_per_pixel/8);
}


fb_sess *fb_init(const char *fb_dev_name)
{
	fb_sess *fb;
	int st, size;
	unsigned int i,j;
	
	fb = malloc(sizeof(*fb));
	if(!fb) abort();
	
	fb->fd = open(fb_dev_name, O_RDWR);
	if(fb->fd < 0) {
		free(fb);
		fprintf(stderr, "Cannot open framebuffer device file.\n");
		exit(EXIT_FAILURE);
	}
	
	st = ioctl(fb->fd, FBIOGET_FSCREENINFO, &fb->finfo);
	if(st) {
		perror("ioctl failed");
		close(fb->fd);
		free(fb);
		fprintf(stderr, "Cannot get fixed screen info.\n");
		exit(EXIT_FAILURE);
	}
	
	st = ioctl(fb->fd, FBIOGET_VSCREENINFO, &fb->vinfo);
	if(st) {
		perror("ioctl failed");
		close(fb->fd);
		free(fb);
		fprintf(stderr, "Cannot get variable screen info.\n");
		exit(EXIT_FAILURE);
	}
	
	size = fb_get_fb_size(fb);
#if CONFIG_MMAP
	fb->memp = mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, fb->fd, 0);
	if(fb->memp == MAP_FAILED) {
		perror("mmap failed");
		close(fb->fd);
		free(fb);
		fprintf(stderr, "Cannot mmap the framebuffer device.\n");
		exit(EXIT_FAILURE);
	}
#else
	fb->memp = malloc(size);
	if(!fb->memp) {
		abort();
	}
#endif
	
	/* clear display */
	memset(fb->memp, 0, size);
	
	for(i = 0; i < fb_xres(fb); i++)
		for(j = BARRE_SIZE; j < fb_yres(fb); j++)
			fb_draw_pixel(fb, i, j, BACKGROUND_COLOR);
			
	fb_sync(fb);
	
	return fb;
}

void fb_close(fb_sess *fb)
{
	if(fb) {
#if CONFIG_MMAP
		munmap(fb->memp, fb_get_fb_size(fb));
#else
		free(fb->memp);
#endif
		close(fb->fd);
		free(fb);
	}
}



/*************************************************************************
 * Function Name: fb_draw_pixel
 *
 * Parameters:	fb_sess* : pointeur vers la session fb_Graphic
 *				unsigned int x : abscisse du pixel ˆ dessinner
 *				unsigned int y : ordonnŽe du pixel ˆ dessinner
 *				unsigned int color : couleur du pixel ˆ dessinner
 *
 * Return: void
 *
 * Description: Dessine un pixel
 *************************************************************************/
void fb_draw_pixel(fb_sess *fb, unsigned int x, unsigned int y, unsigned int color)
{
	unsigned int offset;
	
	/* framebuffer is 16 bits per pixel */
	offset = (x + fb->vinfo.xoffset) * (fb->vinfo.bits_per_pixel/8) + (y + fb->vinfo.yoffset) * fb->finfo.line_length;
	
	switch(fb->vinfo.bits_per_pixel) {
		case 32:
		case 24:
			*(fb->memp + offset+2) = (color >> 16) & 0xff;
		case 16:
			*(fb->memp + offset+1) = (color >> 8) & 0xff;
		case 8:
			*(fb->memp + offset) = (color >> 0) & 0xff;
			break;
		default:
			abort();
	}
}

unsigned int fb_xres(fb_sess *fb) {
	return fb->vinfo.xres;
}

unsigned int fb_yres(fb_sess *fb) {
	return fb->vinfo.yres;
}

unsigned int fb_bits_per_pixel(fb_sess *fb) {
	return fb->vinfo.bits_per_pixel;
}


void fb_sync(fb_sess *fb) {
#if CONFIG_MMAP
	msync(fb->memp, fb_get_fb_size(fb), MS_SYNC);
#else
	lseek(fb->fd, 0, SEEK_SET);
	write(fb->fd, fb->memp, fb_get_fb_size(fb));
#endif
}



#define ARRAY_NUMELEM(A)	((sizeof(A)/sizeof((A)[0])))
/*int main(void)
 {
 fb_sess *fb;
 const char *fb_dev_name;
 char buf[256];
 
 fb_dev_name = "/dev/fb0";
 
 printf("Trying to open the framebuffer device...\n");
 
 fb = fb_init(fb_dev_name);
 if(!fb) {
 fprintf(stderr, "Could not initialize %s.\n", fb_dev_name);
 return EXIT_FAILURE;
 }
 
 printf("Getting framebuffer information:\n");
 printf("\tID:            %s\n", fb->finfo.id);
 printf("\tcolordepth:    %d bits/pixel\n", fb_bits_per_pixel(fb));
 printf("\tvideo memsize: %d bytes\n", fb_get_fb_size(fb));
 printf("\tresolution:    %dx%d\n", fb_xres(fb), fb_yres(fb));
 printf("\tvirtual res.:  %dx%d\n", fb->vinfo.xres_virtual,
 fb->vinfo.yres_virtual);
 printf("\tgrayscale:     %d\n", fb->vinfo.grayscale);
 printf("\tphys size:     %dx%d mm\n", fb->vinfo.width,
 fb->vinfo.height);
 printf("\tpixel clock:   %d picoseconds\n", fb->vinfo.pixclock);
 printf("\tleft margin:   %d\n", fb->vinfo.left_margin);
 printf("\tright margin:  %d\n", fb->vinfo.right_margin);
 printf("\tupper margin:  %d\n", fb->vinfo.upper_margin);
 printf("\tlower margin:  %d\n", fb->vinfo.lower_margin);
 printf("\thsync_len:     %d\n", fb->vinfo.hsync_len);
 printf("\tvsync_len:     %d\n", fb->vinfo.vsync_len);
 printf("\trotate:        %d\n", fb->vinfo.rotate);
 printf("\tsmem_len:      %d\n", fb->finfo.smem_len);
 printf("\tline_length:   %d\n", fb->finfo.line_length);
 printf("\tmmio_start:    0x%08x\n", fb->finfo.mmio_start);
 printf("\tmmio_len:      %d\n", fb->finfo.mmio_len);
 printf("\tmmio_len:      %d\n", fb->finfo.mmio_len);
 
 
 draw_lines(fb);
 usleep(1000*1000);
 draw_sine(fb);
 
 fb_close(fb);
 
 printf("Done. Exitting...\n");
 
 return 0;
 }
 */


ball makeBall(int x, int y, int color, int size) {
	
	ball balle;
	
	balle.x = x;
	balle.y = y;
	balle.color = color;
	balle.size = size;
	
	return balle;	
}

void drawBall(fb_sess *fb, ball *balle) {
	
	int i, j, k, l;
	
	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)
			if(mat_balle[i][j] == 1)	
				for(l = 0; l < balle->size; l++)			
					for(k = 0; k < balle->size; k++)
						fb_draw_pixel(fb, balle->x + i * balle->size + l - 4 * balle->size, balle->y + j * balle->size + k - 4 * balle->size, balle->color);
	
	return;
}

void clearBall(fb_sess *fb, ball *balle) {
	
	int i, j, k, l;
	
	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)		
			for(k = 0; k < balle->size; k++)
				for(l = 0; l < balle->size; l++)
					fb_draw_pixel(fb, balle->x + i * balle->size + l - 4 * balle->size, balle->y + j * balle->size + k - 4 * balle->size, BACKGROUND_COLOR);
	
	return;
}


void moveBall(fb_sess *fb, ball *balle, int AccX, int AccY) {
	
	clearBall(fb, balle);
	
	if(AccX > 0)
		balle->x += max(COEFF_X * AccX, -balle->x + 4 * balle->size);
	else
		balle->x += min(COEFF_X * AccX, fb_xres(fb) - balle->x - 5 * balle->size);
	
	if(AccY > 0)		
		balle->y += max(COEFF_Y * AccY, -balle->y + 4 * balle->size + BARRE_SIZE);
	else		
		balle->y += min(COEFF_Y * AccY, fb_yres(fb) - balle->y - 5 * balle->size);
	
	drawBall(fb, balle);
	
	return;
}
	      

unsigned int invert_y(fb_sess *fb, unsigned int y) {
	
	return fb_yres(fb) - y;
}

int getCharNum(char c) {
	
	if(c >= 32 && c <= 126)
		return c - 32;
	else
		return 63;
	
}

unsigned int drawChar(fb_sess *fb, unsigned int size, unsigned int x, unsigned int y, char c, unsigned int color, unsigned int backcolor) {
	
	unsigned int i, j, k,l;
	unsigned int tempcolor;
	
	y = invert_y(fb, y);
	
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 9; j++) {
			
			if(alphabet[getCharNum(c)][j][i] == 0)
				tempcolor = backcolor;
			else
				tempcolor = color;
			
			for(k = 0; k < size; k++)
				for(l = 0; l < size; l++)
					fb_draw_pixel(fb, x+size*i+k, y-(size*(9-j)+l), tempcolor);
		}
	}
	
	return x + size*6;
}

unsigned int drawString(fb_sess *fb, unsigned int size, unsigned int x, unsigned int y, char s[], unsigned int color, unsigned int backcolor, unsigned int firstsize) {
	
	unsigned int i = 0, offset;
	char c;
	
	c = s[i++];
	
	offset = drawChar(fb, size+firstsize, x, y, c, color, backcolor);
	
	while((c = s[i++]) != '\0') {
		
		if(offset > fb_xres(fb) - (5 + 6*size)) {
			offset = x;
			y -= 12*size + firstsize*2;
		}
		
		if(offset == x && c == ' ')
			continue;
		
		offset = drawChar(fb, size, offset, y + firstsize*2, c, color, backcolor);
	}
	
	return offset;
}




