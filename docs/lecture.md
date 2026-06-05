# cub3D Mandatory Lecture

## 1. このドキュメントの目的

このドキュメントは、`cub3D` の mandatory を
Ubuntu 22.04 + `minilibx-linux` 前提で実装するために必要な知識を整理したものである。

対象は次の 4 つ。

- `minilibx-linux` の基本
- Linux 版 MLX の event / hook の扱い
- parser / validator の実装知識
- raycasting の基礎

今回は Linux 専用で進めるので、
macOS 差分や backend abstraction の話は扱わない。

## 2. mandatory で最低限必要なこと

subject の mandatory では、少なくとも次が必要になる。

- `.cub` を読む
- `NO`, `SO`, `WE`, `EA`, `F`, `C`, map を解析する
- map が閉じていることを検証する
- player の初期位置と向きを決める
- raycasting で壁を描画する
- `WASD` と左右矢印で操作する
- `ESC` と close button で正常終了する
- window の再表示や最小化復帰で破綻しない

つまり必要なのは単なる描画ではなく、

- parser
- validator
- game state
- rendering
- input
- cleanup

の 6 つである。

## 3. Ubuntu + minilibx-linux 前提

### 3.1 何を前提にするか

今回の実装では次を固定する。

- target は Ubuntu 22.04
- library は `minilibx-linux`
- texture は `.xpm`
- input は `mlx_hook()` ベース
- key 判定は `X11/keysym.h`

### 3.2 Linux 版 MLX の build

`minilibx-linux/README.md` と test 用 Makefile から見ると、
Linux 版では次の link が必要になる。

```make
-Lminilibx-linux -lmlx -lXext -lX11 -lm -lbsd
```

環境側には少なくとも次が必要になる。

- X11 include files
- `libxext-dev`
- `libbsd-dev`

### 3.3 Libft

subject では `libft` 利用が許可されている。
そのため `Makefile` は `libft` も自動 build する前提で組む。

## 4. MiniLibX の基本

### 4.1 MLX がやっていること

MiniLibX は、window と image と event loop を扱う薄い library である。

`cub3D` では概ね次の順に使う。

1. `mlx_init()`
2. `mlx_new_window()`
3. `mlx_new_image()`
4. `mlx_get_data_addr()`
5. image buffer に自分で pixel を書く
6. `mlx_put_image_to_window()`
7. `mlx_hook()` と `mlx_loop_hook()`
8. `mlx_loop()`

### 4.2 3 つの主要 object

- `mlx_ptr`
  - MLX 全体の context
- `win_ptr`
  - window 実体
- `img_ptr`
  - image 実体

この 3 つは役割が違う。

- `mlx_ptr` は全体で 1 個
- `win_ptr` は画面
- `img_ptr` は pixel buffer

### 4.3 最低限使う API

```c
void	*mlx_init(void);
void	*mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
void	*mlx_new_image(void *mlx_ptr, int width, int height);
char	*mlx_get_data_addr(void *img_ptr, int *bpp, int *line_len, int *endian);
int		mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr,
			int x, int y);
void	*mlx_xpm_file_to_image(void *mlx_ptr, char *filename,
			int *width, int *height);
int		mlx_destroy_image(void *mlx_ptr, void *img_ptr);
int		mlx_destroy_window(void *mlx_ptr, void *win_ptr);
int		mlx_destroy_display(void *mlx_ptr);
int		mlx_hook(void *win_ptr, int event, int mask, int (*funct)(), void *param);
int		mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param);
int		mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param);
int		mlx_loop(void *mlx_ptr);
```

## 5. window に直接描かず image に描く理由

`cub3D` は毎 frame ほぼ全画面を書き換える。
この状況で `mlx_pixel_put()` を主戦力にすると遅い。

そのため実装は次で固定する。

1. frame image を 1 枚作る
2. image buffer に天井、床、壁を書き込む
3. `mlx_put_image_to_window()` で転送する

これが一番素直で壊れにくい。

## 6. pixel 書込の考え方

### 6.1 なぜこの式になるのか

```c
pixel = addr + y * line_len + x * (bpp / 8);
```

image は 2D に見えるが、memory 上では 1 本の byte 列である。

考え方:

- `addr`
  - image memory の先頭
- `y * line_len`
  - `y` 行下へ進む
- `x * (bpp / 8)`
  - その行の中で `x` pixel 分だけ右へ進む

つまり、

```text
先頭 address + y 行分 + x pixel 分
```

である。

### 6.2 `line_len` を使う理由

1 行の byte 数は単純に `width * 4` と一致するとは限らない。
そのため「1 行進む量」は `line_len` を使って計算する。

### 6.3 典型的な書き方

```c
char			*pixel;
unsigned int	*dst;

pixel = img->addr + y * img->line_len + x * (img->bpp / 8);
dst = (unsigned int *)pixel;
*dst = color;
```

## 7. event / loop

### 7.1 4 つの関数の違い

```c
int	mlx_loop(void *mlx_ptr);
int	mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param);
int	mlx_hook(void *win_ptr, int event, int mask, int (*funct)(), void *param);
int	mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param);
```

- `mlx_loop`
  - event loop を開始する
- `mlx_loop_hook`
  - event がないときに呼ぶ関数を登録する
- `mlx_hook`
  - 任意 event に callback を登録する
- `mlx_expose_hook`
  - `Expose` 専用の shortcut

一言で言うと、

- `mlx_loop` は回す
- 残りは登録する

### 7.2 `cub3D` での使い分け

- `mlx_hook`
  - key press
  - key release
  - close button
- `mlx_expose_hook`
  - 再表示時の再描画
- `mlx_loop_hook`
  - 毎 frame の更新と描画
- `mlx_loop`
  - 最後に開始

### 7.3 Expose とは何か

`Expose` は「その window を描き直してよいし、描き直すべきかもしれない」
という通知である。

典型例:

- window が隠れたあと再び見える
- 最小化から戻る
- 一部再表示が必要になる

`cub3D` では、
最新 frame をもう一度 `mlx_put_image_to_window()` すればよい。

### 7.4 `mlx_loop_hook()` は固定 FPS ではない

`mlx_loop_hook()` は「毎 frame」っぽく使えるが、
厳密な一定周期 API ではない。

そのため移動量は frame count ではなく、
`delta time` ベースで計算する方が安全である。

## 8. Linux 版 MLX で重要な罠

### 8.1 `mlx_key_hook()` を使わない

Linux 版 MLX では `mlx_key_hook()` が `KeyRelease` に寄っている。
そのため、押下中移動を作る用途に向かない。

mandatory では次の方針にする。

```c
mlx_hook(win, KeyPress, KeyPressMask, on_key_press, &game);
mlx_hook(win, KeyRelease, KeyReleaseMask, on_key_release, &game);
mlx_hook(win, DestroyNotify, 0, on_destroy, &game);
mlx_expose_hook(win, on_expose, &game);
mlx_loop_hook(mlx, on_loop, &game);
```

### 8.2 keycode は `X11/keysym.h`

Linux 前提なので、key 判定は `X11/keysym.h` を使う。

例:

- `XK_Escape`
- `XK_w`
- `XK_a`
- `XK_s`
- `XK_d`
- `XK_Left`
- `XK_Right`

### 8.3 close button

window の赤いバツは `DestroyNotify` を hook して処理する。

```c
mlx_hook(win, DestroyNotify, 0, on_destroy, &game);
```

## 9. Linux 前提の cleanup

終了時は最低限次を行う。

1. frame image を destroy
2. texture image を destroy
3. window を destroy
4. `mlx_destroy_display(mlx_ptr)`
5. `free(mlx_ptr)`

`mlx_destroy_display()` は display を閉じるだけで、
`mlx_ptr` 自体は free しない点に注意する。

## 10. parser / validator の考え方

### 10.1 `.cub` の要点

subject の重要点は次である。

- map は file の最後
- element は順不同
- element 間の空行は許可
- map の見た目はそのまま保持する
- space は map の一部

### 10.2 texture / color / map を分ける

parser は次の順に考えると分かりやすい。

1. file 全文を読む
2. 行へ分割する
3. element 行を解釈する
4. map 行を収集する

重要なのは「読込」と「意味解釈」を混ぜすぎないこと。

今回の file 構成では directory を増やしすぎないため、
validator の本体は `src/parse/validate_map.c` に寄せる想定でよい。
つまり `validate` は module の論理責務であり、
必ずしも専用 directory を切る必要はない。

### 10.3 map の閉包判定

最も堅い方法は次である。

1. raw map を保存する
2. 最大幅で長方形 grid を作る
3. 足りない場所は space で埋める
4. flood fill で外へ抜ける通路がないか確認する

space を valid map cell とみなさず、
「外側扱い」にするのが重要である。

### 10.4 runtime でも grid を使う

閉包判定だけでなく、
移動時の `can_move_to()` にも同じ `grid` を使うと単純である。

拒否すべきもの:

- `1`
- space
- 範囲外

### 10.5 player は点ではなく半径を持たせる

runtime の衝突判定では、
player を map 上の 1 点として扱うと不自然になりやすい。

起きやすい問題:

- 壁へ少しめり込んで見える
- 角を斜めにすり抜けやすい
- 壁際で視点が不自然になる

そのため `can_move_to()` は、
player 中心だけではなく小さな半径つきで判定する。

今回の設計では `PLAYER_RADIUS = 0.25` を採用する。

考え方:

- `next_x, cur_y` を先に判定する
- `cur_x, next_y` を別に判定する
- 各判定で `x ± radius`, `y ± radius` を見て、
  `1` や space に触れないか確認する

つまり本質は、
「中心点が進めるか」ではなく
「player の体積ぶんを含めても進めるか」を見ることである。

## 11. raycasting の考え方

### 11.1 そもそも何をしているか

`cub3D` は本物の 3D engine ではない。
2D の map をもとに、3D っぽく見せている。

上から見れば世界はこうである。

```text
111111
100001
10N001
100001
111111
```

ここで player の位置から ray を飛ばし、
壁までの距離を使って「画面上の縦線の高さ」を決める。

### 11.2 1 列ごとに処理する

画面全体を一気に描くのではない。
画面の `x` 列ごとに 1 本 ray を飛ばす。

正確にはこうである。

- 1 つの画面列に対応して 1 本 ray を作る
- ray は map 上を進む
- 壁に当たったら、その列に描く壁の高さを決める
- その列に対応する texture の縦 slice を描く

つまり、

```text
1 画面列 = 1 ray の結果 = 1 本の壁縦線
```

ここで注意:

- ray 自体は map 上の線であり、斜めにも飛ぶ
- 画面に描く結果が縦線である

### 11.3 `dir` と `plane`

player には 2 種類の vector を持たせる。

- `dir`
  - 正面方向
- `plane`
  - 視野の横幅を作るための vector

各列の ray は概ね次で作る。

```c
camera_x = 2 * x / (double)screen_width - 1;
ray_dir_x = dir_x + plane_x * camera_x;
ray_dir_y = dir_y + plane_y * camera_x;
```

`camera_x` は画面の左端で `-1`、中央で `0`、右端で `1` に近い。

### 11.4 DDA とは何か

ray を少しずつ動かすのでは遅い。
そこで「次の grid 境界まで一気に進む」方法を使う。
これが DDA である。

考えるもの:

- 今どの cell にいるか
- 次に縦境界へ当たるまでの距離
- 次に横境界へ当たるまでの距離

小さい方へ進み続ければ、
どの cell を通っていくかが分かる。

### 11.5 `perp_wall_dist`

壁に当たったあと、画面に使う距離は
「ray が飛んだ長さそのもの」ではなく
`perp_wall_dist` を使う。

これは、

- player 正面方向で見た奥行き距離

と考えると分かりやすい。

これを使う理由は、
画面端の斜め ray で魚眼っぽい歪みを減らすためである。

### 11.6 `line_height = screen_height / perp_wall_dist`

この式は、

- 近い壁ほど大きく見える
- 遠い壁ほど小さく見える

をそのまま表している。

例:

- 距離 `1.0` -> 高い壁
- 距離 `2.0` -> 半分くらいの高さ
- 距離 `4.0` -> さらに低い

### 11.7 texture の貼り方

壁に当たったら知りたいのは次である。

- どの方角の壁か
- 壁のどの位置に当たったか

方角で texture を選び、
当たった位置の小数部分から `tex_x` を決める。

その後、

- 画面上では `draw_start` から `draw_end`
- texture 上では `tex_y` を増やしながら

1 pixel ずつ描けばよい。

## 12. 入力処理の考え方

### 12.1 瞬間入力より key state

`cub3D` では key state を持つ方が扱いやすい。

例:

- `on_key_press`
  - `game->keys.w = 1`
- `on_key_release`
  - `game->keys.w = 0`
- `on_loop`
  - `keys` を見て移動と回転を更新

この方法の利点:

- 長押しに強い
- key repeat に依存しない
- `delta time` と相性がよい

### 12.2 movement

前進 / 後退:

- `dir_x`
- `dir_y`

を使う。

平行移動:

- `dir` を 90 度回した向き

を使う。

### 12.3 回転

左右回転は 2D 回転行列を使う。

```text
new_x = old_x * cos(rot) - old_y * sin(rot)
new_y = old_x * sin(rot) + old_y * cos(rot)
```

`dir` と `plane` の両方を同じ角度だけ回す。

## 13. 実装順の理解

最初から全部作ろうとしない方がよい。
進め方は次が自然である。

1. file / Makefile 骨格
2. `cub3d.h` と構造体
3. parser / validator
   実 file では `parse_map.c` と `validate_map.c` を軸に進める
4. MLX window と frame image
5. 単色背景
6. raycast だけで壁線
7. texture 付き壁
8. input と movement
9. cleanup
10. Ubuntu VM 最終確認

## 14. よくある失敗

### 14.1 `mlx_key_hook()` を使う

Linux では期待とずれやすい。
mandatory は `mlx_hook()` を使う。

### 14.2 `mlx_pixel_put()` 中心で描く

遅い。
frame image を使う。

### 14.3 map を trim してしまう

space は map の一部である。
見た目どおり保持する。

### 14.4 map validate と runtime map を別物にしすぎる

検証用 grid を runtime の `can_move_to()` にも使うと単純になる。

### 14.5 `line_len` を無視する

pixel 書込計算が壊れる。

### 14.6 Linux cleanup を忘れる

`mlx_destroy_display()` と `free(mlx_ptr)` が必要である。

## 15. ここまでで理解しておくべきこと

最低限、次を自分の言葉で説明できれば十分である。

- なぜ `mlx_hook()` を使うのか
- `Expose` は何か
- なぜ image に描くのか
- `addr + y * line_len + x * (bpp / 8)` の意味
- 1 画面列と 1 ray の関係
- `perp_wall_dist` を使う理由
- なぜ map を正規化 grid にして閉包判定するのか
