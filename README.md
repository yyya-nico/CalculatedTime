# CalculatedTime

TVTest が計算した現在時刻を、ステータスバーに `YYYY/MM/DD HH:MM:SS` 形式で表示するプラグインです。

> [!IMPORTANT]
> このプラグインは [yyya-nico/TVTest](https://github.com/yyya-nico/TVTest) の `develop` ブランチ向けです。
> 以下に記載する TOT / SIT の選択処理はこのフォーク独自の挙動です。他の TVTest にはそのような挙動はありません。

## 表示される時刻

プラグインは TVTest の次の変数を1秒ごとに展開して表示します。

```text
%tot-year%/%tot-month2%/%tot-day2% %tot-hour2%:%tot-minute2%:%tot-second2%
```

`yyya-nico/TVTest` では、表示時刻は次の順で決まります。

1. TOT を受信している場合は、PCR で補間した TOT 時刻
2. TOT がなく、SIT の伝送情報ループまたはサービス記述子ループに `partial_transport_stream_time_descriptor` がある場合は、その `JST_time` から得た時刻
3. どちらも得られない場合は、PC の時刻を基にした EPG 時刻

プラグインは公開 SDK の変数文字列を表示しているため、現在どの時刻源が選ばれているかを判別しません。変数を展開できない場合は `----/--/-- --:--:--` を表示します。

## ビルド

このプロジェクトは TVTest SDK のサンプル用ファイルを参照します。`yyya-nico/TVTest` のソースツリーを取得し、このリポジトリを次の位置に配置してください。

```text
TVTest/
└─ sdk/
   └─ Samples/
      └─ CalculatedTime/
         ├─ CalculatedTime.cpp
         └─ CalculatedTime.vcxproj
```

`CalculatedTime.vcxproj` を Visual Studio で開き、使用する TVTest と同じプラットフォーム（Win32 または x64）の `Release` 構成でビルドします。生成物は次の場所に出力されます。

```text
bin/<Platform>/Release/CalculatedTime.tvtp
```

## インストール

1. `CalculatedTime.tvtp` を TVTest の `Plugins` フォルダーへコピーします。
2. TVTest を起動し、プラグイン一覧から `Calculated Time` を有効にします。
3. ステータスバーに「計算済みTOT/SIT時刻」が表示されることを確認します。

ステータス項目の表示 / 非表示とプラグインの有効 / 無効は連動します。
