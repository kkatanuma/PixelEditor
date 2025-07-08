# ピクセルエディター
ユタ州大学の課題　C++ Software Development コースの課題  

制作期間 2週間  
開発人数 4人

## コントリビューター
Bryce Radle  
Cole Phillips  
Keita Katanuma  
Maxwell Gestland  

## 開発環境
C++  
QT Framework  
GitHub

## 実装機能
ピクセルの描画と消しゴム機能  
描画カラーの変更  
解像度の変更  
スプライトのリアルタイムプレビュー
スタンプ機能  
フレームの追加・削除  
スプライトシートの保存・読み込み　(.ssp形式)  
GIFファイルの読み込み対応  

## 担当した機能
### スプライトシートへの保存・読み込み  
PixelのフレームデータをJSON形式に変換し、.sspファイルとして保存  
JSONから各フレームのピクセル画像を復元  

### GIFファイルへの対応  
GIFファイルを読み込み  
各フレームを抽出し、エディターにインポート  

主に model.h / model.cpp　において、JSONおよび画像の変換、ファイルの保存と読み込み機能を担当しました。

