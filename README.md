# vanity_wallet
Vanity wallet generator <br> 
Spliced off code from jannotti/cpp-algorand-sdk <br>
Searches for wallets that start with a common five letter English word (or any other list of words of wallet length or less). <br>
It does this by generating random wallets and comparing them to a list of encoded target words <br>
Because the target words are not necessarily on the encoding byte boundary, a final masked byte has to be checked. <br>
Checks around 750,000 wallets per second on a c2-standard-30 / Intel Cascade Lake<br>
<br>
1664464465 : 139500818, 755.69 k hashes per second <br>
1664464542 : HIREME5MQPUNACM2CN2DWTCS23GZDKFE257MHVYJM73WVNCFXR2HYBFBEA <br>
1664464542 : scatter mammal bulb level opera cart live bubble case pipe actress guide seat assault sting great imitate balance between share van stage panther above anxiety <br>
