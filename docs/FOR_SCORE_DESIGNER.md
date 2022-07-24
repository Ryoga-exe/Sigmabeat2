# 譜面の作成方法

### 日本語 | [English](./en/FOR_SCORE_DESIGNER.md) |

このドキュメントは、Sigmabeat 用の譜面の作成方法をお伝えします。

## sgm ファイルを作成する

`score/` ディレクトリ下に `.sgm` 拡張子のついたテキストファイルを作成してください。文字のエンコードは`UTF-8`です。

例
```
.
└── score
    ├── example1
    │   └── example.sgm
    └── example2
        └── example2.sgm
```

`config.toml` を変更することで `score/` ディレクトリ以外も指定することができます。

## ヘッダ部分を記述する

楽曲の詳細な情報部分です。タイトルや作曲者を設定します。

`#{タグ名}  {値}` と指定します。

```
// 楽曲のタイトル
#Title     Example Title
// 作曲者
#Artist    Example Artist
// カテゴリー (現時点では意味がないです)
#Category  Example
// 譜面製作者
#NoteDesigner Example
// 楽曲のファイルのパス (sgm ファイルからの相対パスです。設定していなかったり存在しない場合は再生されません)
#Music     Music/example.wav
// ジャケット画像のパス (sgm ファイルからの相対パスです。設定していなかったり存在しない場合はアイコンになります)
#Image     Image/example.png
// 楽曲に関連する URL (現時点では意味がないです)
#URL       https://example.com
// 背景色を設定 (1つ指定で全部分、2つで上下のグラデーション、4つで四方からのグラデーション、#RANDOM とするとランダムな色に設定します)
#BgColor   #44B4B1, #006194
// メニュー画面での曲の再生位置
#DemoStart 3000
// 現時点では意味がないです
#MusicVol  100
// 現時点では意味がないです
#SEVol     0
// 譜面のオフセット値 (ミリ秒)
#OffSet    -100
// BPM (小数点可能)
#BPM       160
// 曲のレベル (コンマ区切りで4つ指定、値を記入しなかった場合はそのレベルは存在しないことになります、後述の譜面部分においては記入しなかった波括弧を飛ばします)
#Level     3, 5, 7, 12
// メニュー画面での表示優先度、高いほど先に表示されます
#PRIORITY  100
```

## 譜面部分を記述する

波括弧を並べて順に Easy、Normal、Hard、Expert となります。

```
// Easy
{

}

// Normal
{

}

// Hard
{

}

// Expert
{

}
```

注意ですが、一小節は波括弧直後から始まります。(修正されるかもしれませんが...)
つまり、

```
{ 1,1,1,1
  1,1,1,1
}
```

とするときちんと一小節から始まります、ご注意ください。

また、一行につき一小節となります。`|` を使うことにより強制的に小節を分けることができます。

リズムに関しては小節を `,` で区切ることにより表現します。
つまり、`1,1,1,1` とすると 4分の4拍子の間隔で、
`1,,,,1,,,,1,,,` とすると 4分の3拍子の間隔で流れてきます。

4分の4拍子の間隔で三連符を入れたい場合などは 4 と 3 の公倍数で分割することにより表現することができます。`1,1,1,1,1,1,1,1,1,1,1,1`

### 対応表

| 0~5 | a~f | g~l | m~r |
| --- | --- | --- | --- |
| Tap ノーツ  |  Holdノーツ(始)  | Holdノーツ(終)  | Pressノーツ  |

小さい方から順に 0,1,2,3,4,5 レーンに割り当てられます。

### マクロ

- `<s 2.0>`
  - スピードをその時点から 2.0 に設定します。
- `<j 300>`
  - 判定ラインを 300 に移動させます (デフォルトは60)
- `<l>`
  - 小節線を追加します
- `<e>`
  - 小節線を自動で挿入するかを切り替えます
- `<b 200>`
  - BPM を 200 にします。

### その他の記法

- `#BPM 200`
- `#SPEED 2.0`
- `#JUDGEBAR 300`
- `#FUNCDELAY 1/16`
  - `#` から始まる記法において 1/16 だけ反映を遅らせます 例えば `96/192` とすると 96/192 だけ遅らせます。分子に 0 を指定すると元に戻ります。
- `#FUNCDELAYMS 200`
  - `#FUNCDELAY` をミリ秒で指定する版です。

### 例

複雑な譜面の例 (https://twitter.com/Ryoga_exe/status/1550082078586576896) でプレイしている譜面の序盤の部分

```
{#FuncDelay 0/192
#BPMChange 42.667
#FuncDelay 0/192
#Speed 0
#FuncDelay 64/192
#Speed 0.667
#FuncDelay 65/192
#Speed 0
#FuncDelay 67/192
#Speed 0.667
#FuncDelay 68/192
#Speed 0
#FuncDelay 70/192
#Speed 0.667
#FuncDelay 71/192
#Speed 0
#FuncDelay 73/192
#Speed 0.667
#FuncDelay 74/192
#Speed 0
#FuncDelay 75/192
#Speed 0.667
#FuncDelay 76/192
#Speed 0
#FuncDelay 77/192
#Speed 0.667
#FuncDelay 78/192
#Speed 0
#FuncDelay 80/192
#Speed 0.667
#FuncDelay 81/192
#Speed 0
#FuncDelay 101/192
#Speed 0.667
#FuncDelay 102/192
#Speed 0
#FuncDelay 104/192
#Speed 0.667
#FuncDelay 105/192
#Speed 0
#FuncDelay 107/192
#Speed 0.667
#FuncDelay 108/192
#Speed 0
#FuncDelay 110/192
#Speed 0.667
#FuncDelay 111/192
#Speed 0
#FuncDelay 113/192
#Speed 0.667
#FuncDelay 114/192
#Speed 0
#FuncDelay 115/192
#Speed 0.667
#FuncDelay 116/192
#Speed 0
#FuncDelay 119/192
#Speed 0.667
#FuncDelay 120/192
#Speed 0
#FuncDelay 123/192
#Speed 0.667
#FuncDelay 124/192
#Speed 0
#FuncDelay 176/192
#Speed 0.033
#FuncDelay 180/192
#Speed 0.067
#FuncDelay 184/192
#Speed 0.1
#FuncDelay 188/192
#Speed 0.133
	,,,
#FuncDelay 0/192
#BPMChange 128
#FuncDelay 0/192
#Speed 0.5
	d,,je,
	ck,,id,
	bj,,hc,
#FuncDelay 0/192
#BPMChange 256
#FuncDelay 0/192
#Speed 0.6
	3,,3,
```