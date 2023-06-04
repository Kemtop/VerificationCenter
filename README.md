<h1 align="center">Система криптографической защиты информации «Гранит»</h1>

<h2 align="center">Краткое описание.</h2>

<p align="left">
C++ (Qt), C#(.Net 6) проект. Разработка приведенной С++ реализации закончена в 2019 году и более не поддерживается.<br>
</p>
<p align="left">
Читая данное описание вы соглашаетесь с MIT License.
</p>
<p align="left">
Проект предназначен для развертывания защищенной сети передачи данных(группа серверов, терминалы передачи данных, клиентское ПО), защиты личных данных, криптографических исследованиях.
</p>
<p align="left">
Автору всегда хотелось написать собственную систему защиты информации.
</p>
<p align="left">
В приведенных исходниках вы можете найти собственную реализацию крипто алгоритмов
(блочный шифр: ГОСТ 34.12 - 2015, <a href="./Doc/CipherMode/CipherMode.md">ГОСТ 34.13-2015</a>, ассиметричный-<a href="./Doc/RSA/Rsa.md">RSA 4096</a>, подпись файлов - ГОСТ 34.10-2012,
 <a href="./Doc/HashFunc/hash3411.md">ГОСТ 34.11-2012</a> - функция хеширования).
</p>
<p align="left">
Проект не использует стороннюю библиотеку OpenSSL, GPG, или ее части. Это позволяет избавиться от известных (и скрытых) уязвимостей, и при необходимости реализовывать новые алгоритмы кодирования.
</p>
<p align="left">
Основная цель проекта-простота и уверенность в отсутствии Backdoor’s. Прозрачный код. Проект был реализован после глубокого изучения существующих систем с открытым исходным кодом. 
</p>
<p align="left">
Ведется перенос С++ кода на .Net. 
Разрабатывается <a href="./Doc/CloudStorage/CloudStorage.md">облачное хранилище.</a>
</p>
<p align="left">
На данный момент средняя скорость декодирования блочного шифра в .Net реализации, составляет 2 410 183 байт/сек, или 2,3Мбайт/с при чтении и записи с hdd. 
</p>
<p align="left">
2 869 266 байт/сек или 2,73Мбайт/с при чтении и записи на ssd nvme.
</p>
<p align="left">
Замеры указаны при декорировании файла размером 6Мбайт.
</p>
<p>
Qt реализация работает со скоростью 2 738 844 байт/сек или 2,61 Мбайт/с (на hdd).
</p>
<p align="left">
<a href="./Doc/DirInfo.md">
Содержание директорий репозитория.
</a>
<hr>
<p align="left">
Проект состоит из:
  <p align="left">
  <strong>
  Удостоверяющий центр(CA)
  </strong>
  </p>
  <p><i>
  Qt приложение работающее с БД Postgresql. Реализующее полноценный функционал удостоверяющего центра. Получает от пользователя запросы на создание сертификатов, создает сертификаты, формирует базу сертификатов. Используется собственный алгоритм создания сертификатов по протоколу ASN1.
  Полученные сертификаты возможно устанавливать в ОС Windows.
  </p>
  <p> 
  В исходниках не представлен(мало вероятно что вам потребуется создавать развернутую сеть пользователей) . Есть идеи о переносе на .Net.
  </p>
  <p>
   <a href="./Doc/CA/CA.md">Внешний вид УЦ</a>
  </p>
  </i>
  <br>
   <p align="left">
  <strong>
   Генератор закрытого ключа
  </strong>
  </p>
  <p><i>
  Используя биологический генератор случайных чисел(см. <a href="./Doc/RoseM/QuickQuide.md">инструкцию к программе</a>), формирует закрытый ключ для ассиметричной системы шифрование, и помещает в секретный ключевой контейнер, защищенный паролем. Используется свой формат контейнера, не совместимый c RFC стандартами.
  <p>
  Исходники Qt находятся в  \Native\PrivateKeyGen
  </p>
  <br>

  </p></i>
   <p align="left">
    <strong>
    Программа шифрования файлов
    </strong>
  </p>
  <p><i>
  Полноценная версия Qt программы, позволяющая шифровать и расшифровывать файлы полученный из распределенной сети. Использует БД  доверенных сертификатов(сформированную УЦ и размещенную в открытом источнике). Имеет механизм создания запросов на получение сертификатов в УЦ.
  </p>
  <p>
  Qt исходники удалены из текущего репозитория.  
  </p></i>
  <br>

   <p align="left">
   <strong>
   Аппаратно - программный комплекс  защиты программ
   </strong>
  </p>
  <i>
  <p>
  Плата с 32 битными микроконтроллером stm32f103c8t6, которая подключается к ПК и не позволяет войти в программу без аппаратного ключа. К компьютеру подключается посредством usb кабеля.
  </p>
  <p>
  </p>
  <p>
  Ключ и серийный номер продукта вшивается в плату,  файл ключа помещается рядом с программой. Программа запрашивает шифрованный AES, RSA серийный номер, и проверяет его специальным алгоритмом. 
  </p>
  </i>
  <p>
  <a href="./Doc/HardKey/Key.md">Описание системы</a>
  </p>
  <p>
   Исходники аппаратного ключа - HardwareProjects\STM32UsbKey<br> 
   Пример использования в Qt приложении - \Native\HardwareKeyExample\<br> 
  </p>
  <br>
  <p align="left">
     <strong>
     Dll библиотека реализующая все основные функции
    (и пример ее использования)
     </strong>
   </p>
   <p>
   <i>
    <p>
     .Net библиотека(1% перенесен из Qt) - \Managed\CryptoRoom
    </p>
    <p>
    С++ dll находиться в \Native\LibGranitK.
    </p>
    <p>
    Частично реализована .Net 6 обвёртка над ней 
    <a href="./Doc/LibExample/LibExample.md">
    \Managed\LibExample\ </a>
    </p>
    <p>
     Ведется оптимизация кода и перенос на .Net
    </p>
   </i>
   <br>
   <p align="left">
   <strong>
     Однокомпонентные программы шифрования данных "Роза" и "Vanish Box" 
   </strong>
  </p>
  <p>
  <p>
  "Роза" написана на C++ c использованием Qt framework. Разработка окончена в 2019 году, больше не поддерживается.<br>
 "Vanish Box" – .Net 6 реализация с использованием Avalonia UI. Ведется разработка.
  </p>
  Любая из этих программ предназначена для работы без удостоверяющего центра (УЦ). Не нужен сертификат заверенный УЦ и БД сертификатов получателя. Используя её, вы можете уже сейчас зашифровать ваши файлы с фотографиями, и поместить их на Amazon Drive. И быть уверенными что даже, если злоумышленники взломают ваше облако - то кроме последовательности байт они ни чего там не увидят.
  </p>
  <p>
  Применение не стандартного формата данных, гибридного шифрования, ГОСТ, и изменений в текущей реализации алгоритмов - заставит злоумышленника отказаться от задачи расшифровки ваших фотографий.
  </p>
  <p>
    Исходники  "Роза" - \Native\EasyCript\<br> 
    Исходники  "Vanish Box" - \Managed\CryptoRoom\VanishBox<br> 
  </p>
  <p> 
   <a href="./Doc/RoseM/QuickQuide.md ">Инструкция к программе "Роза"</a>
   <a href="./Binaries/Binaries.zip"><br>Рабочая "portable" версия "Роза"(ver 2019)</a>
  </p>
  <br>
   <p align="left">
   <strong>
     Аппаратный криптографически устойчивый генератор случайных чисел(на основе шума p-n перехода транзистора)
   </strong>
  </p>
  <i>
  <p>
  Основная цель внешнего ГПСЧ – это исключение атак на операционную систему и внедрение вредоносного кода в средства генерации случайных чисел. 
  </p>
  <p>
 Приведенный исходный код генератора("прошивка"), не позволяет его напрямую подключить к компьютеру через USB порт. После подачи питания происходит непрерывная генерация данных в файл на flash накопитель подключенный к схеме генератора. 
  </p>
  <p>
   Используя существующий исходный код и схемо технику-вы можете разработать свой ГСЧ и использовать его в разных исследовательских работах. Аналоговая часть может быть использована для измерения АЧХ.
  </p>
  </i>
  <p align="left">
  Исходники(Keil)-\HardwareProjects\HwRandom <br>
  <p>
  <a href="./Doc/Rnd/RndTests.md">Результаты тестов</a>
  </p>
 <a href="./Doc/Rnd/RndHw.md">Внешний вид</a>
  </p>
 <br>
</p>
<p align="left">
   <strong>
     Приложения для анализа ГПСЧ методом Монте-Карло
   </strong>
</p>
<p>
<i>
Простое приложение для анализа возможности использование данного ГПСЧ в криптографии.
Первоначальная версия реализована на Winforms, ведется переделка на web.
Данное приложение позволяет вам увидеть насколько хороший ваш ГПСЧ.
</i>
<p>
<a href="./Doc/Rnd/Rnd.md">Внешний вид работы</a>
</p>
</p>
<br>
<p align="left">
</p>
<br>
<b>
<p align="left">
Это некоммерческий проект. Исходный код предоставлен исключительно для ознакомительных и учебных целей. 
</p>


