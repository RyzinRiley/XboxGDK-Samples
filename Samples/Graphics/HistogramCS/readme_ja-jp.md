![](./media/image1.png)

# ヒストグラム CS サンプル

*このサンプルは、Microsoft Game Development Kit と互換性があります (2022 年 3 月)*

# 説明

このサンプルは、Xbox でのコンピュート シェーダーのパフォーマンスに関する考慮事項をいくつか示![](./media/image2.jpeg)します。 実際には、シーンの強度の 64 バケット ヒストグラムの演算を、(最適な実装によって) 0.2 ミリ秒未満で行うための最適な方法を示します。

# サンプルのビルド

Xbox One 開発キットを使用している場合は、アクティブなソリューション プラットフォームを `Gaming.Xbox.XboxOne.x64` に設定します。

Xbox Series X|S 開発キットを使用している場合は、アクティブなソリューション プラットフォームを `Gaming.Xbox.Scarlett.x64` に設定します。

*詳細については、**GDK ドキュメント*の「__サンプルの実行__」を参照してください。

# サンプルの使用方法

| 操作 | ゲームパッド |
|---|---|
| サイクル ヒストグラムの手法 | A button |
| 回転ビュー | 左サムスティック |
| ビューのリセット | 左サムスティック (クリック) |
| Exit | ビュー ボタン |

# 実装メモ

CPU プログラミングでは、複数のコアに作業を分散する場合、キャッシュ使用率とパフォーマンスを向上させるために、各コアが連続したデータ ブロックを取得するようにすることがよくあります。 HLSL では、各シェーダー プログラムは独自の空間 (CPU のスレッドに似ています) で実行されているため、GPU にアルゴリズムを実装する場合も同じように考えると簡単です。 ただし、最新の GPU は並列性が高いため、これは当てはまりません。

Xbox One GPU は、実際にウェーブと呼ばれる最大 64 幅のスレッドのグループで動作します。 つまり、各スレッドは、すべてまったく同じ命令を実行するロックステップで動作する、より大きな並列スレッドのグループの一部です。 CPU 上の 64 フロート幅の SIMD 命令セットに似たものだと考えることができます。

つまり、結論としては個々のスレッドが隣接するデータをプルするのは最適ではないということです。 理想的なケースは、隣接するスレッドが隣接するデータを読み取るように読み取りを結合することです。 実質的には、スレッド グループ内の各スレッドは、ループの反復ごとに 64 番目の項目ごとに読み取ります。 これは、サンプルからわかるように、最終的により最適な結果となります。

ただし、このサンプルでは、読み取り順序だけがブロック要因ではありません。 ヒストグラムは、ヒストグラムの各バケットにアトミック加算を実行することによって生成されます (合計 64 個のバケットがあります)。 Xbox One GPU はこれらの処理を非常に迅速に行います (実際には L2 キャッシュで実行される) が、それでもゲート要因になります。 これをさらに高速に実行するために、HLSL のグループ共有メモリを介して公開される非常に高速な内部 LDS キャッシュを利用できます。 各スレッドをスキャンライン全体でスレッド全体に固定することで、LDS で結果を構築し、最後に最終的な結果をアトミックに追加します。 LDS でのアトミック操作は、メイン メモリ (または L2) よりも大幅に高速であるため、パフォーマンスが大幅に向上します。

**この手法やその他の詳細については、Xfest 2013 のコンピュート シェーダーの講演を確認してください。**

# 既知の問題

ありません。

# 更新履歴

サンプルの元のバージョンは、XSF ベースのフレームワークを使用して記述されました。 6 月に ATG サンプル テンプレートを使用するように書き換えられました
2020 年。

# プライバシーに関する声明

サンプルをコンパイルして実行する場合、サンプルの使用状況を追跡するために、サンプルの実行可能ファイルのファイル名が Microsoft に送信されます。 このデータ コレクションからオプトアウトするには、Main.cpp の "サンプル使用状況テレメトリ" というラベルの付いたコードのブロックを削除します。

Microsoft のプライバシー ポリシー全般の詳細については、「[Microsoft のプライバシーに関する声明](https://privacy.microsoft.com/en-us/privacystatement/)」を参照してください。


