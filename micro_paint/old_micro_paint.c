#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>


#define FULL 'R'
#define EMPTY 'r'

typedef struct s_map
{
	int height;
	int width;
	char *map;
}			t_map;

typedef struct s_rect
{
	float x;
	float y;
	float width;
	float height;
	char c;
	char type;
}			t_rect;

void debug(t_map *map)
{
	//printf("W: %d, H: %d\n", map->width, map->height);
	for (int i = 0; i < map->height; i++)
	{
		write(1, &map->map[i * map->width], map->width);
		write(1, "\n", 1);
	}
}

int ft_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i])
		i++;
	return i;
}

void ft_putstr(char *str)
{
	write(1, str, ft_strlen(str));
}

int ft_error(char *str, FILE *file)
{
	if (file)
		fclose(file);
	ft_putstr("Error: ");
	ft_putstr(str);
	ft_putstr("\n");
	return 1;
}

int parse_map(FILE *file, t_map *map)
{
	char fill;
	int ret = fscanf(file, "%d %d %c", &map->width, &map->height, &fill);
	if (ret != 3 || (map->width < 1 || map->width > 300) || (map->height < 1 || map ->height > 300))
		return (1);
	map->map = malloc(sizeof(char) * (map->width * map->height));
	if (!map->map)
		return 1;
	memset(map->map, fill, map->width * map->height);
	return 0;
}

#define INSIDE 1
#define BORDER 2
int is_in(t_rect *rect, float x, float y)
{
	float xtl;
	float xbr;
	float ytl;
	float ybr;

	xtl = rect->x;
	ytl = rect->y;

	xbr = xtl + rect->width;
	ybr = ytl + rect->height;
	if (x < xtl || xbr < x || y <  ytl || ybr < y)
		return 0;
	if (x - xtl < 1.00000000 || xbr - x < 1.00000000 || y - ytl < 1.0000000 ||  ybr - y < 1.0000000)
		return BORDER;
	return INSIDE;
}

void fill_pixel(t_map *map, int x, int y, t_rect *rect)
{
	int inside;
	inside = is_in(rect, (float)x, (float)y);
	if (inside == BORDER || (inside == INSIDE && rect->type == FULL))
		map->map[x + (y * map->width)] = rect->c;
}

int apply_rect(t_map *map, t_rect *rect)
{
	//printf("RECT W: %f, H: %f, TYPE: %c, X: %f, Y: %f, CHAR: %c\n", rect->width, rect->height, rect->type, rect->x, rect->y, rect->c);
	if ((rect->width < 0.00000000 || rect->height < 0.0000000) || (rect->type != FULL && rect->type != EMPTY))
		return 1;
	for (int y = 0; y < map->height; y++)
		for (int x = 0; x < map->width; x++)
			fill_pixel(map, x, y, rect);
	return 0;
}

int ft_exec(FILE *file)
{

	t_map map;
	t_rect rect;
	int ret;

	if (parse_map(file, &map))
		return 1;
	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.c)) == 6)
	{
//printf("RET: %d, W: %f, H: %f, TYPE: %c, X: %f, Y: %f, CHAR: %c\n", ret, rect.width, rect.height, rect.type, rect.x, rect.y, rect.c);
		if (apply_rect(&map, &rect))
			return 1;
	}
//printf("RET: %d, W: %f, H: %f, TYPE: %c, X: %f, Y: %f, CHAR: %c\n", ret, rect.width, rect.height, rect.type, rect.x, rect.y, rect.c);
	if (ret == -1)
	{
		debug(&map);
		return 0;
	}
	return 1;
}

int main(int argc, char *argv[])
{
	FILE *file;

	if (argc != 2)
		return (ft_error("argument", NULL));
	file = fopen(argv[1], "r");
	if (!file || ft_exec(file))
		return (ft_error("Operation file corrupted", file));
	return 0;
}
