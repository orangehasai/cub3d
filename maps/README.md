# Map Assets

Valid cases:

- `maps/valid/smoke_minimal.cub`: smallest positive smoke case
- `maps/valid/corridor_long.cub`: long horizontal corridor layout
- `maps/valid/vertical_keep.cub`: tall and narrow stacked layout
- `maps/valid/offset_padded.cub`: leading-space padded map with uneven rows
- `maps/valid/elements_shuffled.cub`: valid scene with texture/color identifiers in mixed order
- `maps/valid/elements_shuffled_blanklines.cub`: mixed order plus blank lines, tabs, and leading spaces
- `maps/valid/rooms_wide.cub`: medium multi-room layout
- `maps/valid/grand_hall.cub`: large open hall
- `maps/valid/mega_dungeon.cub`: large maze-style stress map

All valid maps use the existing textures under `texture/`.

Invalid cases are split so each file targets one failure cause:

- `maps/invalid/duplicate_color.cub`: duplicate color identifier
- `maps/invalid/duplicate_texture.cub`: duplicate texture identifier
- `maps/invalid/empty_line_inside_map.cub`: empty line inside map
- `maps/invalid/invalid_color.cub`: invalid color value
- `maps/invalid/invalid_map_character.cub`: invalid map character
- `maps/invalid/invalid_texture_image.cub`: readable `.xpm` path with broken XPM data
- `maps/invalid/invalid_texture_path.cub`: unreadable texture path
- `maps/invalid/missing_color.cub`: missing color identifier
- `maps/invalid/missing_map.cub`: missing map
- `maps/invalid/missing_spawn.cub`: missing player spawn
- `maps/invalid/missing_texture.cub`: missing texture identifier
- `maps/invalid/multiple_spawns.cub`: multiple player spawns
- `maps/invalid/open_border.cub`: map is not closed because `0` reaches border
- `maps/invalid/open_space_leak.cub`: map is not closed because space touches floor
- `maps/invalid/texture_not_xpm.cub`: texture path does not use `.xpm`
