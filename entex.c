
#include "rtv1.h"

void	debug(t_global *g)
{
	int i;
	int j;

	i = 1;
	j = 1;
	while (j < HEIGHT - 1)
	{
		i = 1;
		while (i < WIDTH - 1)
		{
			if ((g->data_ptr[j * HEIGHT + i + 1] == 0
			&& g->data_ptr[j * HEIGHT + i - 1] == 0)
			&& g->data_ptr[(j - 1) * HEIGHT + i] == 0
			&& g->data_ptr[(j + 1) * HEIGHT + i] == 0
			&& g->data_ptr[(j + 1)* HEIGHT + i + 1] == 0
			&& g->data_ptr[(j - 1) * HEIGHT + i - 1] == 0
			&& g->data_ptr[(j + 1) * HEIGHT + i - 1] == 0
			&& g->data_ptr[(j - 1) * HEIGHT + i + 1] == 0)
				g->data_ptr[j * HEIGHT + i] = 0;
			i++;
		}
		j++;
	}
}

void	save_im(int *a, int *c, int w, int h)
{
	int i = 0;
	int j = 0;
	while (j < h)
	{
		i = 0;
		while (i < w)
		{
			*(c + j * w + i) = *(a + j * w + i);
			i++;
		}
		j++;
	}
}

int	myintmod(int x, int m)
{
	int ret;

	ret = fmod(x, m);
	if (x >= 0)
		return (ret);
	return (ret + m);
}


double	mymod(double x, int m)
{
	double ret;

	ret = fmod(x, m);
	if (x >= 0)
		return (ret);
	return (ret + m);
}

int		mid_col(int *a, int w, int h, int x, int y)
{
	t_vector vret;
	int s;
	int ret;

	s = 1;
//	ret = *(a + y * w + x);
	vret = rgb(*(a + y * w + x));
	if (x < w - 1)
	{
//		ret = ret + *(a + y * w + x + 1);
		vret = sum(vret, rgb(*(a + y * w + x + 1)));
		s++;
	}
	if (y < h - 1)
	{
		vret = sum(vret, rgb(*(a + y * w + x + w)));
//		ret = ret + *(a + (y + 1) * w + x);
		s++;
	}
	if (y < h - 1 && y < h - 1)
	{
		vret = sum(vret, rgb(*(a + y * w + x + 1 + w)));
//		ret = ret + *(a + (y + 1) * w + x + 1);
		s++;
	}
	vret = scale(1 / (double)s, vret);
	vret.x = lround(vret.x);
	vret.y = lround(vret.y);
	vret.z = lround(vret.z);
//	ret = ret / s;
	return (/*ret*/brg(vret));
}

void	rewrite_pix(int *a, int *o, int x, int y, int w, int h, int xmax, int ymax, t_global *g)
{
	t_vector mid_col;
	int			ran;
	int			ywx;

	ywx = y * w + x;
	ran = 1;
	mid_col = rgb(*(o + ywx));
	g->ray->x = x - h / 2;
	g->ray->y = -y + w / 2;

	if (x >= xmax + 1|| y >= xmax + 1)
		return ;
//	if (x > 0)
	{
		mid_col = sum(rgb(*(o + y * w + myintmod(x - 1, xmax))), mid_col);
		ran++;
	}
//	if (x < w - 1)
	{
		mid_col = sum(rgb(*(o + y * w + myintmod(x + 1, xmax))), mid_col);
		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
//	if (y > 0)
	{
		mid_col = sum(rgb(*(o + myintmod(y - 1, ymax) * w + x)), mid_col);
		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
//	if (y < h - 1)
	{
		mid_col = sum(rgb(*(o + myintmod(y + 1, ymax) * w + x)), mid_col);
		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
//	if (y < h - 1 && (x < w - 1))
	{
		mid_col = sum(rgb(*(o + myintmod(y + 1, ymax) * w + myintmod(x + 1, xmax))), mid_col);
		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
//	if (y < h - 1 && (x > 0))
	{
		mid_col = sum(rgb(*(o + myintmod(y + 1, ymax) * w + myintmod(x - 1, xmax))), mid_col);

		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
//	if (y > 0 && (x < w + 1))
	{
		mid_col = sum(rgb(*(o + myintmod(y - 1, ymax) * w + myintmod(x + 1, xmax))), mid_col);

		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
	if (y > 0 && x > 0)
	{
//		mid_col = sum(rgb(*(o + ywx - w - 1)), mid_col);
		mid_col = sum(rgb(*(o + myintmod(y - 1, ymax) * w + myintmod(x - 1, xmax))), mid_col);

		ran++;
//		if (con(g))
//			printf("sum is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);


	}
	mid_col = scale(1 / (double)ran, mid_col);
	mid_col.x = lround(mid_col.x);
	mid_col.y = lround(mid_col.y);
	mid_col.z = lround(mid_col.z);


	if (con(g))
	{
//		printf("after scale is %f,%f,%f\n", mid_col.x, mid_col.y, mid_col.z);
//		printf("rgbbgr %f,%f,%f\n", rgb(brg(mid_col)).x, rgb(brg(mid_col)).y, rgb(brg(mid_col)).z);
	}
	*(a + ywx) = brg(mid_col);
}


void	smooth(int *a, int w, int h, int xmax, int ymax, t_global *g)
{
	int i;
	int j;
	int *orig;

	i = 0;
	j = 0;
	orig = (int *)malloc(sizeof(int) * w * h + 1);
	save_im(a, orig, w, h);
	while (j < h)
	{
		i = 0;
		while (i < w)
		{
			rewrite_pix(a, orig, i, j, w, h, xmax, ymax, g);
			i++;
		}
		j++;
	}
}

void		alias(int *dst, int *a, int w, int xmax, int ymax, int h)
{
	int mid;
	int i;
	int j;

	i = 0;
	j = 0;
	while (j < h / 2 && j < xmax)
	{
		i = 0;
		while (i < w / 2 && i < xmax)
		{
			*(dst + j * (w  ) + i) = mid_col(a, w, h, 2 * i, 2 * j);
			i++;
		}
		j++;
	}
}

extern int fd;

int		start_threads(void *f, t_global *g)
{
	int 	i;
	clock_t t[2];

	t[0] = clock();
	i = -1;


	static int frame = 0;
	frame++;
// experiment start
/*
	i = i + 1;
	recalc(g->tcps[i]);
*/
//experiment end
// uncomment later

	int last_recorded_frame = 0;

	if (frame >last_recorded_frame - 1)
	{
	while (++i < CORES)
		pthread_create(&g->tid[i], NULL, f, g->tcps[i]);
	i = -1;
	while (++i < CORES)
	{
		if (WIDTH > 751)
			printf("joining cores\n");
		pthread_join(g->tid[i], NULL);
		printf("cores joined\n");
	}

	i = -1;
	//	debug(g);


/*
	if (WIDTH > 2500)
	{
		alias(g->data_ptr, g->data_ptr, WIDTH, HEIGHT, WIDTH, HEIGHT);
		printf("doing alias2\n");
	}
	if (WIDTH > 1000)
		alias(g->data_ptr, g->data_ptr, WIDTH, HEIGHT, WIDTH, HEIGHT);
*/

//	printf("putting image to window\n");
	mlx_put_image_to_window(g->mlx_ptr, g->win_ptr, g->img_ptr, 0, 0);
//	system("screencapture -x -R 0,46,1440,900 ~/Desktop/$(date +%y%m%d%H%M%S).png");

	char string[100];
//	printf("success\n");
	if (RECORD_VIDEO)
	{
		sprintf(string, "screencapture -x -R 0,46,%d,%d ~/Desktop/$(date +%%y%%m%%d%%H%%M%%S_frame%d).png", WIDTH, HEIGHT, frame);
		system(string);
	}
	}
//	printf("clock\n");
	t[1] = clock();
	printf("fps %f\n",1 / ((double)(t[1] - t[0]) / (double)CLOCKS_PER_SEC));
	return (1);
}
