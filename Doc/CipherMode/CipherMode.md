<h2 align="center">Режимы работы блочного шифра.</h2>

<p align="left">
Для кодирования данных блочный шифр работает в режиме простой замены с зацеплением.
</p>
<p align="left">
На рисунках показан принцип работы. Где: IV-начальный вектор(случайное число), P-кодируемый блок данных, C-зашифрованные данные, Еk-алгоритм кодирования с использованием ключа k.
</p>
<p align="left">
</p>
<p align="left">
<img src="./CipherModeCBC.png" width="100%">
</p>
<p align="left">
<img src="./CipherModeCBC1.png" width="100%">
</p>
<p align="left">
Для работы с контейнерами используется режим CFB(Cipher Feedback Mode).
</p>
<p align="left">
Режим обратной связи по шифротексту, режим гаммирования с обратной связью -для шифрования следующего блока открытого текста он складывается по модулю 2 с перешифрованным (блочным шифром) результатом шифрования предыдущего блока.
</p>
<p align="left">
<img src="./CFBEncryption.png" width="100%">
</p>
<p align="left">
<img src="./CFBDecryption.png" width="100%">
</p>