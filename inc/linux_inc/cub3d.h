#ifndef CUB3D_H
#define CUB3D_H

#include <../../mlx_linux/mlx.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
# include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

// Macro for enabling/disabling debug output
#define DEBUG_MODE 1

#if DEBUG_MODE
#define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

enum
{
    FREE,
    ALLOC,
};

// 💨🥂🍎🍌🥬 ou tiriri m3a 💃
// --------------------
// Constants
// --------------------       
#define CELL_SIZE 32          // Map tile size
#define FOV 0.66               // Field of view

#if defined(__APPLE__)
# define W_KEY 13
# define A_KEY 0
# define S_KEY 1
# define D_KEY 2
# define LEFT_KEY 123
# define RIGHT_KEY 124
# define ESCAPE_KEY 53
#else
# define W_KEY 119
# define A_KEY 97
# define S_KEY 115
# define D_KEY 100
# define LEFT_KEY 65361
# define RIGHT_KEY 65363
# define ESCAPE_KEY 65307
#endif

#define MOVE_SPEED 2.8
#define ROT_SPEED 0.06
#define HITBOX_MARG 0.2
// #define HITBOX_MARG 0.15 // more edge close

#define PI 3.141592653589793

#define MAP_WIDTH 1366
#define MAP_HEIGHT 768

#define MINIMAP_CENTER_X 100 // Adjust as needed for your window dimensions
#define MINIMAP_CENTER_Y 100
#define MINIMAP_TILE_SIZE 10

#define MINIMAP_RADIUS 100
#define MINIMAP_WIDTH 200
#define MINIMAP_HEIGHT 200
#define MINIMAP_SCALE 4
#define TRAIL_MAX_SIZE 100 

#define CEILING_COLOR 0x87CEEB // Sky blue
#define FLOOR_COLOR 0x8B4513 // rown
#define MINIMAP_FRAME_COLOR 0xFFD700 // rown


//540   960


// --------------------
// Structures
// --------------------

typedef struct s_player_trail
{
    int x[TRAIL_MAX_SIZE];   // X coordinates of trail positions
    int y[TRAIL_MAX_SIZE];   // Y coordinates of trail positions
    int size;                // Current size of the trail (number of positions stored)
} t_player_trail;

// Player structure
typedef struct s_player
{
    double x;
	double y;                // Player's position in the map
    double dir_x;
	double dir_y;        // Direction vector
    double plane_x;
	double plane_y;    // Camera plane (used for FOV calculations)
    int forward;
    int backward;
    int left;
    int right;
    int rotate_left;
    int rotate_right;
    double move_speed;
    double rot_speed;
    int first_time;
    int prev_x;
    int door_open;
} t_player;

// Map structure
typedef struct s_map
{
    char **grid;                // 2D grid representing the map
    int width;
	int height;          // Map dimensions
} t_map;


typedef struct s_free
{
    void    *add;
    struct s_free   *next;
}               t_free;


// Image structure (for textures or frame buffers)
typedef struct s_img
{
    void *img_ptr;              // Image pointer in MiniLibX
    unsigned char *data;                 // Pixel data
    int bpp;                    // Bits per pixel
    int size_line;              // Size of a line
    int endian;   
    int width;                  // Width of the image
    int height;               // Endianness of pixel data
} t_img;

typedef struct s_ray
{
    double camera_x;
    double ray_dir_x;
    double ray_dir_y;
    int map_x;
    int map_y;
    double delta_dist_x;
    double delta_dist_y;
    double side_dist_x;
    double side_dist_y;
    double perp_wall_dist;   ///distination from the player to the wall
    int step_x;
    int step_y;
    int hit;
    int side;
    int line_height;
    int draw_start;
    int draw_end;
    int is_door;
} t_ray;

typedef struct s_minimap
{
    int start_x;
    int start_y;
    int end_x;
    int end_y;
} t_minimap;

typedef struct s_ceil_floor
{
    double ray_dir_x0;
    double ray_dir_y0;
    double ray_dir_x1;
    double ray_dir_y1;
    double floor_step_x;
    double floor_step_y;
    double floor_x;
    double floor_y;
} t_ceil_floor;


typedef struct s_textures
{
    void    *image;
    void    *add;
    int     width;      // Texture width
    int     height;     // Texture height
    int     bp_pixels; // Bits per pixel (MLX fills this)
    int     l_length;    // Bytes per row (MLX fills this)
    int     endian; 
} t_textures;

typedef struct s_game
{
	char	*norh;
	char	*west;
	char	*east;
	char	*south;
	char	*floor;
	char	*ceiling;
    unsigned int    c_floor;
    unsigned int    c_ceiling;
    int             current_frame;
    double          last_frame_time;
    double          frame_delay;
    int             frame_counter;
    int             is_shooting;
    // t_textures *gun[50];
    t_textures *gun[21];
    t_textures *image[5];

} t_game;


typedef struct s_parsing
{
	int			fd;
	int			file_lenght;
	int			map_start_index;
	char		**file_lines;
} t_parsing;

typedef struct s_data
{
    void *mlx;                  // MiniLibX instance
    void *win;                  // Window pointer
    t_map map;                  // Map data
    t_player player;            // Player data
    t_player_trail player_trail;
    t_minimap minimap;
    t_ceil_floor ceil_floor;
    t_ray ray;
    t_img img;
    t_game  game;
    t_parsing *info;
    // int key_state[256];
} t_data;

typedef struct s_cordinate
{
    int     y;
    int     color;
    int     tex_num;
    double  wall_x;
    int     tex_x;
    int     tex_y;
    double  tex_step;
    double  tex_pos;
}               t_cordinate;

typedef struct s_info
{
    int             x;
    int             y;
    int             j;
    int             dest_y;
    int             bpp_src;
    int             bpp_dst;
    char            *src_row;
    unsigned char   *dst_row;
}               t_info;



// --------------------
// Prototypes
// --------------------

// Initialization
void map_init(t_map *map, const char *map_file);
void set_player_plane(t_player *player);
int find_player_position(t_map *map, t_player *player);
void data_init(t_data *data);
void	init_player_defaults(t_player *player);
void init_player_direction(t_player *player, char orientation);
void	init_player_trail(t_player_trail *trail);
void	init_map_and_player(t_data *data);
bool	is_player_cell(char cell);
void	set_player_direction(char cell, t_player *player);
void	set_direction(t_player *player, double dir_x, double dir_y);

                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Rendering
int get_pixel_index(int x, int y, t_img *img);
void draw_pixel(t_img *img, int x, int y, int color);
void put_pixel_to_image(t_img *img, int x, int y, int color, int img_width, int img_height);
void calculate_line_steps(int x1, int y1, int x2, int y2, int *dx, int *dy, int *p);
int color_cell_matching(char cell);
void draw_line(t_img *img, int x1, int y1, int x2, int y2, int color);
void draw_rectangle(t_img *img, int x, int y, int width, int height, int color, int img_width, int img_height);
void render(t_data *data);
void map_free(t_map *map);
void raycast(t_data *data);
void render_minimap(t_data *data);
void draw_rectangle(t_img *img, int x, int y, int width, int height, int color, int img_width, int img_height);
void render_3d_view(t_data *data);
void render_ceiling_and_floor(t_data *data);


                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Minimap
void draw_minimap_tiles(t_data *data);
void draw_player_on_minimap(t_data *data);
void calculate_viewport(t_data *data);
void render_ceiling_and_floor(t_data *data);

                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Raycasting
// void initialize_ray(t_ray *ray, const t_data *data, int x);
// void calculate_step_and_side_dist(t_ray *ray, const t_data *data);
// void perform_dda(t_ray *ray, const t_map *map);
// void calculate_wall_distance(t_ray *ray, const t_player *player);
// void calculate_line_dimensions(t_ray *ray, int screen_height);
// void draw_wall(const t_ray *ray, t_data *data, int x);
// void raycast(t_data *data);
void initialize_ray(t_ray *ray, t_data *data, int x);
void	calculate_step_and_side_dist(t_ray *ray, t_data *data);
void	perform_dda(t_ray *ray, t_map *map, t_player *player);
void calculate_wall_distance(t_ray *ray, t_player *player);
void calculate_line_dimensions(t_ray *ray, int screen_height);
void draw_wall(t_ray *ray, t_data *data, int x);
void raycast(t_data *data);

                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Movements
int is_walkable(t_map *map, double x, double y);
void move_forward(t_player *player, t_map *map);
void move_backward(t_player *player, t_map *map);
void strafe_left(t_player *player, t_map *map);
void strafe_right(t_player *player, t_map *map);
void rotate_left(t_player *player);
void rotate_right(t_player *player);
void move_player(t_data *data);
void handle_key_press(int key, t_data *data);
void handle_key_press_rotation(int key, t_data *data);
int key_press(int key, t_data *data);
void handle_key_release(int key, t_data *data);
void handle_key_release_rotation(int key, t_data *data);
int key_release(int key, t_data *data);
// void ppos(t_player *player);
// void draw_rectangle(t_img *img, int x, int y, int width, int height, int color);


                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Mouse
void setup_mouse_rotation(t_data *data);
int mouse_press(int button, int x, int y, t_data *data);

                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

//Doors
void handle_door(t_ray *ray, t_data *data);

                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Utils
void quit(char *msg);
void ft_bzero(void *s, size_t n);
int game_loop(t_data *data);
void clear_img(t_data *data);
void clear_screen(t_data *data, int color);
void draw_vertical_line2(t_img *img, int x, t_ray *ray, int color);

                                /*~~~~ /\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\ ~~~~*/

// Parsing
void    *ft_malloc(int size, int status);
void    my_perror(int status, char *str);
int        ft_strncmp(const char *s1, const char *s2, int n);
int         ft_strlen(const char *str);
int        check_file(const char *argv);
void    file_parsing(t_data *data, char *str);
void    store_data(t_data *data, int fd);
char    *ft_strdup(char *s1);
char    *ft_strchr(const char *s, int c);
char    *ft_substr(char *s, unsigned int start, size_t len);
char    **ft_split(char const *s, char c);
// char    **ft_split2(char const *s);
char    *get_next_line(int fd);
char    *ft_strjoin(char *s1, char *s2);
void    fill_texture(char *str, char **texture, int *count);
int        file_lenght(char *str);
void    get_file_lines(char *str, t_parsing *info);
int        map_first_line_index(char **str);
void    parse_map(t_data *data);
void    check_rest(char    **str);
char    *ft_substr2(char *s, unsigned int start, size_t len);
void    *ft_calloc(size_t count, size_t size);
int        only_space(char *str);
int        ft_strlen2(char **str);
void    check_map_surrending(char **map);
void    free_str(char **str);
int        is_num(char *str);
// int        ft_atoi(const char *str);
int	    ft_atoi(const char *str, int *succes);
void     print_data(t_data *data);
void    check_inside_map(char **map);
void    check_texture(char *str, t_game *game, int *count);
void     print_2d_array(char **array);
char    *ft_strtrim(char const *s1, char const *set);
void    fill_color(char **str, char **texture, int *count);
int        is_num2(char *str);
void print_map(char **map);
char	*ft_strdup2(char *s1);
char	*ft_itoa(int nb);
int	is_map_line(char *str);
int	wrong_char(char c);
int	map_lenghttttttttttttt(t_data *data);
// void	my_perror(int status, char *str);
// int		ft_strncmp(const char *s1, const char *s2, int n);
// int	 	ft_strlen(const char *str);
// int		check_file(const char *argv);
// void	file_parsing(t_data *data, char *str);
// void	store_data(t_data *data, int fd);
// char	*ft_strdup(char *s1);
// char	*ft_strchr(const char *s, int c);
// char	*ft_substr(char *s, unsigned int start, size_t len);
// char	**ft_split(char const *s, char c);
// char	*get_next_line(int fd);
// char	*ft_strjoin(char *s1, char *s2);
// void	fill_texture(char *str, char **texture, int *count);
// int		file_lenght(char *str);
// void	get_file_lines(char *str, t_parsing *info);
// int		map_first_line_index(char **str);
// void	parse_map(t_data *data);
// void	fill_ceiling(char *str, char **texture, int *count);
// void	fill_floor(char *str, char **texture, int *count);
// void	check_rest(char	*str);
// char	*ft_substr2(char *s, unsigned int start, size_t len);
// void	*ft_calloc(size_t count, size_t size);
// int		only_space(char *str);
// int		ft_strlen2(char **str);
// void    check_map_surrending(char **map);
// void	free_str(char **str);
// int		is_num(char *str);
// int		ft_atoi(const char *str);
// void 	print_data(t_data *data);
// void    check_inside_map(char **map);


///////////////////////////////////////////////////////////////////////////////////
//textures  and colors //
///////////////////////////////////////////////////////////////////////////////////

void	images_init(t_data *data);
void	free_textures(t_data *data);
void	colors_init(t_data *data);
void    load_gun_frames(t_data  *data);
void    render_gun_sprite(t_data *data);

#endif