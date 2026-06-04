# cub3D Task Breakdown

## 1. 目的

このドキュメントは `docs/subject.md`, `docs/design.md`, `docs/lecture.md`
を実装タスクへ分解したものである。

方針:

- scope は mandatory のみ
- 1 task は 30 分程度で review できる粒度にする
- macOS で反復し、Linux で最終確認する
- task 間の依存を明示する
- 曖昧さは task 側へ持ち込まず、前提を固定する

## 2. 固定前提

この task 表では、実装上の前提を次で固定する。

- local development は macOS、final runtime は Ubuntu 22.04 +
  `minilibx-linux`
- backend 差分は `platform` 層へ隔離する
- common code は backend 固有 header を直接参照しない
- key input は common code から `mlx_key_hook` を直接使わず、
  `platform` 経由で `mlx_hook` を扱う
- texture asset は実運用上 `.xpm` を使う
- render は persistent frame image へ描いてから
  `mlx_put_image_to_window()` する
- movement は `can_move_to()` 経由で walkable cell のみへ進める
- `can_move_to()` は少なくとも wall `1` と padding space を拒否し、
  player が有効 map 外へ出ないことを優先する
- bonus feature は task に含めない

補足:

- `platform` を先に作ることで、mac で進めた code を Linux へ移す際の
  差分を最小化できる
- `can_move_to()` を 1 箇所へ隔離することで、将来の仕様調整が必要でも
  movement task 全体を壊さずに済む
- `.xpm` 前提は subject の path 要件と矛盾しない。運用上の安全策である

## 3. Review Rule

各 task の review では、少なくとも次を確認する。

- subject 要件に直接対応しているか
- `docs/design.md` の責務分離と依存方向を壊していないか
- `docs/norm.md` の 25 行 / 5 変数 / 4 引数 / 5 関数制限を守れる分割か
- backend 差分が common code へ漏れていないか
- 部分失敗時の cleanup と error path が考慮されているか

## 4. Task Table

| ID | Task | Depends | Done when | Review focus |
| --- | --- | --- | --- | --- |
| T01 | Project skeleton and Makefile bootstrap | - | `include/`, `src/` 配下の基本配置と、backend 切替を見越した Makefile 骨格ができている | wildcard 不使用、host/backend 切替方針、default target が `all` |
| T02 | Shared config, type, and platform declarations | T01 | `config.h`, `types.h`, `cub3d.h`, `platform.h` に共通定数と構造体宣言が揃う | `struct` が `.c` に漏れていない、`KEY_*` / `EVENT_*` が 1 箇所に集約されている |
| T03 | Error API and free helpers | T02 | `Error\n` 出力関数、共通 free helper、部分初期化対応の destroy helper 方針が入る | stderr 出力、deep function で `exit()` しない設計 |
| T04 | Main entry and bootstrap path | T01 T02 T03 | `main` から zero-init, argc check, `.cub` 拡張子 check, app bootstrap 呼び出しまで通る | 正常系 / 異常系の return path が整理されている |
| T05 | Read `.cub` file into one buffer | T03 T04 | `open` + `read` で可変長 buffer に全文を読み込める | read loop, EOF, open/read error の扱い |
| T06 | Split file buffer into lines | T05 | `char **lines` が作れ、末尾行の扱いが安定する | empty line, trailing newline, allocation rollback |
| T07 | Parse context and scene scan | T06 | `t_parse_ctx` を用いた行走査で map 開始位置と section 境界を判定できる | map 開始後の空行 / identifier 再登場を reject |
| T08 | Parse texture identifiers | T07 | `NO`, `SO`, `WE`, `EA` を 1 回ずつ格納し、path の存在確認まで行う | duplicate / missing / empty path / unreadable path |
| T09 | Parse floor and ceiling colors | T07 | `F`, `C` の `R,G,B` を parse して `0xRRGGBB` へ変換できる | range check, comma error, space handling |
| T10 | Collect raw map rows and finalize scene completeness | T07 T08 T09 | raw map row を見た目どおり保持し、scene 要素の欠落検知まで終わる | map must be last, shortest/longest row, no silent trim |
| T11 | Validate map characters | T10 | 許可文字 ` 01NSEW` 以外を reject できる | invalid char message と行走査の単純さ |
| T12 | Detect spawn and initialize player vectors | T11 | spawn がちょうど 1 個であることを確認し、`x,y,dir,plane` を初期化できる | `N/S/E/W` ごとの `dir` / `plane` が正しい |
| T13 | Build normalized closed grid | T10 T11 | ragged map から padding 付き長方形 grid を作れる | raw map を壊さずに validation 用 grid を分離している |
| T14 | Flood-fill closed-map validation | T13 T12 | 外側 flood fill で open map を検出できる | space 接触, spawn/`0` 接触, rollback と message |
| T15 | Local mac MLX bootstrap | T04 T10 T14 | mac backend で `mlx_init`, `mlx_new_window` が app state へ組み込まれる | common init path、backend 差分の漏れ、window failure path、common subset だけを使っている |
| T16 | Persistent frame image and pixel-write helper | T15 T02 | frame image の作成、`mlx_get_data_addr`, pixel write helper が揃う | `line_len`, `bpp`, `endian` を固定値扱いしない |
| T17 | Texture image loading and address fetch | T15 T08 | 4 texture を MLX image として読み込み、`addr` まで取得できる | `.xpm` common subset, width/height 保存, cleanup 順序 |
| T18 | Ceiling and floor background fill | T16 T09 | frame 全体の上半分 / 下半分を色で塗り分けられる | 毎 frame 上書き、全 pixel 塗り漏れなし |
| T19 | Ray initialization per screen column | T12 T16 | `camera_x`, `ray_dir`, `map_x`, `map_y`, `delta_dist` 初期化ができる | `t_ray` へ状態集約、列ごと処理の独立性 |
| T20 | DDA stepping and wall hit detection | T19 T10 | `1` に当たるまで DDA で cell を進められる | `step_x/y`, `side_dist_x/y`, side 判定の正しさ |
| T21 | Perpendicular distance and draw range | T20 | `perp_wall_dist`, `line_height`, `draw_start/end` を計算できる | fish-eye 防止、画面外 clamp |
| T22 | Wall side selection and texture X coordinate | T17 T20 T21 | hit side から N/S/E/W texture と `tex_x` を決められる | side 判定と texture 反転の正しさ |
| T23 | Draw one textured wall column | T16 T22 | 1 列分の textured wall を frame へ描ける | `tex_pos`, `tex_step`, pixel write の bounds |
| T24 | Full-frame render pass and expose redraw | T18 T23 | 全列 raycast -> present が通り、expose で再描画できる | frame clear -> wall draw -> `mlx_put_image_to_window` 順序 |
| T25 | Platform hook binding and event mapping | T15 T24 | `platform_bind_hooks()` で `KeyPress`, `KeyRelease`, `DestroyNotify`, `Expose`, `LoopHook` を登録し、backend 生 key/event を project 定数へ正規化できる | `mlx_key_hook` を使わない、backend event 差分が common code に漏れていない |
| T26 | Key-state transitions and ESC handling | T25 | `WASD`, arrows, `ESC` の press/release が state に反映される | raw backend keycode 直書き回避、`KEY_*` 定数使用 |
| T27 | Delta-time update and rotation | T12 T25 T26 | `gettimeofday()` ベースで `delta_sec` を更新し、左右回転できる | frame count 依存を避ける、回転行列の更新順 |
| T28 | Movement update through `can_move_to()` | T14 T26 T27 | 前後移動と平行移動ができ、無効 cell へ進入しない | movement policy が `can_move_to()` に閉じている |
| T29 | Runtime loop wiring | T24 T26 T27 T28 | loop 1 回で `time -> input update -> render -> present` が成立する | update/render の責務分離、無駄な再生成なし |
| T30 | Unified normal-exit and early-failure cleanup | T03 T15 T17 T29 | `ESC`, close button, init failure の全経路で安全に終了できる | image -> window -> platform cleanup -> heap の逆順解放 |
| T31 | Parser negative test maps | T10 T11 T12 T14 | invalid `.cub` 群を作り、主要 error path を再現できる | duplicate id, bad color, open map, invalid char, bad spawn |
| T32 | Positive smoke maps and texture assets | T10 T17 T24 | 最小 valid map と確認用 texture asset が揃う | mandatory 条件だけで起動確認できる構成 |
| T33 | Linux backend bring-up, runtime verification, and norm pass | T29 T30 T31 T32 | `platform` 層だけの差分で `linux()` 上の build/run を確認し、`norminette` で主要 file 群を通す | subject 要件と Linux 実機挙動の差が common code へ漏れていないか |
| T34 | README mandatory draft and final packaging | T33 | 英語 README の required section を満たし、検証後に `make clean` と `make fclean` を行う | `Description`, `Instructions`, `Resources`, AI usage, final cleanup |

## 5. Recommended Review Order

review 順は dependency 順を守る。

- Phase 1
  - T01-T04
- Phase 2
  - T05-T10
- Phase 3
  - T11-T14
- Phase 4
  - T15-T18
- Phase 5
  - T19-T24
- Phase 6
  - T25-T30
- Phase 7
  - T31-T34

この順にすると、常に「前段が壊れていない前提」で次の review ができる。

## 6. Consistency Check

この task 表は、作成後に次の観点で整合性確認を行った。

- `docs/subject.md`
  - mandatory scope だけを対象にしている
  - `.cub` parser, 4 texture, floor/ceiling color, map close, controls,
    close button, clean exit, README を task に反映している
- `docs/design.md`
  - module 分離、Norm 前提の細分化、MLX 初期化順、render flow,
    parser/validator 分割を task 依存へ反映している
- `docs/lecture.md`
  - mac 開発 / Linux 最終移行、`platform` 分離、frame image 利用、
    raycast 列単位処理、XPM 運用を task 前提へ反映している
- `docs/norm.md`
  - 1 task で review 可能な粒度まで分割し、巨大関数化しない流れにした

task 表へ反映した修正点:

- 開発方針を Linux-first から「mac 反復 + Linux 最終確認」へ変更した
- backend 差分を `platform` 層へ閉じ込める task を前提へ反映した
- 衝突判定は `can_move_to()` に隔離して「有効 map 外へ出ない」を優先して固定した
- texture 形式は mac / Linux 共通運用を優先して `.xpm` 前提に固定した
- subject の README 要件を task から漏らさないよう T34 を維持した
- AGENTS.md の運用に合わせ、最終 task に `make clean` と `make fclean`
  を明記した
