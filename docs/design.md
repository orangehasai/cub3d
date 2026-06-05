# cub3D 設計

## 1. 目的

この設計書は `docs/subject.md` の mandatory part を、
Ubuntu 22.04 + `minilibx-linux` 前提で実装するための土台を定義する。

狙いは次の 3 点である。

1. mandatory を通すために必要な責務分離だけを残す
2. Norm を守れる粒度で file / function を分ける
3. 実装量と保守性のバランスを取る

今回は Linux 固定で進めるため、以前の `mac -> Linux` 移行前提の抽象化は採用しない。

## 2. 固定方針

- 実行環境は Ubuntu 22.04 + `minilibx-linux` とする
- scope は mandatory のみとする
- backend 差分吸収用の `platform` 層は作らない
- key / event は Linux 前提で `X11/X.h`, `X11/keysym.h` をそのまま使う
- texture は `.xpm` を前提にする
- 描画は off-screen image に行い、最後に `mlx_put_image_to_window()` する
- グローバル変数は使わない
- map は file の見た目どおり保持し、検証用に別の正規化 grid を作る
- 衝突判定は `can_move_to()` に集約する
- movement / rotation は `delta time` ベースで更新する

## 3. 過剰にしないための判断

今回あえてやらないこと:

- mac 対応
- backend abstraction
- bonus のための先回り実装
- public / private header の細分化
- module を増やしすぎる分割

今回残すこと:

- `parse`, `validate`, `init`, `render`, `input`, `cleanup` の責務分離
- map 閉包判定のための padding grid + flood fill
- raycast 計算状態を構造体へ寄せる設計
- cleanup の逆順解放

この設計は「最小コード」ではなく、
「mandatory を安定して通しやすい最小限の整理」を狙う。

## 4. モジュール構成

### 4.1 module

- `main`
  - 引数検証
  - 全体初期化呼び出し
  - loop 開始
- `init`
  - game 構造体初期化
  - MLX 初期化
  - texture 読み込み
  - 終了処理
- `parse`
  - file 読み込み
  - texture / color / map 解析
- `validate`
  - map 文字検証
  - spawn 検証
  - 閉包判定
- `render`
  - frame 背景塗り
  - raycast
  - texture sampling
- `input`
  - key press / release
  - movement / rotation
  - loop update
- `utils`
  - error 出力
  - free 補助
  - 文字列補助

### 4.2 依存方向

依存は次の方向だけにする。

```text
main
  -> init
  -> parse
  -> validate
  -> render
  -> input
  -> utils
```

`render` が `parse` の内部事情を知らないように、
必要な情報は `t_game`, `t_map`, `t_player`, `t_img` に集約する。

## 5. Norm 前提の分割方針

- 関数は 25 行以内
- 引数は 4 個以内
- ローカル変数は 5 個以内
- `for`, `switch`, `do...while`, 三項演算子は使わない
- 1 `.c` file の関数定義は 5 個以内

そのため、

- parser は `read_file`, `parse_elements`, `parse_map`, `validate_map` に分ける
- render は `render_frame`, `raycast`, `texture_sample` に分ける
- input は `hook`, `move` に分ける
- 1 列分の ray 状態は `t_ray` にまとめる

## 6. データ構造

### 6.1 image / color

```c
typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
	int	value;
}	t_rgb;

typedef struct s_img
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_img;
```

### 6.2 map / scene

```c
typedef struct s_map
{
	char	**rows;
	char	**grid;
	int		width;
	int		height;
}	t_map;

typedef struct s_scene
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	t_rgb	floor;
	t_rgb	ceiling;
	t_map	map;
}	t_scene;
```

設計意図:

- `rows` は file の見た目を保持する
- `grid` は padding 付き長方形 map として保持する
- runtime の衝突判定も `grid` を使う

### 6.3 player / input / time

```c
typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
	double	plane_x;
	double	plane_y;
}	t_player;

typedef struct s_keys
{
	int	w;
	int	a;
	int	s;
	int	d;
	int	left;
	int	right;
}	t_keys;

typedef struct s_time
{
	long	prev_usec;
	double	delta_sec;
}	t_time;
```

### 6.4 ray

```c
typedef struct s_ray
{
	int		x;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		side;
	double	camera_x;
	double	ray_dir_x;
	double	ray_dir_y;
	double	side_dist_x;
	double	side_dist_y;
	double	delta_dist_x;
	double	delta_dist_y;
	double	perp_wall_dist;
	double	wall_x;
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		tex_x;
}	t_ray;
```

### 6.5 game

```c
typedef struct s_game
{
	void		*mlx;
	void		*win;
	t_img		frame;
	t_img		no_tex;
	t_img		so_tex;
	t_img		we_tex;
	t_img		ea_tex;
	t_scene		scene;
	t_player	player;
	t_keys		keys;
	t_time		time;
	int			running;
}	t_game;
```

## 7. header 方針

今回の mandatory では header は `include/cub3d.h` の 1 枚を基本とする。

ここへ置くもの:

- 必要な system include
- MLX include
- X11 event / keysym include
- 定数マクロ
- `struct` 定義
- prototype

分割しすぎない代わりに、
未使用 include と循環依存を作らないことを重視する。

## 8. 初期化シーケンス

```text
main
  -> 引数検証
  -> t_game zero initialize
  -> .cub 読込
  -> element parse
  -> map parse
  -> map validate
  -> player 初期化
  -> mlx_init
  -> mlx_new_window
  -> frame image 作成
  -> texture image 読込
  -> hook 登録
  -> mlx_loop_hook 登録
  -> mlx_loop
```

要点:

- parse / validate が通るまで window を開かない
- texture path の妥当性確認は parse 時に行う
- 実画像の読込は MLX 初期化後に行う

## 9. `.cub` 解析設計

### 9.1 読み込み

- 引数は 1 個のみ
- 拡張子が `.cub` でなければエラー
- `open` + `read` で全文を読み込む
- 行分割して `char **lines` を作る

### 9.2 element 解析

対象:

- `NO`
- `SO`
- `WE`
- `EA`
- `F`
- `C`

規則:

- map より前だけで受理する
- 順不同でよい
- 重複はエラー
- 欠落はエラー

### 9.3 map 解析

規則:

- map は file の最後
- map 開始後の空行はエラー
- map 行は見た目どおり保存する
- space も map の一部として保存する

## 10. validate 設計

### 10.1 文字検証

許可文字:

- `0`
- `1`
- `N`
- `S`
- `E`
- `W`
- space

### 10.2 spawn 検証

- spawn はちょうど 1 個
- spawn 文字から player 初期向きを決める

初期方向:

- `N`: `dir = (0, -1)`, `plane = (0.66, 0)`
- `S`: `dir = (0, 1)`, `plane = (-0.66, 0)`
- `E`: `dir = (1, 0)`, `plane = (0, 0.66)`
- `W`: `dir = (-1, 0)`, `plane = (0, -0.66)`

### 10.3 閉包判定

最も壊れにくい方法として、
padding 付き長方形 `grid` を作って flood fill で検証する。

手順:

1. raw row の最大幅を求める
2. 足りない部分を space で埋めた `grid` を作る
3. 外周側または space 連結側から flood fill する
4. `0` または spawn に触れたら open map と判定する

この方法を採用する理由:

- map の形がいびつでも扱いやすい
- space を含む map を正しく弾きやすい
- runtime の `can_move_to()` にも同じ `grid` を流用できる

## 11. 描画設計

### 11.1 基本方針

- 毎 frame、まず背景を塗る
- その後、画面の全列に対して raycast を行う
- 最後に `mlx_put_image_to_window()` する

### 11.2 frame image

- 起動時に 1 枚だけ `frame image` を作る
- 毎 frame その buffer を上書きする
- `mlx_pixel_put()` は使わない

### 11.3 raycast

1 列ごとに次を行う。

1. `camera_x` を求める
2. `ray_dir` を作る
3. DDA で最初の壁に当たるまで進む
4. `perp_wall_dist` を求める
5. `line_height`, `draw_start`, `draw_end` を求める
6. 壁面方向から texture を選ぶ
7. `tex_x` を求めて 1 列描く

### 11.4 texture

- subject 上は path 指定だけだが、実装では `.xpm` に固定する
- texture は `mlx_xpm_file_to_image()` で読む
- 各 texture でも `addr`, `bpp`, `line_len`, `endian` を持つ

## 12. 入力設計

### 12.1 hook

mandatory では次を扱う。

- `KeyPress`
- `KeyRelease`
- `DestroyNotify`
- `Expose`
- `mlx_loop_hook`

Linux 版 MLX では `mlx_key_hook()` が `KeyRelease` に寄るため、
入力は `mlx_hook()` を主に使う。

### 12.2 key state

- `on_key_press` で key state を 1 にする
- `on_key_release` で key state を 0 にする
- `on_loop` で state を見て movement / rotation を更新する

### 12.3 movement

- `W`: 前進
- `S`: 後退
- `A`: 左平行移動
- `D`: 右平行移動
- `Left`: 左回転
- `Right`: 右回転

更新量:

```text
move_step = MOVE_SPEED * delta_sec
rot_step = ROT_SPEED * delta_sec
```

### 12.4 衝突判定

`can_move_to(x, y)` を用意し、
次の cell へ進めるかどうかを 1 箇所で判定する。

拒否対象:

- `1`
- space
- map 範囲外

## 13. エラー処理と cleanup

### 13.1 エラー形式

```text
Error
<message>
```

方針:

- stderr 出力
- deep な関数で `exit()` しない
- 上位へ失敗を返し、最後にまとめて cleanup する

### 13.2 cleanup 順序

1. frame image
2. texture image 4 枚
3. window
4. `mlx_destroy_display(mlx)`
5. `free(mlx)`
6. map / path / lines など heap 領域

各 pointer は `NULL` 初期化し、
部分初期化失敗でも同じ destroy 関数を通せるようにする。

## 14. 想定 file 構成

```text
include/
  cub3d.h

src/
  main.c

  init/
    init_game.c
    init_mlx.c
    init_texture.c
    destroy_game.c

  parse/
    read_file.c
    parse_elements.c
    parse_map.c
    validate_map.c

  render/
    render_frame.c
    raycast.c
    texture_sample.c

  input/
    hook.c
    move.c

  utils/
    error.c
    free.c
    string.c
```

この構成にした理由:

- mandatory に必要な責務は残る
- file 数が過剰にならない
- それでも Norm で関数を逃がせる

## 15. 実装順序

1. T01: file 構成と Makefile
2. `cub3d.h` と構造体定義
3. error / cleanup 基盤
4. `.cub` 読込
5. texture / color / map parse
6. map validate
7. player 初期化
8. MLX window / frame image 初期化
9. texture image 読込
10. 背景描画
11. raycast
12. input / movement
13. Ubuntu VM で検証
14. README / cleanup

## 16. テスト観点

### 16.1 parser / validator

- `.cub` 以外の拡張子
- file open 失敗
- texture 要素不足
- texture 重複
- color format error
- color range error
- invalid char
- spawn 0 個 / 2 個以上
- open map
- map 途中空行

### 16.2 render / input

- window が開く
- 床と天井が塗られる
- 4 方向で texture が変わる
- `WASD` で移動できる
- 左右矢印で回転できる
- `ESC` で終了できる
- close button で終了できる
- minimize / 復帰で破綻しない

### 16.3 最終確認

- Ubuntu VM 上で `make`
- 正常 map で起動
- 異常 map で `"Error\n"` を返す
- `norminette`
- 確認後に `make clean`, `make fclean`

## 17. 最終判断

この設計で固定する判断は次の 5 つである。

1. Linux 専用設計にする
2. `platform` 層は作らない
3. header は `cub3d.h` 1 枚を基本にする
4. `.xpm` と `mlx_hook()` 前提で進める
5. map は `rows` と `grid` の 2 表現を持つ
