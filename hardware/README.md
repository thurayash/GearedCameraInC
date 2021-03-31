## BEFORE UPLOADING THE SCRIPT ##

Please remove the 10uF capacitor between the RESET and the GND pin


Before running the client script :
    Please run this command
        stty -F /dev/ttyACM0 cs8 9600 ignbrk -brkint -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts

