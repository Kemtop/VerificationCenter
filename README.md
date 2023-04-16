<h1 align="center">Система криптографической защиты информации «Гранит»</h1>

<h2 align="center">Краткое описание.</h2>

<p align="left">
Qt C++,C# приложение, разработанное с использованием IDE Visual Studio.<br>
</p>
<p align="left">
Читая данное описание вы соглашаетесь с MIT License.
</p>
<p align="left">
Автору известны существующие ошибки и недоработки в коде. Ведется переделка на .NET 6 и максимальное удаление Qt кода.
Ведется переделка на .NET 6 и максимальное удаление Qt кода.
</p>
<p align="left">
  Решение состоит из:
  <p align="left">
  -Удостоверяющий центр(УЦ,он же центр распределения ключей)
  </p>
  <p align="left">
   -Генератор закрытого ключа
  </p>
   <p align="left">
    -Программа шифрования файлов
  </p>
   <p align="left">
   -Аппаратный блок защиты УЦ(без него УЦ не запуститься)
  </p>
   <p align="left">
     -Dll библиотека реализующая все основные функции
    (и пример ее использования)
   </p>
   <p align="left">
   <a href="./Doc/RoseM/QuickQuide.md ">
     -Однокомпонентная программа шифрования данных "Роза" (проект EasyCript)
     </a>
  </p>
   <p align="left">
     -Аппаратный криптографически устойчивый генератор случайных чисел(на основе шума p-n перехода транзистора)
  </p>
 <br>
</p>  

<p align="left">
Удостоверяющий центр, предназначен для создания пользовательских сертификатов, заменяет существующие платные удостоверяющие центры. Создает полноценные сертификаты, которые могут быть установлены в ОС Windows(при установке криптопровайдера поддерживающего ГОСТы).
</p>
<p align="left">
Для кодирования и декодирования данных используется гибридное шифрование.
Блочный шифр: ГОСТ 34.12- 2015, ГОСТ 34.13-2015, ассиметричный-RSA 4096, подпись файлов - ГОСТ 34.10-2012.<br>
</p> 
<p align="left">
  Проект не использует стороннюю библиотеку OpenSSL,или ее части. Это позволяет избавиться от известных(и скрытых) уязвимостей OpenSSL, и при необходимости реализовывать новые алгоритмы кодирования.
</p>  
<p align="left">
Для работы с большими числами использована библиотека 
mpir. Для чтения ASN1 и шифрования RSA используется 
CryptoPP.
</p>  
<p align="left">
 Контейнер закрытого ключа использует собственный формат, похожий на ASN1.
</p>  
<p align="left">
Аппаратный блок защиты представляет собой плату на микроконтроллере STM32, которая хранит зашифрованный(AES) ключ и взаимодействует с ПО. На основе ряда простых математических функций реализован генератор ключа и его проверка.
Без блока удостоверяющий центр не запустится.
</p>  
<p align="left">
В рамках данной системы приведен упрощенный функционал по защите клиентского ПО от не санкционированного использования. Приведен генератор серийного номера и библиотека для его использования. 
</p>
<p align="left">
Приведенный аппаратный криптографически устойчивый генератор случайных чисел не позволяет его напрямую подключить к существующему программному комплексу. После подачи питания происходит непрерывная генерация данных в файл на flash накопитель подключенный к схеме генератора. 
Проект MonteCarloRndAnalyse предназначен для визуальной оценке работы генераторов случайных чисел методом Монте-Карло. Проект требует рефакторинга и предоставлен исключительно как визуальное средство анализа данных.
</p>
<h4 align="left">
ПО «Роза-М» может использовать сертификаты сгенерированные удостоверяющим центром или работать автономно. В этом случае программа автоматические генерирует ключ и само подписанный сертификат. В таком режиме программа позволяет защищать файлы с использованием указанных выше криптографических алгоритмов.
</h4>
</p>
<p align="left">
    <h2 align="center">Внешний вид ПО "Роза-M". <br><a href="./Doc/RoseM/QuickQuide.md ">Инструкция</a> <a href="./Binaries/Binaries.zip"><br>Portable версия</a> </h2>
<img src="./img_assets/Rose.png" width="100%">
Это самостоятельная программа(не требует наличилия УЦ). Достаточно сгенерировать ключ, придумать пароль и программа может кодировать и декодировать файлы используя гибридное шифрование(симметричное и ассиметричное).
</p> 

<p align="left">
<h2 align="center">Внешний вид УЦ "ГранитК".</h2>
<img src="./img_assets/UC1.png" width="100%">
<img src="./img_assets/UC2.png" width="100%">
Удостоверяющий центр генерирует сертификаты, ведет базу действующих и аннулированых сертификатов. Для работы УЦ требуется установка БД postgresql.
</p> 

<p align="left">
<h2 align="center">Аппаратный ключ защиты ПО.</h2>
<img src="./img_assets/hwkey1.jpg" width="100%">
<img src="./img_assets/hwkey.jpg" width="100%">
</p> 
<p align="left">
<h2 align="center">Аппаратный генератор случайных чисел</h2>
Используется эффект шума p-n перехода транзистора.
Плата с микроконтроллером снята с основной платы.
<img src="./img_assets/rand1.jpg" width="100%">
<img src="./img_assets/rand2.jpg" width="100%">
</p> 
<p align="left">
<h2 align="center">Часть лога git.</h2>
<img src="./img_assets/git.png" width="100%">
</p> 


<p align="left">
<b>
Это некоммерческий проект. Исходный код предоставлен исключительно для ознакомительных и учебных целей.
</b>
</p>


