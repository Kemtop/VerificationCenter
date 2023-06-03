<h2 align="center">Как собрать решение GranitCrypt, использующее Qt.</h2>
<p align="left">
Для сборки необходимо установить Visual Studio 2017,  плагин для работы с Qt qt-vsaddin-msvc2017-2.8.0.vsix, установленный набор инструментов платформы v140. Скачать Qt 5.10.1.
</p>
<p align="left">
Открываем решение:<br>
<img src="./SelectSolution.jpg" width="80%">
</p>
<p>
Visual Studio предлагает нам перенацелить проект. Нажимаем "Отмена".<br>
<img src="./Retarget.jpg" width="80%"><br>
Задаем версию Qt:<br>
<img src="./SetQtVersion0.jpg" width="80%"><br>
Устанавливаем проектам NativeDll, GranitCore, EasyCript версию Qt:<br>
<img src="./SetQtVersion1.jpg" width="80%"><br>
<img src="./SetQtVersion2.jpg" width="50%"><br>

Назначаем проект EasyCript автозагружаемым проектом.<br>
Используем собранные библиотеки CryptoPP, Mpir.<br>
Копируем все файлы из \Binaries\CryptoPPLib.zip в \Native\CryptoPP\Win32\Output\
и \Binaries\Mpir.zip в \Native\Mpir<br>
CryptoPP необходим для работы с RSA и чтения ASN1. Mpir – библиотека больших чисел, для подписи файлов.<br>
Последовательно собираем проекты:<br>
1. GranitCore - ядро библиотеки;<br>
2. NativeDll - C++ библиотека, которую могут использовать внешние программы;<br>
3. EasyCript - программа шифрования «Роза»;<br>
</p>