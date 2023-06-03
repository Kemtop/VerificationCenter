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

Переводим в Release:<br>
<img src="./SelectRealease.jpg" width="80%"><br>
Назначаем проект EasyCript автозагружаемым проектом.<br>
Последовательно собираем проекты:<br>
1. cryptlib - библиотека работы с RSA;<br>
2. lib_mpir_cxx - MPIR Library(библиотека для работы с большими числами);<br>
3. lib_mpir_gc;<br>
4. GranitCore;<br>
4. NativeDll - C++ библиотека, которую могу использовать внешние;программы.<br>
5. EasyCript - программа шифрования «Роза»;<br>
</p>