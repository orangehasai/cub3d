# cub3D Mandatory Lecture

## 1. このドキュメントの目的

このドキュメントは、`cub3D` の mandatory part を実装するために必要な知識を、MiniLibX を中心に整理したものである。

対象は次の 3 つ。

- MiniLibX をどう使えば mandatory 要件を満たせるか
- raycasting 実装に最低限必要な数学と描画知識
- `.cub` の parser / validator / render / input をどう組み合わせるべきか

参照元:

- `docs/subject.md`
- `docs/design.md`
- `docs/norm.md`
- `minilibx_opengl_20191021/mlx.h`
- `minilibx-linux/README.md`
- `minilibx-linux/man/man3/*.3`
- `minilibx-linux/mlx.h`
- `minilibx-linux` の source code

## 2. mandatory で満たすべきこと

subject の mandatory では、少なくとも次が必要になる。

- `.cub` を読み込み、テクスチャ 4 枚、床色、天井色、map を解釈する
- map が閉じていることを検証する
- プレイヤー初期位置と向きを決める
- 一人称視点の 3D 表現を ray-casting で描画する
- 左右矢印で視点回転、`WASD` で移動する
- `ESC` とウィンドウの close で正常終了する
- window の expose や focus change で破綻しない

つまり mandatory は、単なる MLX のお絵描き課題ではない。

- parser
- validator
- game state
- rendering
- event handling
- cleanup

の 6 つが揃ってはじめて成立する。

## 3. MiniLibX の考え方

### 3.1 MLX は何をしているか

MiniLibX は、backend ごとの差はあっても、共通して
「window を作る」「image を作る」「event loop を回す」
ための薄い API である。

`cub3D` では、mac と Linux の両方で使える共通 subset だけを主に使う。

共通 subset での流れは概ね次になる。

1. `mlx_init()` で MLX backend の利用準備を行う
2. `mlx_new_window()` で window を作る
3. `mlx_new_image()` で memory 上の image buffer を作る
4. image buffer に自前で pixel を書く
5. `mlx_put_image_to_window()` で window に転送する
6. `mlx_hook()` や `mlx_loop_hook()` で input / loop を回す

MLX の重要な発想は次の 2 つ。

- window に直接描くより image に描いて最後に転送する
- 入力は callback と event loop で受け取る

`cub3D` ではこの 2 つが本質になる。

### 3.2 3 種類の主要オブジェクト

MLX を使うときは、次の 3 つを区別して考える。

- `mlx_ptr`
  - MLX backend の context
  - `mlx_init()` の戻り値
- `win_ptr`
  - window 実体
  - `mlx_new_window()` の戻り値
- `img_ptr`
  - image buffer 実体
  - `mlx_new_image()` や `mlx_xpm_file_to_image()` の戻り値

役割が違うので混同しない。

- `mlx_ptr` は全体で 1 個
- `win_ptr` は描画先 window
- `img_ptr` は描画データ

## 4. mac 開発と Linux 最終移行

### 4.1 基本方針

この課題では、次の進め方が現実的である。

- 開発と反復確認は macOS で行う
- 最終移行と最終確認は Linux で行う

ただし、そのためには最初から
「backend 差分を common code に漏らさない」ことが重要である。

### 4.2 何を共通化し、何を分離するか

共通化しやすいもの:

- parser
- validator
- map close 判定
- player state
- raycasting
- texture sampling
- frame image 描画
- game loop の中身

最初から分離すべきもの:

- Makefile の link 設定
- keycode / keysym
- event / mask 定数
- hook 登録方法
- close button の扱い
- `mlx_destroy_display()` の有無

つまり、移行差分は `platform` 層へ閉じ込める。

### 4.3 local mac での確認

local mac では、repo にある mac 向け MLX を使って
window 表示、入力、描画の反復確認を行う想定でよい。

重要なのは backend 名そのものより、common subset だけを使うことである。

- `mlx_init`
- `mlx_new_window`
- `mlx_new_image`
- `mlx_get_data_addr`
- `mlx_put_image_to_window`
- `mlx_xpm_file_to_image`
- `mlx_hook` / `mlx_expose_hook` / `mlx_loop_hook`
- `mlx_loop`

### 4.4 Linux 最終ターゲット

本番は Ubuntu 22.04 であり、最終的な挙動確認は Linux 基準で行う。

### 4.4.1 Linux 版の前提

`minilibx-linux/README.md` では Linux 版に次を要求している。

- X11 include files
- `libxext-dev`
- `libbsd-dev`

リンク時も Linux では X11 系ライブラリが必要になる。

`minilibx-linux/test/Makefile.mk` では次の形になっている。

```make
LFLAGS = -L.. -lmlx -L$(INCLIB) -lXext -lX11 -lm
LFLAGS += -lbsd
```

したがって `cub3D` でも Linux 向けには概ね次を意識する。

```make
-Lminilibx-linux -lmlx -lXext -lX11 -lm -lbsd
```

### 4.4.2 Linux へ移すときに差が出やすい点

今回の本番は Ubuntu 22.04 であり、`mlx` の挙動は macOS 版と完全には同じではない。

特に差が出やすいのは次。

- keycode / keysym
- close button の扱い
- `mlx_key_hook()` の挙動
- `mlx_destroy_display()` の有無

そのため、common code ではなく `platform` 層でこれらを吸収する。

## 5. mandatory で使う MLX API

### 5.1 初期化

```c
void	*mlx_init(void);
```

役割:

- MLX backend の利用準備を行い、以降の API が使う context を返す

注意点:

- `NULL` を返したら以降は何もしてはいけない
- `mlx_ptr` はアプリ全体で使い回す
- Linux では実質的に display 接続であり、mac では backend 側の初期化
  handle と考えるとよい

### 5.2 window

```c
void	*mlx_new_window(void *mlx_ptr, int size_x, int size_y, char *title);
int		mlx_destroy_window(void *mlx_ptr, void *win_ptr);
```

役割:

- 表示用 window の作成と破棄

mandatory で重要な点:

- window は 1 つで十分
- close button による終了を必ず処理する
- expose 後に再描画できるようにする

### 5.3 image

```c
void	*mlx_new_image(void *mlx_ptr, int width, int height);
char	*mlx_get_data_addr(void *img_ptr, int *bpp, int *line_len, int *endian);
int		mlx_put_image_to_window(void *mlx_ptr, void *win_ptr, void *img_ptr,
			int x, int y);
int		mlx_destroy_image(void *mlx_ptr, void *img_ptr);
```

mandatory では最重要 API 群である。

考え方:

- 毎 frame 新しい image を作らない
- 起動時に frame image を 1 枚作る
- その image buffer を毎 frame 上書きする
- 最後に window に転送する

これがいわゆる frame buffer である。

### 5.4 texture 読み込み

```c
void	*mlx_xpm_file_to_image(void *mlx_ptr, char *filename,
			int *width, int *height);
```

役割:

- `.xpm` ファイルを image として読み込む

重要:

- subject は path しか要求していない
- ただし mac / Linux の共通運用を考えると `.xpm` が最も扱いやすい
- MLX の XPM 実装は完全ではない
- 複雑な XPM は読めないことがある

したがって texture は「シンプルな `.xpm`」前提に揃えるのが安全である。

### 5.5 event / loop

```c
int	mlx_loop(void *mlx_ptr);
int	mlx_loop_hook(void *mlx_ptr, int (*funct_ptr)(), void *param);
int	mlx_hook(void *win_ptr, int event, int mask, int (*funct)(), void *param);
int	mlx_expose_hook(void *win_ptr, int (*funct_ptr)(), void *param);
```

役割:

- event 受信
- callback の登録
- main loop 実行

mandatory では `mlx_hook()` が中心になるが、
common code からは `platform_bind_hooks()` のような薄い wrapper で
呼ぶ方が移行差分は少ない。

### 5.5.1 4 つの関数の違い

この 4 つは同じ種類の関数ではない。

- `mlx_loop`
  - event loop 自体を開始する関数
  - callback を登録する関数ではない
  - 普通は最後に 1 回だけ呼ぶ
- `mlx_loop_hook`
  - event がないときに呼ぶ callback を登録する関数
  - 毎 frame 更新処理の置き場として使いやすい
- `mlx_hook`
  - backend の window event に callback を登録する汎用関数
  - Linux では実質的に X11 event を扱う
  - `KeyPress`, `KeyRelease`, `DestroyNotify` などを扱える
- `mlx_expose_hook`
  - `Expose` event 専用の簡易関数
  - 再描画専用の shortcut と考えると分かりやすい

つまり大きく分けると:

- `mlx_loop`
  - ループを回す関数
- `mlx_loop_hook`, `mlx_hook`, `mlx_expose_hook`
  - 呼ばれる関数を登録する関数

### 5.5.2 `cub3D` での使い分け

mandatory の `cub3D` では次の役割分担が自然である。

- `mlx_hook`
  - キー押下
  - キー解放
  - window close
- `mlx_expose_hook`
  - window 再表示時の再描画
- `mlx_loop_hook`
  - 毎 frame の更新と描画
- `mlx_loop`
  - 全登録が終わったあとに loop 開始

backend 差分を隠した典型例:

```c
platform_bind_hooks(&app);
mlx_loop(app.mlx);
```

### 5.5.3 よくある誤解

- `mlx_loop_hook` は「一定周期で必ず呼ばれる timer」ではない
- `mlx_hook` は loop を開始しない
- `mlx_expose_hook` は毎 frame 描画用ではなく、再表示時の再描画用である
- `mlx_loop` を呼ばないと hook 登録だけしても callback は動かない

この 4 つを 1 文で言うなら次になる。

- `mlx_loop` は「回す」
- 残り 3 つは「何が起きたときに何を呼ぶかを登録する」

### 5.6 destroy display

```c
int	mlx_destroy_display(void *mlx_ptr);
```

これは Linux 側で必要になる終了処理差分の代表例である。

ただし重要な点として、`minilibx-linux/mlx_destroy_display.c` を見ると、
この関数は Linux 側で `XCloseDisplay()` を呼ぶだけで
`mlx_ptr` 自体は `free` しない。

つまり Linux では終了処理で次を行う必要がある。

1. image を destroy
2. window を destroy
3. `mlx_destroy_display(mlx_ptr)`
4. `free(mlx_ptr)`

この差分も common code に直書きせず、
`platform_destroy_mlx()` のような関数へ隔離する方が安全である。

## 6. window に直接描くな、image に描け

### 6.1 `mlx_pixel_put()` を主戦力にしない理由

man page 上は `mlx_pixel_put()` で window に 1 pixel ずつ描ける。

しかし `cub3D` では不向きである。

- 画面全体を毎 frame 描く
- 壁 1 本ごとに多数の pixel を描く
- FPS が必要

この状況で window へ直接 pixel を打つと非常に遅い。

そのため `cub3D` では次の方針を取る。

- `mlx_new_image()` で frame image を作る
- `mlx_get_data_addr()` で raw buffer を得る
- buffer に自前で pixel を書き込む
- 1 frame ごとに `mlx_put_image_to_window()` で転送する

### 6.2 image buffer の意味

`mlx_get_data_addr()` で得られる値:

- `bpp`
  - 1 pixel あたり何 bit か
- `line_len`
  - 1 行あたり何 byte か
- `endian`
  - byte order
- `addr`
  - image memory の先頭

画面上の `(x, y)` に対応する address は概ね次で計算する。

```c
pixel = addr + y * line_len + x * (bpp / 8);
```

これは 2D の画像を 1 次元のメモリに行単位で並べているためである。
意味は「先頭 address + y 行分 + x pixel 分」である。

ここで大事なのは、`line_len` が必ずしも `width * 4` ではないこと。
必ず `line_len` を使う。

### 6.3 色表現

`mlx_pixel_put` の man page では色は `0x00RRGGBB` とされている。

例:

- `0x00FF0000` red
- `0x0000FF00` green
- `0x000000FF` blue

床色 / 天井色は parser で `R,G,B` を読んだら、
`(r << 16) | (g << 8) | b` に変換して持っておくと便利である。

### 6.4 expose の再描画

window が他 window に隠れた後や、再表示された後には expose が飛ぶ。
このとき何もしないと表示が崩れたり真っ黒に見えることがある。

mandatory では次が安全。

- 最新 frame image を保持する
- expose hook で `mlx_put_image_to_window()` を呼び直す

## 7. platform 差分で特に重要な点

### 7.1 `mlx_key_hook()` は Linux では KeyPress ではない

これは非常に重要である。

`minilibx-linux/mlx_key_hook.c` を見ると、Linux 版の `mlx_key_hook()` は
`KeyRelease` に hook している。

つまり Linux 版では、次の期待は外れる。

- `mlx_key_hook()` で押下中移動を作る
- `mlx_key_hook()` で KeyPress を取りたい

したがって `cub3D` では、
common code から `mlx_key_hook()` を直接使わない方が安全である。
backend 側で `mlx_hook()` を使って `KeyPress` と `KeyRelease` を明示し、
project 側のキー定数へ変換する。

推奨:

```c
platform_bind_hooks(&app);
```

### 7.2 close button は `DestroyNotify` hook を使う

Linux MLX の `mlx_loop.c` を見ると、WM の close button は内部で
`DestroyNotify` hook に流している。

Linux backend 側では 42 でよく次のように扱う。

```c
mlx_hook(win, DestroyNotify, 0, on_destroy, &app);
```

`StructureNotifyMask` を与える流儀もあるが、Linux MLX の実装上は
`hook[DestroyNotify]` に callback が入っていることが本質である。

### 7.3 `mlx_loop_hook()` は固定 FPS ではない

`mlx_loop_hook()` は「イベントがないときに呼ばれる」関数であって、
一定周期保証ではない。

つまり:

- フレームレート制御 API ではない
- busy loop になりうる

そのため移動量は frame count ではなく time delta で決めるべきである。

例:

```text
move_step = MOVE_SPEED * delta_sec
rot_step = ROT_SPEED * delta_sec
```

### 7.4 `endian` を無視しすぎない

多くの Linux 環境では little endian なので、そのまま動いて見えることが多い。
しかし設計上は `bpp`, `line_len`, `endian` を正しく受け取るべきである。

mandatory では通常の `0x00RRGGBB` をそのまま 32bit 書き込みして十分な
ことが多いが、少なくとも `line_len` と `bpp` は絶対に固定値扱いしない。

## 8. mandatory で推奨される event 設計

### 8.1 callback 署名

MLX の hook では callback の実引数は event により異なる。

common code が扱う callback は、backend 生値ではなく
project 側で正規化した key / state を受け取る前提にすると管理しやすい。

主に使うもの:

```c
int	on_key_press(int key, t_app *app);
int	on_key_release(int key, t_app *app);
int	on_destroy(t_app *app);
int	on_expose(t_app *app);
int	on_loop(t_app *app);
```

実際の prototype は `int (*)()` 扱いだが、実装側では上記のように
受ける想定でよい。

### 8.2 キー状態を持つ

押した瞬間に 1 回だけ移動する実装より、キー状態を保持する実装の方が
`cub3D` に向いている。

例:

- `on_key_press()`
  - `app->keys.w = 1`
  - `app->keys.left = 1`
- `on_key_release()`
  - `app->keys.w = 0`
  - `app->keys.left = 0`
- `on_loop()`
  - `keys` を見て移動 / 回転を更新
  - その後に描画

この方式にすると:

- 長押しに強い
- 環境依存の key repeat に依存しない
- 移動速度を delta time ベースにできる

### 8.3 backend 生定数を raw number で書かない

common code では backend 生定数を使わず、
project 側の `KEY_*` 定数を使う。

代表例:

- `KEY_ESC`
- `KEY_LEFT`
- `KEY_RIGHT`
- `KEY_W`, `KEY_A`, `KEY_S`, `KEY_D`

Linux backend では内部で `X11/keysym.h` の値をこれへ対応づける。
mac backend では mac 側の keycode を同じ `KEY_*` へ対応づける。

## 9. texture の扱い

### 9.1 texture も image

`mlx_xpm_file_to_image()` で読み込んだ texture も、内部的には image として扱う。
したがって壁 texture に対しても `mlx_get_data_addr()` を呼ぶ。

保持したい情報:

- `img_ptr`
- `addr`
- `width`
- `height`
- `bpp`
- `line_len`
- `endian`

これにより、raycast 中に texture から直接色をサンプリングできる。

### 9.2 texture は起動時に 1 回だけロードする

毎 frame 読み込むのは論外である。

起動時:

1. parser が path を得る
2. MLX 初期化後に texture を 4 枚ロードする
3. `addr` まで取得する

loop 中:

- 必要な pixel を参照するだけ

### 9.3 北南東西の対応

mandatory では、当たった壁面によって texture を切り替える必要がある。

つまり raycast 結果から最低限わかるべきことは次。

- x 側面に当たったか y 側面に当たったか
- ray の進行方向は正か負か

これで north / south / east / west を選べる。

## 10. raycasting に必要な知識

ここからは MLX ではなく `cub3D` 本体に必要な知識である。

### 10.1 2D map と 3D 表示の関係

まず最初に重要なのは、`cub3D` は本物の 3D 空間を持っていないという点である。
内部的には、ただの 2D map である。

- `1` は wall
- `0` は empty
- `NSEW` は player spawn

上から見たイメージはこうである。

```text
111111
100001
10N001
100001
111111
```

これは「上から見た迷路」にすぎない。

では、なぜ 3D に見えるのか。

答えは、画面の縦 1 列ごとに
「その方向には何が見えるか」を調べて描いているからである。

3D に見えていても、実際にしていることは次である。

- 画面の縦列ごとに 1 本 ray を飛ばす
- その ray が最初に当たる wall を探す
- 壁までの距離に応じて縦線の高さを決める
- その縦線に texture を貼る

つまり `cub3D` の画面は「縦線をたくさん並べて描いたもの」である。

### 10.2 1 列ずつ描くとはどういうことか

たとえば画面幅が 640 pixel なら、考え方としては次になる。

1. 0 列目用の ray を飛ばす
2. 1 列目用の ray を飛ばす
3. 2 列目用の ray を飛ばす
4. ...
5. 639 列目用の ray を飛ばす

ここでの列番号は、画面の左から右へ増える。

- 0 列目
  - 左端
- 319 列目や 320 列目付近
  - 中央付近
- 639 列目
  - 右端

各列は「その画面位置の方向に見える壁 1 本分」を担当している。

ここで重要なのは、ray 自体が縦に飛んでいるわけではないことだ。
縦なのはあくまで「画面上の pixel 列」である。

ray は map 上では、player の位置から扇状に広がる。

イメージ:

```text
画面上の列

0 1 2 3 4 5 6
| | | | | | |

対応する ray の向き

\ \ \ | / / /
 \ \ \|/ / /
    player
```

つまり:

- 画面には縦の列がある
- 各列に対応して 1 本 ray を飛ばす
- その ray は map 上では斜めも含めてさまざまな向きになる

したがって、画面の左側の列ほど「視界の左側」を担当し、
中央は正面、画面の右側の列ほど「視界の右側」を担当する。

ここでいう「左」「右」は world 座標の `x` 軸のことではない。
プレイヤーから見た相対的な左 / 右である。

### 10.3 なぜ壁は縦線になるのか

mandatory では壁を「床から天井まで立っている板」とみなす。

そのため 1 本の ray が壁に当たったとき、
その列に必要なのは「どれくらい高く見えるか」だけである。

- 壁が近い
  - 画面では高く見える
- 壁が遠い
  - 画面では低く見える

つまり正確には、

- `1 本の ray`
  - map 上で壁との交点を調べるための線
- `1 本の縦線`
  - その ray の結果を画面に描くための壁スライス

という対応になる。

### 10.4 player の持つべき状態

最低限必要なのは次。

- position `x, y`
- direction `dir_x, dir_y`
- camera plane `plane_x, plane_y`

`x, y` は map 上の現在地である。

`dir` は「真正面を向いている矢印」である。

例:

- 東向き
  - `dir = (1, 0)`
- 北向き
  - `dir = (0, -1)`

`plane` は最初は少し分かりづらいが、
「画面の左右の広がりを作る横向きベクトル」と考えるとよい。

- `dir`
  - 正面方向
- `plane`
  - 視野の横幅

例:

- 北向き
  - `dir = (0, -1)`
  - `plane = (0.66, 0)`

ここでの `0.66` は視野の広さを決める係数である。

### 10.5 1 本の ray はどう作るのか

画面の列 `x` を 1 本選んだら、その列専用の ray の向きを作る。

概念的にはこうである。

- 画面左端
  - 正面より視界の左側を向く ray
- 画面中央
  - ほぼ `dir` そのまま
- 画面右端
  - 正面より視界の右側を向く ray

式としては次がよく使われる。

```c
camera_x = 2 * x / screen_width - 1;
ray_dir = dir + plane * camera_x;
```

意味は単純である。

- `camera_x = -1`
  - 左端
- `camera_x = 0`
  - 真ん中
- `camera_x = 1`
  - 右端

そして `plane * camera_x` によって、
`dir` を「プレイヤー視点で見た左 / 右」へ少しずつずらしている。

たとえば player が北を向いているなら、視界の左側 ray には負の `x`
成分が入り、右側 ray には正の `x` 成分が入る。
逆に player が東を向いているなら、左右の違いは主に `y` 成分へ現れる。
つまり、どの成分が変わるかは player の向き次第であり、
常に `x` 成分で左右が決まるわけではない。

### 10.6 1 本の ray で知りたいこと

1 本の列について最終的に知りたいのは次の 3 つだけである。

1. 最初にどの壁へ当たるか
2. その壁までの距離はいくつか
3. 壁のどの位置に当たったか

これが分かれば:

- 距離から壁の高さを決められる
- 当たった位置から texture のどの列を使うか決められる

### 10.7 DDA

DDA は、ray がどの map cell を順に通過するかを効率よく追う手法である。

初心者向けに言い換えると、
「ray をちまちま少しずつ進める代わりに、
次のマス境界まで一気に進める方法」である。

素朴にやると:

- ray を 0.001 ずつ進める
- そのたびに壁かどうか調べる

となるが、これは遅い。

そこで DDA では毎回、

- 次の縦線に先に当たるか
- 次の横線に先に当たるか

を比べる。

小さい方へ 1 マス進める、を繰り返せばよい。

### 10.8 DDA を俯瞰図で見る

```text
+---+---+---+---+
|   |   |   | 1 |
+---+---+---+---+
|   | P |   | 1 |
+---+---+---+---+
|   |   |   | 1 |
+---+---+---+---+
```

- `P` が player
- 右上方向へ ray を飛ばす

この ray は、細かい点を打ちながら進むのではなく、
「次に x 境界へ当たるか」「次に y 境界へ当たるか」を比べながら
cell から cell へ進んでいく。

そして `1` の cell に入った瞬間に、
「この列はここで壁に当たった」と判断する。

### 10.9 DDA でよく出る変数の意味

各列でよく使う変数は次である。

- `map_x`, `map_y`
  - 今 ray がいる map cell
- `step_x`, `step_y`
  - x / y のどちら向きへ進むか
- `delta_dist_x`, `delta_dist_y`
  - x 境界 / y 境界を 1 つ越えるたびに増える距離
- `side_dist_x`, `side_dist_y`
  - 次の x 境界 / y 境界まであとどれくらいか

流れはこうである。

1. `camera_x` を求める
2. `ray_dir` を求める
3. `delta_dist_x`, `delta_dist_y` を求める
4. `step_x`, `step_y`, `side_dist_x`, `side_dist_y` を初期化
5. x 方向か y 方向の近い方へ 1 cell ずつ進める
6. `1` に当たったら停止

停止後に:

- 垂直距離 `perp_wall_dist`
- 壁線の高さ
- texture の x 座標

を求める。

### 10.10 壁の高さはどう決まるのか

壁までの距離が求まったら、画面に描く高さを決める。

発想は単純である。

- 近い壁は大きく見える
- 遠い壁は小さく見える

そのため、壁線の高さは距離に反比例させる。

```c
line_height = screen_height / perp_wall_dist;
```

ここで `perp_wall_dist` を使うのは、
視野の端で壁が不自然に膨らんで見えるのを防ぐためである。

### 10.11 画面のどこからどこまで壁を描くのか

壁線の高さが決まったら、その縦線の開始位置と終了位置を決める。

考え方:

- 画面中央を基準に上下へ伸ばす

たとえば:

```c
draw_start = -line_height / 2 + screen_height / 2;
draw_end = line_height / 2 + screen_height / 2;
```

これで「中央に立っている壁」に見える。

### 10.12 texture はどう貼るのか

壁に当たったとき、その列に必要なのは texture 全体ではない。
必要なのは「texture の 1 列分」である。

なぜなら画面側も 1 列ずつ描いているからである。

必要なのは次である。

- 壁のどの位置に当たったか
  - `wall_x`
- それを texture 幅へ変換したもの
  - `tex_x`

つまり:

- ray が壁の左寄りに当たった
  - texture の左寄りの列を使う
- ray が壁の中央付近に当たった
  - texture の中央付近の列を使う

そして画面の縦線を上から下へ塗る間に、
texture 側も上から下へ対応する pixel を取ってくる。

### 10.13 1 列の処理をまとめる

1 列の処理を初心者向けに言い換えると次になる。

1. この列は画面のどの向きか決める
2. その向きに ray を飛ばす
3. DDA で最初の壁を探す
4. 壁までの距離を求める
5. 距離から壁の見た目の高さを決める
6. 壁のどの位置に当たったかから texture 列を決める
7. その列を画面に描く

これを全列について繰り返すと、3D に見える画面になる。

### 10.14 floor / ceiling

mandatory では床と天井は単色でよい。

したがって毎 frame 最初に:

- 上半分を ceiling color
- 下半分を floor color

で塗りつぶせばよい。

その上から壁列を描く。

### 10.15 最初に理解すべき最小セット

raycasting を最初から完璧に理解する必要はない。
まずは次の 4 点が分かれば十分である。

1. 世界は本当は 2D map である
2. 画面は縦 1 列ずつ描いている
3. 各列は 1 本の ray に対応している
4. 壁までの距離で列の高さが決まる

そのうえで DDA と texture 貼りを順に理解すればよい。

## 11. parser / validator に必要な知識

### 11.1 parser は MLX より先

`.cub` が壊れているだけで window を開く必要はない。
順序は必ず次にする。

1. 引数確認
2. `.cub` 読込
3. texture path / color / map parse
4. map validate
5. player 初期化
6. その後に MLX 初期化

### 11.2 map は「見た目通り」に扱う

subject では space も map の有効な一部とされている。
したがって次が重要になる。

- 行長が不揃いであり得る
- 末尾 space を勝手に落とさない
- map 開始後の空行はエラー扱いが安全

### 11.3 閉包判定

map が閉じているかの判定は mandatory の本質である。

実装しやすい方法:

1. 最長行に合わせて長方形 buffer を作る
2. 足りない場所を space で埋める
3. 外周に 1 マス余白を作る
4. 外側から flood fill する
5. `0` または spawn に接触したら open map と判定

## 12. 描画ループの完成形

mandatory の loop は概ね次の流れになる。

1. `gettimeofday()` で delta time を取る
2. key state から player を更新する
3. frame image の床 / 天井を塗る
4. 各 x 列について raycast する
5. 壁 texture を frame image に書く
6. `mlx_put_image_to_window()` で表示する

この処理は `mlx_loop_hook()` の callback に置くのが自然である。

## 13. 最小構成のコードイメージ

```c
#include "cub3d.h"
#include "platform.h"

static int	on_key_press(int keysym, t_app *app)
{
	if (keysym == KEY_ESC)
		return (app_close(app));
	if (keysym == KEY_W)
		app->keys.w = 1;
	if (keysym == KEY_S)
		app->keys.s = 1;
	if (keysym == KEY_A)
		app->keys.a = 1;
	if (keysym == KEY_D)
		app->keys.d = 1;
	if (keysym == KEY_LEFT)
		app->keys.left = 1;
	if (keysym == KEY_RIGHT)
		app->keys.right = 1;
	return (0);
}

static int	on_key_release(int keysym, t_app *app)
{
	if (keysym == KEY_W)
		app->keys.w = 0;
	if (keysym == KEY_S)
		app->keys.s = 0;
	if (keysym == KEY_A)
		app->keys.a = 0;
	if (keysym == KEY_D)
		app->keys.d = 0;
	if (keysym == KEY_LEFT)
		app->keys.left = 0;
	if (keysym == KEY_RIGHT)
		app->keys.right = 0;
	return (0);
}

static int	on_destroy(t_app *app)
{
	return (app_close(app));
}

int	main(int argc, char **argv)
{
	t_app	app;

	if (app_init(&app, argc, argv) != 0)
		return (1);
	platform_bind_hooks(&app);
	mlx_loop(app.mlx);
	return (0);
}
```

これはあくまでイメージであり、Norm 対応のため実際はより細かく分割する。

## 14. mandatory 実装でよくある失敗

### 14.1 `mlx_key_hook()` を使ってしまう

Linux 版では KeyRelease に紐づくため、移行時に押下中移動が崩れる。

### 14.2 `mlx_pixel_put()` で全画面を描く

遅い。frame image を使う。

### 14.3 `line_len` を無視する

texture や frame のメモリアクセスが壊れる。

### 14.4 `mlx_ptr` を free し忘れる

Linux では `mlx_destroy_display()` の後に `free(mlx_ptr)` が必要。

### 14.9 backend 差分を common code に直書きする

最初は動いても、mac -> Linux 移行時の変更量が一気に増える。

### 14.5 texture を毎 frame 読み込む

起動時 1 回だけ読む。

### 14.6 map の space を雑に trim する

subject 違反になりやすい。

### 14.7 close button を未処理にする

mandatory 不足になる。

### 14.8 expose を無視する

window 管理が不安定に見える。

## 15. 実装順のおすすめ

理解コストが低い順に進めるなら次がよい。

1. `.cub` parser と validator を完成させる
2. platform 境界を作る
3. mac で MLX 初期化と window 表示だけ通す
4. frame image を単色で表示する
5. `WASD` / 矢印 / `ESC` / close を通す
6. 壁線だけの raycast を描く
7. texture を貼る
8. Linux backend へ切り替えて最終確認する
9. cleanup と異常系を詰める

## 16. この課題で本当に理解しておくべきこと

最終的に説明できるべき内容は次である。

- `mlx_ptr`, `win_ptr`, `img_ptr` の違い
- `mlx_get_data_addr()` が返す `bpp`, `line_len`, `endian` の意味
- なぜ image buffer に描いてから window に転送するのか
- なぜ backend 差分を platform 層へ隔離するのか
- ray を 1 列ごとに飛ばすと 3D に見える理由
- map が閉じていることをどう検証するか
- どのタイミングで何を解放するか

この 6 点を自分の言葉で説明できれば、mandatory 実装に必要な理解としては十分強い。
