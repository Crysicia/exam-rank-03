#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct s_map
{
	int width;
	int height;
	char *matrice;
	char color;
}			t_map;

typedef struct s_shape
{
	char type;
	float x;
	float y;
	float radius;
	char color;
}			t_shape;

int ft_error(char *msg, int code)
{
	int i = 0;
	while (msg[i])
		i++;
	write(1, msg, i);
	return code;
}

float sq(float x)
{
	return x * x;
}

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

void draw_pixel(t_map *map, t_shape *shape, int x, int y)
{
	float distance = sqrtf(sq((float)x - shape->x) + sq((float)y - shape->y)) - shape->radius;

	if (distance > 0.00000000)
		return ;
	if ((distance > -1.00000000 && shape->type == 'c') || shape->type == 'C')
		map->matrice[x + (y * map->width)] = shape->color;
}

void draw_shape(t_map *map, t_shape *shape)
{
	for (int y = 0; y < map->height; y++)
		for (int x = 0; x < map->width; x++)
			draw_pixel(map, shape, x, y);
}

int invalid_shape(t_shape *shape)
{
	if (shape->radius <= 0.00000000 || (shape->type != 'c' && shape->type != 'C'))
		return 1;
	return 0;
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

int ft_exec(FILE *file, t_map *map)
{
	int ret;
	t_shape shape;

	if (parse_map(file, map))
		return 1;
	while ((ret = fscanf(file, "%c %f %f %f %c\n", &shape.type, &shape.x, &shape.y, &shape.radius, &shape.color)) == 5)
	{
		if (invalid_shape(&shape))
			return 1;
		draw_shape(map, &shape);
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
	if (!file || ft_exec(file, &map))
	{
		fclose(file);
		free(map.matrice);
		return ft_error("Error: Operation file corrupted\n", 1);
	}
	display(&map);
	fclose(file);
	free(map.matrice);
	return 0;
}
