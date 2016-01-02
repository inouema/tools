rem 
rem 全ユーザーのゴミ箱を空にする
rem
rd /s c:\$Recycle.Bin

rem
rem シャットダウンバッチファイル
rem -s : 電源を切る
rem -t : タイムアウト
rem -f : 実行中のプロセスを強制的に終了する
rem
shutdown -s -t 60 -f