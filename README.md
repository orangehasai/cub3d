*This project has been created as part of the 42 curriculum by stonegaw, skeita.*

# cub3D

## Description

`cub3D` is the mandatory part of the 42 graphics project based on a simple
ray-caster inspired by Wolfenstein 3D. The program parses a `.cub` scene file,
validates the map, loads four wall textures plus floor and ceiling colors, and
renders a first-person view with MiniLibX on Linux.

## Instructions

### Build

```bash
make
```

### Run

Run the program with a `.cub` scene:

```bash
./cub3D maps/valid/minimal.cub
```

Other example maps:

```bash
./cub3D maps/valid/offset.cub
./cub3D maps/valid/elements_shuffled.cub
./cub3D maps/valid/big.cub
./cub3D maps/valid/mega.cub
```

### Controls

- `W` / `A` / `S` / `D`: move
- `Left Arrow` / `Right Arrow`: rotate camera
- `ESC`: quit cleanly
- Window close button: quit cleanly

## Resources

Project references:

[Cud3Dの資料](https://docs.google.com/presentation/d/1dQrnEUmDkodlIC366O_YdLbO2-MBsuzEkgC8F2xjxOY/edit?slide=id.p#slide=id.p)
[Lode's Computer Graphics Tutorial](https://lodev.org/cgtutor/raycasting.html)

AI usage:

- AI was used to review parser edge cases, clarify MLX/X11 event behavior,
  explain ray-casting math, formulas, and rendering rationale, and help draft
  test assets and documentation.
- All generated suggestions were manually checked against the subject, the
  current codebase, and actual build behavior before being kept.
