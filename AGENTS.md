## 指示
- 本プロジェクトは42tokyoの課題の１つです。
- 課題要件、コーディング規則、設計などに関して、docs配下にドキュメントがあります。それらを読み。理解できたら「理解」と返答していください。
- プロジェクト設計、実装に関して不明点がある場合は、都度ユーザに問い合わせてください。
- makeで動作確認後は"make clean", "make fclean"でビルド成果物を掃除してください。
- ローカルには `val:latest` と `ubuntu:22.04` の Docker イメージがあるが、X11 開発パッケージ入りの専用イメージはない。
- 本番環境は ubuntu22.04 です。そのため動作確認は `ubuntu:22.04` イメージを使用し、`build-essential`, `xorg`, `libxext-dev`, `libx11-dev`, `libbsd-dev` を入れたコンテナで行うこと。
- 起動例は以下。

```bash
docker run --rm -it --platform linux/amd64 \
	-v "$PWD":/app -w /app ubuntu:22.04 \
	bash -lc 'apt-get update && apt-get install -y build-essential xorg libxext-dev libx11-dev libbsd-dev && bash'
```

- reviewは以下の観点から指摘して
    - コードが適切に実行可能か
    - コードが要件、normに適合しているか
    - コード上の不整合がないか
    - 品質（保守性・責務分離・依存関係の少なさ）
    - 可読性
