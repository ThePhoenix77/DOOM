# DOOM

A 3D maze explorer built in C, inspired by the original **Wolfenstein 3D** and **DOOM** games. This project is part of the [42 school](https://42.fr) curriculum and implements a full raycasting engine from scratch using the **MiniLibX** graphics library.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Technologies](#technologies)
- [Algorithms & Implementation Details](#algorithms--implementation-details)
  - [Raycasting with DDA](#raycasting-with-dda)
  - [Perspective-Correct Wall Rendering](#perspective-correct-wall-rendering)
  - [Floor & Ceiling Casting](#floor--ceiling-casting)
  - [Texture Mapping](#texture-mapping)
  - [Player Movement & Collision Detection](#player-movement--collision-detection)
  - [Camera Rotation](#camera-rotation)
  - [Interactive Doors](#interactive-doors)
  - [Minimap](#minimap)
  - [Gun Animation](#gun-animation)
  - [Map Parsing](#map-parsing)
  - [Memory Management](#memory-management)
- [Map Format (.cub)](#map-format-cub)
- [Controls](#controls)
- [Building & Running](#building--running)
- [Project Structure](#project-structure)

---

## Overview

cub3D renders a navigable first-person 3D world from a simple 2D tile map. Every frame, a ray is cast for each vertical column of the screen. Wall distances are computed using the **Digital Differential Analysis (DDA)** algorithm, and the projected wall slices are textured based on the wall's cardinal orientation (North, South, East, West). The result is a smooth, pseudo-3D perspective — entirely in software, with no OpenGL rendering calls.

---

## Features

- **Raycasting engine** — full first-person 3D view rendered in software
- **Directional wall textures** — distinct XPM textures for N/S/E/W faces
- **Flat-shaded floor & ceiling** — configurable RGB colors per scene
- **Interactive doors** — `D` tiles that open/close on player interaction
- **Collision detection** — axis-separated hitbox with configurable margin
- **Mouse look** — horizontal camera rotation via mouse movement
- **2D minimap** — scrolling viewport centred on the player
- **Gun sprite animation** — multi-frame shooting animation rendered in HUD
- **Robust map parser** — validates `.cub` file structure, textures, colors, and map layout with clear error messages
- **Cross-platform build** — supports Linux (X11) and macOS (Cocoa/AppKit)

---

## Technologies

| Component | Details |
|---|---|
| **Language** | C (C99, compiled with `cc -Wall -Wextra -Werror -Ofast`) |
| **Graphics library** | [MiniLibX](https://github.com/42Paris/minilibx-linux) — 42's minimal X11/AppKit wrapper |
| **Math library** | `<math.h>` — `fabs`, trigonometric functions for rotation |
| **Image format** | XPM (X PixMap) for all wall textures, door textures, and sprites |
| **Build system** | GNU Make — auto-detects OS and links appropriate MLX variant |
| **Platform support** | Linux (libX11 + libXext) · macOS (OpenGL + AppKit frameworks) |
| **Coding standard** | [Norminette](https://github.com/42School/norminette) — 42 school style rules |

---

## Algorithms & Implementation Details

### Raycasting with DDA

For each vertical screen column `x`, a ray is cast from the player's position in a direction derived from the **camera plane**:

```
camera_x  = 2 * x / screen_width - 1          // [-1, 1] across the screen
ray_dir   = dir_vector + plane_vector * camera_x
```

The **Digital Differential Analysis (DDA)** algorithm then steps through the map grid cell by cell along the ray until a wall (`1`) or door (`D`) is hit:

1. **Delta distances** (`delta_dist_x`, `delta_dist_y`) — the distance the ray must travel to cross one full grid unit in each axis — are precomputed as `|1 / ray_dir|`.
2. **Side distances** (`side_dist_x`, `side_dist_y`) — distance to the first grid line crossing in each axis — are initialised from the player's fractional position.
3. At each DDA step the smaller side distance advances, moving the ray to the next cell boundary. The axis that triggers the step is recorded (`side = 0` for X wall, `side = 1` for Y wall).

### Perspective-Correct Wall Rendering

After the DDA hit, the **perpendicular wall distance** (not the Euclidean distance, which would cause a fisheye effect) is computed:

```
perp_wall_dist = (map_cell - player_pos + (1 - step) / 2) / ray_dir
```

The projected **line height** for the wall slice is then:

```
line_height = screen_height / perp_wall_dist
draw_start  = screen_height / 2 - line_height / 2
draw_end    = screen_height / 2 + line_height / 2
```

### Floor & Ceiling Casting

Floor and ceiling pixels are computed row by row. For each horizontal scanline `y` below (floor) or above (ceiling) the horizon, a `row_distance` is derived from the vertical half-angle, and the world-space floor coordinate is stepped linearly across the row using the left- and right-edge ray directions:

```
row_distance  = 0.5 * screen_height / (screen_height - y)
floor_step    = row_distance * (ray_dir_right - ray_dir_left) / screen_width
```

Each pixel is filled with the flat `c_floor` or `c_ceiling` color parsed from the `.cub` file.

### Texture Mapping

After computing `draw_start`/`draw_end`, the horizontal texture coordinate `tex_x` is determined from the exact wall hit point (`wall_x = perp_wall_dist * ray_dir + player_pos - floor(...)`). The vertical texture coordinate `tex_y` is then stepped through the texture height proportionally as each pixel of the wall slice is drawn:

```
tex_step = texture_height / line_height
tex_pos += tex_step   // per pixel
tex_y = (int)tex_pos & (texture_height - 1)
```

The four wall textures (NO/SO/EA/WE) are loaded at startup from XPM files via `mlx_xpm_file_to_image`.

### Player Movement & Collision Detection

Movement is axis-separated to allow **wall sliding**: the new X and Y positions are tested independently against the map grid. A position is walkable if the cell at `(new_x / CELL_SIZE, y / CELL_SIZE)` (and vice-versa) is not a wall. A configurable `HITBOX_MARG` (0.2 tiles) keeps the player away from wall edges.

```
new_x = player.x + dir_x * move_speed
if (is_walkable(map, new_x / 32, player.y / 32))  player.x = new_x;
if (is_walkable(map, player.x / 32, new_y / 32))  player.y = new_y;
```

### Camera Rotation

Rotation applies a standard 2D rotation matrix to both the direction vector and the camera plane:

```
new_dir_x   =  dir_x   * cos(rot_speed) - dir_y   * sin(rot_speed)
new_dir_y   =  dir_x   * sin(rot_speed) + dir_y   * cos(rot_speed)
new_plane_x =  plane_x * cos(rot_speed) - plane_y * sin(rot_speed)
new_plane_y =  plane_x * sin(rot_speed) + plane_y * cos(rot_speed)
```

Mouse input is captured via `mlx_mouse_hook` and maps horizontal cursor delta to the same rotation, giving smooth mouse-look.

### Interactive Doors

Door cells (`D`) are detected during the DDA traversal. The engine checks the player's proximity and a dedicated key binding to toggle the door's open/closed state, replacing the `D` tile with a space (`0`) when open and restoring it when closed.

### Minimap

A 2D minimap is drawn in the top-left corner every frame. A **scrolling viewport** is computed by centering a fixed-size tile window on the player's grid position:

```
start_x = player_tile_x - (MINIMAP_WIDTH  / (2 * (CELL_SIZE / MINIMAP_SCALE)))
start_y = player_tile_y - (MINIMAP_HEIGHT / (2 * (CELL_SIZE / MINIMAP_SCALE)))
```

Each visible tile is drawn as a small colored rectangle (`CELL_SIZE / MINIMAP_SCALE` pixels), and the player is rendered as a dot at the minimap center.

### Gun Animation

Multiple XPM frames for the gun sprite are loaded at startup into a `t_textures *gun[]` array. The game loop advances `current_frame` based on elapsed time (`frame_delay`). Each frame is blitted onto the lower-center area of the screen using a scanline copy that respects the source/destination bits-per-pixel values.

### Map Parsing

The parser reads the `.cub` configuration file in two passes:

1. **Header pass** — extracts texture paths (`NO`, `SO`, `EA`, `WE`) and floor/ceiling RGB triples (`F`, `C`). Values are validated (file existence, 0–255 color range, no duplicates).
2. **Map pass** — everything after the last header line is treated as the map grid. The map is validated for:
   - Allowed characters only (`0`, `1`, `D`, `N`, `S`, `E`, `W`, space)
   - Exactly one player start position
   - Closed boundaries — every `0`/`D`/player cell must be surrounded by walls or other non-empty cells (flood-fill style border check)

All parsing errors print a descriptive message to stderr and exit cleanly.

### Memory Management

A custom `ft_malloc` wrapper maintains a linked list (`t_free`) of all allocations. Passing `FREE` status deallocates the entire list at once, preventing leaks on both normal exit and error paths — without relying on `gc`-style allocators.

---

## Map Format (.cub)

```
NO assets/north_wall.xpm
SO assets/south_wall.xpm
EA assets/east_wall.xpm
WE assets/west_wall.xpm

F 92,97,103        # Floor color  (R,G,B)
C 193,10,0         # Ceiling color (R,G,B)

111111
100001
1000N1        # N = player start facing North (N/S/E/W)
1000D1        # D = door
100001
111111
```

Map characters:

| Char | Meaning |
|------|---------|
| `1`  | Wall |
| `0`  | Walkable floor |
| `D`  | Door |
| `N` `S` `E` `W` | Player start + initial facing direction |
| ` ` (space) | Outside map boundary |

---

## Controls

| Key | Action |
|-----|--------|
| `W` | Move forward |
| `S` | Move backward |
| `A` | Strafe left |
| `D` | Strafe right |
| `←` / `→` | Rotate camera left / right |
| Mouse X | Rotate camera (mouse-look) |
| Left click | Shoot (gun animation) |
| `E` | Open / close door |
| `ESC` | Quit |
| Window ✕ | Quit |

---

## Building & Running

**Dependencies (Linux):** `gcc`, `make`, `libX11-dev`, `libXext-dev`  
**Dependencies (macOS):** Xcode Command Line Tools (AppKit/OpenGL included)

```bash
# Clone and build
git clone <repo-url> cub3D
cd cub3D
make

# Run with a map file
./cub3D maps/map.cub
```

`make re` performs a full clean rebuild. The Makefile auto-detects the OS and links the correct MiniLibX variant.

---

## Project Structure

```
.
├── Makefile
├── assets/             # XPM wall textures, door texture, gun animation frames
├── inc/
│   ├── linux_inc/
│   │   ├── cub3d.h         # Main header (structs, constants, prototypes) — Linux
│   │   └── cub3d_bonus.h
│   └── macos_inc/          # macOS variant
├── maps/               # Sample .cub map files
├── mlx_linux/          # MiniLibX for Linux (X11)
├── mlx_macos/          # MiniLibX for macOS (AppKit)
└── src/
    ├── bonus/
    │   ├── main.c              # Entry point, MLX hooks setup
    │   ├── raycasting.c        # DDA raycasting loop
    │   ├── raycasting_utils.c  # Step/side distance helpers
    │   ├── render.c            # Frame render orchestration
    │   ├── render_utils.c/2    # Pixel drawing, wall drawing
    │   ├── ceil_and_floor.c    # Floor/ceiling casting + minimap viewport
    │   ├── minimap.c/2         # Minimap tile & player rendering
    │   ├── movements.c/2       # Player movement & rotation
    │   ├── keys.c/2            # Keyboard press/release handlers
    │   ├── mouse.c             # Mouse-look handler
    │   ├── data.c              # Game state initialisation
    │   ├── data_utils.c/2/3    # Texture loading, color init, gun frames
    │   └── utils.c             # Misc helpers (clear screen, game loop)
    └── parsing/
        ├── parsing.c           # Top-level parse entry
        ├── map_parsing.c       # Map grid validation
        ├── checking_map.c      # Boundary & character checks
        ├── parse_data.c/2      # Header field extraction
        ├── fill_data.c         # Texture/color fill helpers
        ├── load_texture.c      # XPM → MLX image loading
        ├── draw_walls.c        # Wall texture pixel sampling
        ├── gun.c               # Gun sprite frame rendering
        ├── get_next_line.c     # Line-by-line file reader
        ├── ft_split.c          # String split utility
        └── utils.c/2/3/4       # String, memory, and I/O helpers
```
