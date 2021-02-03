# led control program

~/myled$ make　　Makefileでコンパイル
~/myled$ ls　　myled.koなどがあればok
~/myled$ sudo chmod 666 /dev/myled0　　/dev/myled0への書き込みを許可 

#### 緑のLEDを点灯させる
~/myled$ echo "green" /dev/myled0 or echo "g" /dev/myled0 

#### 黄のLEDを点灯させる
~/myled$ echo "yellow" /dev/myled0 or echo "y" /dev/myled0

#### 赤のLEDを点灯させる
~/myled$ echo "red" /dev/myled0 or echo "r" /dev/myled0

#### LEDを消す
~/myled$ echo 0 /dev/myled0
