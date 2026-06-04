# cub3D 設計

## 1. 目的

この設計書は `docs/subject.md` の mandatory part を実装するための土台を定義する。

主目的は次の 3 点。

1. `.cub` を安全に読み取り、設定とマップを正しく検証すること。
2. MiniLibX 上で安定してレイキャスト描画を行うこと。
3. Norm を崩さずに、責務分離された構成で実装できること。

本設計は mandatory を主対象とし、bonus は後付けしやすい構造だけ用意する。

## 2. スコープ

mandatory で扱う機能:

- `.cub` ファイルの読込
- テクスチャ 4 方向の設定
- 床色 / 天井色の設定
- マップ検証
- プレイヤー初期位置 / 向きの確定
- レイキャストによる壁描画
- キー入力による移動 / 回転
- `ESC` / ウィンドウ close による正常終了

mandatory では扱わない機能:

- minimap
- door
- sprite
- mouse rotation
- HUD

## 3. 設計方針

- グローバル変数は使わない。
- `parse`, `validate`, `init`, `render`, `input`, `shutdown` を分離する。
- `.cub` は「読み込み」と「意味解釈」を分ける。まず生の行配列を作り、その後に識別子解析と map 解析を行う。
- map は「見た目どおりに保持」し、検証時だけ長方形に正規化する。
- 描画は毎フレーム off-screen image に描いてから `mlx_put_image_to_window` する。
- エラー時は必ず `"Error\n"` と明示的メッセージを返し、途中まで確保した資源も解放する。
- Linux 本番を基準に設計する。ローカル macOS は補助環境と位置付ける。

### 3.1 Norm 反映方針

- すべての関数は 25 行以内を前提に分割する。
- 1 関数あたりのローカル変数は 5 個以内を前提にする。
- 1 関数あたりの引数は 4 個以内を前提にする。
- `for`, `do...while`, `switch`, `case`, 三項演算子は使わない。
- 1 `.c` ファイルあたりの関数定義は 5 個以内に収める。
- `struct` / `typedef` / `enum` の定義はすべて header に置き、`.c` では宣言しない。
- 関数内コメントに依存しない。必要なコメントは関数の外に英語で最小限だけ置く。
- 変数宣言は関数先頭に寄せ、1 行 1 変数、宣言と初期化を分離する前提で設計する。
- マクロは定数用途だけに限定し、ロジックの隠蔽には使わない。
- すべての `.c` / `.h` は 42 header、適切な include guard、未使用 include 禁止を前提にする。

## 4. 全体構成

### 4.1 モジュール

- `app`
  - 全体状態の保持
  - 初期化、メインループ、終了処理
- `parse`
  - ファイル読込
  - 識別子ごとの値抽出
  - raw map 行の収集
- `validate`
  - 設定値の重複 / 欠落確認
  - 色範囲確認
  - map 文字集合確認
  - spawn 数確認
  - map 閉包判定
- `render`
  - frame buffer の塗りつぶし
  - レイキャスト
  - テクスチャサンプリング
- `input`
  - キー状態管理
  - 移動 / 回転の更新
- `util`
  - エラー出力
  - 解放補助
  - 小さな共通関数

### 4.2 依存方向

依存は次の一方向を基本とする。

```text
main
  -> app
    -> parse
    -> validate
    -> render
    -> input
    -> util
```

`render` が `parse` に依存しないように、実行時に必要な値は `scene` / `player` / `app` に集約する。

### 4.3 Norm 制約からの分割戦略

- parser は「ファイル読込」「行走査」「識別子解釈」「map 収集」に分割する。
- validator は「文字検証」「spawn 検証」「閉包判定」に分割する。
- raycast は「ray 初期化」「DDA 前進」「当たり判定後の計算」「texture 選択」「列描画」に分割する。
- input 更新は「delta time 更新」「前後移動」「左右移動」「回転」に分割する。
- ローカル変数数を抑えるため、中間状態は専用構造体に退避する。

## 5. データ構造

### 5.1 設定系

```c
typedef struct s_rgb
{
	int	r;
	int	g;
	int	b;
	int	value;
}	t_rgb;

typedef struct s_texture
{
	char	*path;
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_texture;

typedef struct s_map
{
	char	**rows;
	int		width;
	int		height;
}	t_map;

typedef struct s_scene
{
	t_texture	north;
	t_texture	south;
	t_texture	west;
	t_texture	east;
	t_rgb		floor;
	t_rgb		ceiling;
	t_map		map;
}	t_scene;

typedef struct s_parse_ctx
{
	char	**lines;
	int		index;
	int		map_started;
	int		map_rows;
}	t_parse_ctx;
```

設計意図:

- `t_texture` は path と MLX image 実体を両方持つ。parse 完了時は path だけが埋まり、MLX 初期化後に image をロードする。
- `t_map.rows` は file の見た目を保った raw row を保持する。
- `t_rgb.value` は `0xRRGGBB` に変換した値を持たせ、描画時の再計算を避ける。
- `t_parse_ctx` は parser の走査状態を 1 つに束ね、引数数とローカル変数数を抑える。

### 5.2 プレイヤー / 実行時状態

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

typedef struct s_image
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}	t_image;

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

typedef struct s_ray
{
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
}	t_ray;

typedef struct s_draw
{
	int			line_height;
	int			draw_start;
	int			draw_end;
	int			tex_x;
	double		tex_pos;
	double		tex_step;
	t_texture	*texture;
}	t_draw;

typedef struct s_app
{
	void		*mlx;
	void		*win;
	t_image		frame;
	t_scene		scene;
	t_player	player;
	t_keys		keys;
	t_time		time;
	int			screen_width;
	int			screen_height;
	int			initialized_window;
}	t_app;
```

設計意図:

- `player` は map cell 座標ではなく実数座標を持つ。移動と DDA の両方で扱いやすい。
- 視線方向は `dir_*`、視野面は `plane_*` で持つ。回転は 2D 回転行列で更新する。
- `keys` は press / release を状態化し、`loop_hook` で移動量に変換する。
- `t_time` は delta time 計算を専用化し、移動更新関数の責務を軽くする。
- `t_ray` は 1 列分の DDA 状態をまとめ、raycast 関数の変数数超過を防ぐ。
- `t_draw` は 描画区間と texture 参照をまとめ、列描画処理を分割しやすくする。

## 6. 初期化シーケンス

```text
main
  -> 引数検証
  -> app 構造体 zero initialize
  -> .cub 読込
  -> scene parse
  -> scene validate
  -> player 初期化
  -> mlx 初期化
  -> window 作成
  -> frame image 作成
  -> texture image 読込
  -> hook 登録
  -> loop 開始
```

ポイント:

- parse / validate を MLX 初期化前に済ませる。設定ファイルが壊れているだけでウィンドウを開かない。
- texture path の存在確認は parse / validate で `open()` により行う。
- 実際の画像ロードは MLX 初期化後に行う。

## 7. `.cub` パース設計

### 7.1 入口

- 引数は 1 個のみ受け取る。
- 拡張子が `.cub` でなければ即エラー。
- `open` + `read` でファイル全体を読み込む。
- 行分割して `char **lines` を得る。

`get_next_line` 前提にはしない。subject の許可関数だけで完結させる。

### 7.2 解析ルール

解析対象は 2 種類:

- 識別子行
  - `NO`, `SO`, `WE`, `EA`, `F`, `C`
- map 行
  - 文字集合が ` 01NSEW` のみで構成される行

走査ルール:

1. 空行は map 開始前なら無視してよい。
2. 識別子は map 開始前のみ受理する。
3. 最初の map 行を見つけたら map section 開始とみなす。
4. map 開始後に識別子行が来たらエラー。
5. map 開始後に空行が来たらエラーとする。
6. EOF までの map 行をそのまま保存する。

この方針にする理由:

- subject で map は最後とされているため。
- map 内の空行を許すと、閉包判定や見た目どおり解釈が曖昧になるため。

### 7.3 識別子ごとの処理

Norm 対応のため、parser は次のように細かく分ける。

- `read_cub_file`
- `split_lines`
- `scan_scene_lines`
- `parse_identifier_line`
- `append_map_row`

各関数は `t_parse_ctx` と `t_scene` を受け取り、引数 4 個以内を維持する。

#### texture

- `NO`, `SO`, `WE`, `EA` は 1 回ずつ必須。
- 値は path 文字列。
- 前後の空白を除去したうえで、空文字列はエラー。
- `open(path, O_RDONLY)` できない場合はエラー。

#### color

- `F`, `C` は 1 回ずつ必須。
- 形式は `R,G,B`。
- 各値は 0 から 255 の整数。
- 余計なカンマ、空要素、文字混入はエラー。
- parse 後に `value = (r << 16) | (g << 8) | b` を計算する。

### 7.4 map 保存

- raw 行をそのまま `scene.map.rows` に保持する。
- `scene.map.width` は最長行長。
- `scene.map.height` は行数。
- 短い行は保存時に埋めず、検証用の別バッファ作成時に space 埋めする。

## 8. map 検証設計

### 8.1 文字検証

許可文字:

- `1`: wall
- `0`: empty
- `N`, `S`, `E`, `W`: spawn
- ` `: void / padding

不許可文字が 1 つでもあればエラー。

### 8.2 spawn 検証

- spawn はちょうど 1 個必須。
- 見つけたら `player.x = col + 0.5`, `player.y = row + 0.5` とする。
- 向きに応じて `dir` / `plane` を初期化する。

初期値:

- `N`: `dir=(0,-1)`, `plane=(0.66,0)`
- `S`: `dir=(0,1)`, `plane=(-0.66,0)`
- `E`: `dir=(1,0)`, `plane=(0,0.66)`
- `W`: `dir=(-1,0)`, `plane=(0,-0.66)`

`0.66` は Wolfenstein 系で一般的な視野角設定を想定した値とする。

### 8.3 閉包判定

検証用に長方形 map を作る。

```text
height + 2
width  + 2
```

ルール:

- 外周はすべて space で埋める。
- 元 map は 1 マス内側にコピーする。
- 短い行の足りない部分も space で埋める。

判定方法:

1. 外側 `(0, 0)` から space 領域を flood fill する。
2. flood fill 中、隣接先に `0` または spawn が見えたら map は開いているとみなしてエラー。
3. すべて探索して問題なければ閉じている。

この方法の利点:

- 行長が不揃いでも扱いやすい。
- subject が「space も map の一部」としている点に自然に対応できる。

Norm 対応のため、閉包判定も次の関数に分割する。

- `build_closed_grid`
- `copy_map_to_grid`
- `flood_void`
- `check_map_closed`

`flood_void` は再帰または小さな helper 群で実装し、1 関数内に判定ロジックを詰め込まない。

## 9. 描画設計

### 9.1 描画単位

毎フレーム:

1. frame image 全体の上半分を ceiling 色で塗る。
2. 下半分を floor 色で塗る。
3. 各 x 列に対して raycast を行う。
4. 壁ストライプを texture 付きで上書きする。
5. `mlx_put_image_to_window()` で表示する。

### 9.2 レイキャスト

各列 `x` で次を行う。

1. `camera_x = 2 * x / screen_width - 1`
2. `ray_dir = dir + plane * camera_x`
3. `map_x = (int)player.x`, `map_y = (int)player.y`
4. `delta_dist_x`, `delta_dist_y` を計算
5. `step_x`, `step_y`, `side_dist_x`, `side_dist_y` を計算
6. DDA で `1` に当たるまで進める
7. hit 面が x side か y side かを記録
8. `perp_wall_dist` を求めて fish-eye を防ぐ
9. `line_height = screen_height / perp_wall_dist`
10. 描画開始 / 終了 y を算出
11. 壁の向きに応じた texture を選択
12. texture x 座標と pixel step を計算して描画

Norm 対応のため、1 列の描画は次の単位まで分割する。

- `init_ray`
- `step_ray_until_hit`
- `finish_ray`
- `select_wall_texture`
- `draw_wall_column`

`t_ray` と `t_draw` を使い、各関数のローカル変数を 5 個以内に抑える。

### 9.3 texture 選択

判定条件:

- x side に当たった
  - `ray_dir_x > 0` なら west 面
  - `ray_dir_x < 0` なら east 面
- y side に当たった
  - `ray_dir_y > 0` なら north 面
  - `ray_dir_y < 0` なら south 面

注記:

- 実装時は DDA の `step_x`, `step_y` を使って最終判定した方がバグりにくい。
- 設計上は「どの面にぶつかったか」で texture を切り替えることが本質。

### 9.4 texture sampling

- `wall_x` で当たり面の小数位置を求める。
- `tex_x` を texture 幅に射影する。
- 列描画中は `tex_pos` を加算しながら `tex_y` を更新する。
- 1 pixel 書き込みは frame image の address に直接行う。

この方針により `mlx_pixel_put` は使わない。

## 10. 入力設計

### 10.1 イベント

- `KeyPress`
- `KeyRelease`
- `DestroyNotify`
- `LoopHook`

Linux 前提ならキー定数は数値直書きせず、可能なら `X11/keysym.h` を使う。

### 10.2 移動

毎フレーム、押下中キーから移動量を計算する。

- `W`: 前進
- `S`: 後退
- `A`: 左平行移動
- `D`: 右平行移動
- `LEFT`: 左回転
- `RIGHT`: 右回転

速度は固定値ではなく delta time 依存にする。

```text
move_step = MOVE_SPEED * delta_sec
rot_step  = ROT_SPEED * delta_sec
```

これにより環境差で移動速度が極端に変わるのを防ぐ。

Norm 対応のため、入力更新は次の関数に分割する。

- `update_delta_time`
- `move_forward_backward`
- `move_left_right`
- `rotate_left_right`
- `update_player`

### 10.3 衝突判定

ここは subject 上やや解釈の余地がある。

- bonus list に `Wall collisions` がある。
- ただし、迷路内を移動する mandatory 実装としては壁侵入禁止の方が自然。

暫定方針:

- `can_move_to(x, y)` を独立関数にする。
- 初期設計では `1` と space への侵入を禁止する。
- もし strict に bonus と分離したくなった場合、この関数差し替えで対応できるようにする。

この点は実装前に最終決定したい。

## 11. 画像ロード設計

- Linux MiniLibX の安定性を優先し、初期実装では `mlx_xpm_file_to_image` を使う前提とする。
- path は subject 上汎用だが、実運用上は `.xpm` を採用するのが安全。
- texture サイズは正方形である必要はないが、初期実装では同一サイズ想定の方が単純。
- keycode や画面サイズ、移動速度のような固定値は大文字マクロまたは enum に寄せ、ロジックをマクロ化しない。

ここも設計上の実装方針であり、subject 自体は path 形式しか制約していない。

## 12. エラー処理と終了処理

### 12.1 エラー出力

統一形式:

```text
Error
<explicit message>
```

方針:

- stderr に出す。
- deep な関数で `exit()` しない。
- `int` 戻り値で失敗を上位に返し、最後に cleanup して終了する。

### 12.2 cleanup 順序

逆順で解放する。

1. frame image
2. texture images
3. window
4. display / mlx context
5. map rows
6. texture path
7. その他動的確保領域

部分初期化失敗に備え、各ポインタは `NULL` 初期化する。

## 13. 想定ファイル構成

```text
include/
  cub3d.h
  config.h
  types.h
  app.h
  parse.h
  validate.h
  render.h
  input.h
  error.h

src/
  main.c

  app/
    init.c
    run.c
    destroy.c

  parse/
    read_file.c
    split_lines.c
    scan_scene.c
    parse_scene.c
    parse_texture.c
    parse_color.c
    parse_map.c

  validate/
    validate_scene.c
    validate_map.c
    build_grid.c
    flood_fill.c

  render/
    frame.c
    ray_init.c
    ray_step.c
    ray_finish.c
    texture.c
    draw_column.c

  input/
    key_hook.c
    key_state.c
    update_time.c
    loop_update.c
    move.c
    rotate.c

  util/
    error.c
    free.c
    string.c
```

補足:

- `cub3d.h` は共通型と公開 API を最小限だけ集約する。
- `types.h` にすべての `struct` / `typedef` を集約し、`.c` で型宣言しない。
- `config.h` には定数マクロだけを置く。
- header の相互依存を避けるため、必要に応じて前方宣言を使う。
- 各 `.c` は最大 5 関数までを上限にする。
- file 数は多いが、Norm の関数長制限と変数数制限を守りやすい。

## 14. 実装順序

1. 共通構造体とヘッダ作成
2. エラー / cleanup 基盤作成
3. `.cub` 読込
4. texture / color / map parse
5. map validate
6. player 初期化
7. MLX window / image 初期化
8. floor / ceiling の単色描画
9. 壁線のみの raycast
10. texture 付き raycast
11. 入力と移動
12. 異常系テスト

この順序にすると、常に「表示できる最小状態」を積み上げながら進められる。

実装中は各段階で `norminette` を回し、設計上の分割が実際に Norm に効いているか確認する。

## 15. テスト観点

### 15.1 parser / validator

- 拡張子が違う
- ファイルが開けない
- 識別子不足
- 識別子重複
- 色の数値範囲外
- 色形式不正
- texture path 不正
- map が空
- spawn 0 個 / 2 個以上
- 不正文字混入
- map の途中に空行
- map が開いている
- 行長が不揃い
- space を含む map

### 15.2 runtime

- 起動直後に終了できる
- 矢印回転が滑らか
- `WASD` が想定通り動く
- 窓 close でもリークなく終了
- 長時間ループでもクラッシュしない

確認は可能な限り `linux()` コンテナ側で行う。

## 16. 要確認事項

現時点で実装前に詰めたい点は次の 2 つ。

1. mandatory で壁衝突を有効にするか
   - 私は「有効にする」前提が自然だと考えている。
   - ただし subject の bonus 文言と衝突するので、必要なら切り替え可能な構成にする。
2. texture 入力形式を `.xpm` 前提に寄せるか
   - Linux MiniLibX を考えると `.xpm` 前提が最も堅い。
   - subject 上は path しか縛っていないので、README と運用ルールで明示する手もある。
