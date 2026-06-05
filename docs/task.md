# cub3D Task Breakdown

## 1. 目的

このドキュメントは `docs/subject.md`, `docs/design.md`, `docs/lecture.md`
を、mandatory 実装タスクへ分解したものである。

今回の前提は次で固定する。

- Ubuntu 22.04 + `minilibx-linux`
- mandatory only
- `.xpm` texture
- `mlx_hook()` ベースの入力
- `platform` abstraction なし

## 2. 分割方針

- 1 task は 30 分程度で review できる粒度にする
- task は「1 つの責務が終わったか」で区切る
- 実装量より review しやすさを優先する
- 過剰な抽象化を前提にしない

## 3. 固定判断

- header は `include/cub3d.h` を基本にする
- `libft` は build に含める
- key / event は Linux の `X11` 定数を使う
- runtime map は padding 済み `grid` を使う
- validator の実装は `src/parse/validate_map.c` に寄せる
- 衝突判定は `can_move_to()` に寄せる
- movement / rotation は `delta time` ベースにする

## 4. Review Rule

各 task の review では少なくとも次を確認する。

- subject 要件に直接対応しているか
- `docs/design.md` の責務分離を壊していないか
- `docs/norm.md` の 25 行 / 5 変数 / 4 引数 / 5 関数制限を守れるか
- cleanup と error path が考慮されているか
- Linux 前提の割り切りが過不足なく反映されているか

## 5. Task Table

| ID | Task | Depends | Done when | Review focus |
| --- | --- | --- | --- | --- |
| T01 | Project skeleton and Makefile bootstrap | - | `include/`, `src/` の骨格と、`libft` / `minilibx-linux` を含む Makefile 骨格がある | wildcard 不使用、`all/clean/fclean/re`、再 link 条件 |
| T02 | Shared header and core structs | T01 | `cub3d.h` に定数、構造体、主要 prototype が揃う | `struct` が `.c` に漏れていない、include 過多でない |
| T03 | Error API and cleanup helpers | T02 | `Error\n` 出力、free helper、destroy の下地がある | deep function で `exit()` しない、rollback 前提 |
| T04 | Main entry and argument validation | T01 T02 T03 | `main` から argc / `.cub` 拡張子検証と bootstrap 呼び出しまで通る | return path が明確、早期 failure が安全 |
| T05 | Read `.cub` file and split lines | T03 T04 | file 全文読込と `char **lines` 化ができる | EOF、trailing newline、allocation rollback |
| T06 | Parse textures and colors | T05 | `NO/SO/WE/EA/F/C` を scene に格納できる | duplicate、missing、bad color、path error |
| T07 | Parse raw map rows and scene completeness | T05 T06 | raw map を見た目どおり保持し、map last 条件を守れる | map 開始後の空行、silent trim なし |
| T08 | Validate map chars and spawn | T07 | 許可文字検証と spawn 1 個判定、player 初期化ができる | `N/S/E/W` の向き、invalid char message |
| T09 | Build padded grid and closed-map validation | T07 T08 | 長方形 `grid` 作成と flood fill 閉包判定が通る | space 扱い、open map 検出、runtime への流用 |
| T10 | MLX init, window, and frame image | T04 T09 | `mlx_init`, `mlx_new_window`, `mlx_new_image`, `mlx_get_data_addr` が通る | failure path、image 再利用前提 |
| T11 | Texture image loading | T06 T10 | 4 texture を `mlx_xpm_file_to_image()` で読める | `.xpm` 前提、width/height 保存、cleanup 順序 |
| T12 | Pixel helper and background fill | T10 | pixel write helper と床 / 天井塗りができる | `line_len`, `bpp`, `endian` を固定扱いしない |
| T13 | Ray init and DDA stepping | T08 T10 | 列ごとの ray 初期化と DDA による壁 hit ができる | `t_ray` 集約、side 判定、無限 loop 防止 |
| T14 | Distance, wall side, and texture sampling | T11 T13 | `perp_wall_dist`, draw range, texture 選択、`tex_x` が計算できる | fish-eye 防止、方角判定、texture 反転 |
| T15 | Draw one wall column and full render pass | T12 T14 | 全列 raycast して frame を window へ出せる | clear -> wall draw -> present の順序 |
| T16 | Hook registration and key state handling | T10 T15 | `KeyPress`, `KeyRelease`, `DestroyNotify`, `Expose`, `mlx_loop_hook` を登録し、key state を更新できる | `mlx_key_hook` 不使用、X11 keysym 使用 |
| T17 | Delta time, movement, and rotation | T08 T09 T16 | `delta_sec` 更新、前後移動、平行移動、左右回転ができ、半径つき衝突判定で壁めり込みと角抜けを防げる | `can_move_to()` 集約、`PLAYER_RADIUS`、軸分離更新、frame 依存を避ける |
| T18 | Unified loop and clean exit | T03 T15 T16 T17 | loop 1 回で update + render が成立し、`ESC` / close で安全に終了できる | Linux cleanup、逆順解放、部分初期化対応 |
| T19 | Invalid maps and positive smoke assets | T07 T09 T11 T15 | 異常系 `.cub` と正常系最小 map / texture が揃う | 1 file 1 failure 要因、smoke test 最小構成 |
| T20 | Ubuntu VM verification, norm, and final cleanup | T18 T19 | Ubuntu VM 上で build / run / `norminette` を確認し、最後に `make clean` と `make fclean` を行う | mandatory 要件、Norm、README 反映漏れ |

## 6. Recommended Review Order

- Phase 1
  - T01-T04
- Phase 2
  - T05-T09
- Phase 3
  - T10-T12
- Phase 4
  - T13-T15
- Phase 5
  - T16-T18
- Phase 6
  - T19-T20

この順にすると、
常に前段の責務が固まった状態で次の review ができる。

## 7. Consistency Check

この task 表は作成後に次の観点で見直した。

- `docs/subject.md`
  - mandatory のみを対象にしている
  - `.cub`, map close, texture 4 方向, `WASD`, 矢印, `ESC`, close button,
    README を task に反映している
- `docs/design.md`
  - Linux 固定、`platform` 廃止、`cub3d.h` 中心、`grid` 流用、
    `validate_map.c` へ validator 集約、
    `can_move_to()` 集約を task 前提に反映している
- `docs/lecture.md`
  - `minilibx-linux`, `mlx_hook`, `X11/keysym.h`, `perp_wall_dist`,
    image buffer 前提を task に反映している
- `docs/norm.md`
  - 1 task で review 可能な粒度を保ち、巨大関数化しない流れにした

今回の再構成で落としたもの:

- mac 開発 / Linux 移行の task
- backend abstraction 関連 task
- `platform` module の task
- 必要以上に細かい render / input 分割 task

今回の再構成で残したもの:

- parser / validator / render / input / cleanup の責務分離
- map 正規化 grid と flood fill
- `delta time`
- Ubuntu VM での最終確認
