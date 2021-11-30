# NenEngine
## 概要
 NenEngineは、主にゲーム開発のために製作中のC++20向けメディアライブラリです。  
 某ゲーム開発アニメのNeneEngineに影響を受けています。そのためにエンジンと冠していますが実際はライブラリ程度です。  
 自身のみで使用する予定のライブラリであるので、サポートやドキュメントは充実しないと思われます。  
 **ドキュメントは全て日本語です（All documentation is only in Japanese）。**

## 対応プラットフォーム
 今のところ、Windows、WebGL、Androidに対応しています。Linuxでは確認していませんが、少しの手直しで動くと思います。  

## サンプルコード
 exampleフォルダを参照してください。
``` c++
#include <Nen/Nen.hpp>

/**
 *  メインシーン
 * データメンバが定義出来ないので、
 * 基本的に各種設定を行って他のシーンに移るための踏み台となる
*/
void Main::Setup()
{
    //背景を黒に設定
    GetRenderer()->SetClearColor(nen::Palette::Black);

    //フォントの読み込み
    auto font = std::make_shared<nen::Font>();
    font->LoadFromFile("Assets/Font/mplus/mplus-1p-medium.ttf", 72);

    //アクターを追加
    auto actor = this->AddActor<nen::Actor>();

    //アクターにコンポーネントを追加
    auto text = actor->AddComponent<nen::TextComponent>();
    text->SetFont(font);
    text->SetString("Hello,World!", nen::Palette::White);
}

void Main::Update(float deltaTime)
{
    //キーボードのQが押されたら終了
    if (GetInput().Keyboard.GetKeyValue(nen::KeyCode::Q))
        Quit();

    /**
     * 描画処理は裏側で行っている
    */
}
```
![結果](https://github.com/Astomih/NenEngine/blob/main/example/result.png "result")

## ビルド方法
### Windows
　zipとしてダウンロード・解凍し、NenEngine/Windowsフォルダ内のslnファイルをVisual Studio（2019以降)でビルドしてください。  
 NenEngine/DebugもしくはNenEngine/Releaseフォルダが作成され、nen.libが作成されます。  
 今のところVisual Studioでのビルドのみに対応しています。MinGWでは検証していません。  
### WebGL
 ビルドにはEmscriptenのセットアップおよびCMakeが必要です。
 その後、以下のコマンドを実行してください。
 ```
 $ cd ./Emscripten  
 $ emcmake cmake .
 $ emmake make
 ```
 NenEngine/Emscripten/buildフォルダ内にlibnen.aが作成されます。
 ### Android
  上記に比べて複雑なため、ここでは割愛します。tutorialフォルダのInstall.mdを御覧ください。

## 使用ライブラリ
NenEngineで使用しているライブラリです。  
ライセンス文はlibs内の各フォルダ内に記載されています。
- Assimp
- GLEW
- ImGui
- Lua
- Sol2
- OpenAL 1.1
- rapidJSON
- SDL2
- SDL2_image
- SDL2_mixer
- SDL2_net
- SDL2_ttf
  
各ライブラリはNenEngine/libsフォルダ内にライセンス文ともに格納されています。

## ライセンス
 NenEngineのライセンスはMIT Lisenceです。

## ドキュメント
[APIリファレンス](https://astomih.github.io/NenEngine)  
その他については製作中。。。