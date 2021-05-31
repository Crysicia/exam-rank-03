#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct s_map
{
	int width;
	int height;
	char color;
	char *matrice;
}				t_map;

typedef struct s_shape
{
	float x;
	float y;
	float width;
	float height;
	char type;
	char color;
}				t_shape;

void display(t_map *map)
{
	int i = 0;
	while (i < map->height)
	{
		write(1, &map->matrice[i * map->width], map->width);
		write(1, "\n", 1);
		i++;
	}
}

int ft_error(char *msg, int code)
{
	int i;
	i = 0;
	while (msg[i])
		i++;
	write(1, msg, i);
	return code;
}

int parse_map(FILE *file, t_map *map)
{
	int ret = fscanf(file, "%d %d %c\n", &map->width, &map->height, &map->color);
	if (ret != 3 || map->width < 1 || map->width > 300 || map->height < 1 || map->height > 300)
		return 1;
	map->matrice = malloc(map->width * map->height);
	if (!map->matrice)
		return 1;
	memset(map->matrice, map->color, map->width * map->height);
	return 0;
}

void draw_pixel(t_map *map, t_shape *shape, int x, int y)
{
	float xf = x;
	float yf = y;

	if (xf < shape->x || shape->x + shape->width < xf || yf < shape->y || shape->y + shape->height < yf)
		return ;
	if (((xf - shape->x < 1.0000000 || shape->x + shape->width - xf < 1.0000000 || yf - shape->y < 1.0000000 || shape->y + shape->height - yf < 1.0000000) && shape->type == 'r') || shape->type == 'R')
		map->matrice[(y * map->width) + x] = shape->color;

}

void draw_shape(t_shape *shape, t_map *map)
{
	for (int y = 0; y < map->height; y++)
		for (int x = 0; x < map->width; x++)
			draw_pixel(map, shape, x, y);
}

int invalid_shape(t_shape *shape)
{
	if (shape->width <= 0.00000000 || shape->height <= 0.00000000 || (shape->type != 'R' && shape->type != 'r'))
		return 1;
	return 0;
}

int ft_exec(FILE *file, t_map *map)
{
	int ret = 6;
	t_shape shape;
	if (parse_map(file, map) == 1)
		return 1;
	while ((ret = fscanf(file, "%c %f %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.width, &shape.height, &shape.color)) == 6)
	{
		if (invalid_shape(&shape))
			return 1;
		draw_shape(&shape, map);
	}
	if (ret != -1)
		return 1;
	return 0;
}

int main(int argc, char *argv[])
{
	FILE *file;
	t_map map;
	
	map.matrice = NULL;
	if (argc != 2)
		return ft_error("Error: argument\n", 1);
	file = fopen(argv[1], "r");
	if (!file || ft_exec(file, &map) == 1)
		return ft_error("Error: Operation file corrupted\n", 1);
	display(&map);
	fclose(file);
	free(map.matrice);
	return 0;
}
