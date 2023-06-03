<hr>
</p>
<p align="left">
Каталог <strong>Managed</strong> содержит решение LibExample(.Net 6) и CryptoRoom(.Net 6). 
</p>
<p align="left">
<strong>Решение CryptoRoom</strong> содержит:
  <p>
     <strong>CryptoRoomLib</strong>-.Net библиотека шифрования(ведется перенос кода с Qt).
  </p>
  <p>
    <strong>CryptoRoomLib.Tests</strong>-тесты библиотеки.
  </p>
  <p>
     <strong>CryptoRoomApp</strong>-демо пример для работы с библиотекой(в разработке).
  </p> 
  <p>
    <strong>Ui\MessageBox.Avalonia </strong>-библиотека диалоговых окон  AvaloniaUI и MVVM (https://github.com/AvaloniaCommunity/MessageBox.Avalonia).
  </p>
  <p>
  <strong>VanishBox</strong>-программа шифрования файлов (в разработке), для UI используется Avalonia-11.0.0-preview8.
  </p>
</p>
<br>
<p align="left">
<strong>Решение LibExample</strong> содержит:
</p>
<p align="left">
<strong>ManagedAdapter</strong>-интерфейс согласования Qt dll с Net 6(неуправляемого кода с управляемым) и его реализацию. Простое Api криптосистемы.
</p>
<p align="left">
<strong>Lib.Tests</strong> –тесты реализации интерфейса согласования. Тесты для Api пока не реализованы, ввиду постоянных переделок c++ кода.
</p>
<p align="left">
<strong>LibExample</strong>-<a href="./LibExample/LibExample.md">консольное приложение с демонстрационными примерами. </a>
</p>
<p align="left">
<br>
<strong>Native</strong> - каталог содержит С++ проекты использующее Qt framework. <a href="./BuildQt/HowBuild.md">Инструкция по сборке решения.</a> C 2019 г. больше не поддерживается.
</p>
<p>
<strong>EasyCript</strong> - программа «Роза». Позволяет шифровать файлы без использования удостоверяющего центра.<br>
<strong>GranitCore</strong> - ядро библиотеки. Содержит все функции.<br>
<strong>NativeDll</strong> - С++ dll библиотека с основными функциями.<br>
<strong>PrivateKeyGen</strong> - полный генератор секретного ключа. Создает закрытый ключ и запрос на получение сертификата. Использует биологический датчик случайных чисел.
</p>
<br>
<p align="left">
<strong>Binaries\Asn1Editor</strong>-программа для просмотра файлов в формате ASN1.
</p>
<p align="left">
<strong>Binaries\Binaries.zip</strong>-однокомпонентная(не требует наличия удостоверяющего центра) программа шифрования данных "Роза", написанная на Qt. Рабочая portable версия.
</p>
<p align="left">
<strong>Binaries\QtLibs.zip</strong>-Dll(C++) и Qt файлы необходимые для работы библиотеки.
</p>
<p>
<strong>Binaries\CryptoPPLib.zip</strong> - cобранная библиотека CryptoPP<br>
<strong>Binaries\Mpir.zip</strong> - собранная библиотека MPIR
</p>
<hr>