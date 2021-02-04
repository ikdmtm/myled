# led control program
# キャラクタ型デバイスドライバ
###  用意するもの
rasberry pi<br>
led　緑、黄、赤<br>
抵抗 10kΩ*3<br>
ブレッドボード<br>
ジャンパワイヤ*3<br>

### 使い方
$mkdir myled　　myledディレクトリを作成
$cd myled
~/myled$ make　　Makefileでコンパイル<br>
~/myled$ ls　　myled.koなどがあればok<br>
~/myled$ sudo chmod 666 /dev/myled0　　/dev/myled0への書き込みを許可<br>

#### 緑のLEDを点灯させる
~/myled$ echo "green" /dev/myled0 or echo "g" /dev/myled0 

#### 黄のLEDを点灯させる
~/myled$ echo "yellow" /dev/myled0 or echo "y" /dev/myled0

#### 赤のLEDを点灯させる
~/myled$ echo "red" /dev/myled0 or echo "r" /dev/myled0

#### LEDを消す
~/myled$ echo 0 /dev/myled0
