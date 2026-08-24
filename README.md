# CalculatedTime

TVTest が `%tot-*%` 変数として公開する計算済み時刻を、ステータスバーに
`YYYY/MM/DD HH:MM:SS` 形式で表示する最小プラグインです。

[TVTest](https://github.com/yyya-nico/TVTest) 内部では、TOT があれば PCR 補間された TOT 時刻、TOT がなく SIT に
`partial_transport_stream_time_descriptor` があれば SIT 時刻が使われます。
どちらも得られない場合は PC 時刻由来の EPG 時刻にフォールバックします。
公開 SDK からは、この3種類を区別できません。

`CalculatedTime.vcxproj` を Visual Studio で開き、TVTest と同じプラットフォーム
(Win32 または x64) でビルドしてください。生成された `CalculatedTime.tvtp` を
TVTest の `Plugins` フォルダーへコピーして有効にすると、ステータスバーに項目が
追加されます。
