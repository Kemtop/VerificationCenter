<h1 align="center">Система криптографической защиты информации «Гранит»</h1>

<h2 align="center">Краткое описание.</h2>

<p align="left">
Qt C++,C#(.Net 6) проект.<br>
</p>
<p align="left">
Читая данное описание вы соглашаетесь с MIT License.
</p>
<p align="left">
Краткое описание.
</p>
<p align="left">
Проект предназначен для развертывание защищенной сети передачи данных(группа серверов, терминалы передачи данных, клиентское ПО), защиты личных данных, криптографических исследованиях.
</p>
<p align="left">
Автору всегда хотелось написать собственную систему защиты информации.
</p>
<p align="left">
В приведенных исходниках вы можете найти собственную реализацию крипто алгоритмов
(блочный шифр: ГОСТ 34.12- 2015, ГОСТ 34.13-2015, ассиметричный-RSA 4096, подпись файлов - ГОСТ 34.10-2012).
</p>
<p align="left">
Проект не использует стороннюю библиотеку OpenSSL, GPG, или ее части. Это позволяет избавиться от известных (и скрытых) уязвимостей, и при необходимости реализовывать новые алгоритмы кодирования.
</p>
<p align="left">
Основная цель проекта-простота и уверенность в отсутствии Backdoor’s. Прозрачный код. Проект был реализован после глубокого изучения существующих систем с открытым исходным кодом. 
</p>
<p align="left">
Ведется перенос С++ кода на .Net и Rust. 
Разрабатывается облачное хранилище.
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
  -Удостоверяющий центр(CA)
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
   -Генератор закрытого ключа
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
    -Программа шифрования файлов
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
   -Аппаратный блок защиты УЦ(без него УЦ не запуститься)
   </strong>
  </p>
  <i>
  <p>
  Плата с 32 битными микроконтроллером stm32f103c8t6 , которая подключается к ПК и не позволяет запустить удостоверяющий центр без данного аппаратного ключа. Идея в передачи шифрованных данных между внешним аппаратным блоком, и отдельной библиотекой встроенной в ПО.
  <p>
  В рамках данной задачи применялась «обфускация» asm кода собранного Qt приложения, и добавление цепочек пустышек.
  </p>
  <p>
  Это позволяет серьезно усложнить работу вредоносным программам, и несанкционированного запуска УЦ.
  </p>
  <p>
  Код аппаратного ключа представлен в HardwareProjects\STM32UsbKey.  Остатки библиотеки защиты ПО окончательно удалены из исходных кодов. Возможно в будущем будут вынесены в отдельный проект.
  </p>
  </i>
  <p>
  <a href="./Doc/HardKey/Key.md">Внешний вид</a>
  </p>
  <br>
  <p align="left">
     <strong>
     -Dll библиотека реализующая все основные функции
    (и пример ее использования)
     </strong>
   </p>
   <p>
   <i>
    Сама с++ dll находиться в \Native\LibGranitK.
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
     -Однокомпонентная программа шифрования данных "Роза"
   </strong>
  </p>
  <p>
  Предназначена для работы без УЦ. Используя её, вы можете уже сейчас зашифровать ваши файлы с фотографиями, и поместить их на Amazon Drive. И быть уверенными что даже, если злоумышленники взломают ваше облако-то кроме последовательности байт они ни чего там не увидят.
  </p>
  <p>
  Применение не стандартного формата данных, ГОСТ, и изменений в текущей реализации алгоритмов-заставит злоумышленника отказаться от задачи расшифровки ваших фотографий.
  </p>
  <p>
    Исходники- Qt \Native\EasyCript\. 
  </p>
  <p> 
   <a href="./Doc/RoseM/QuickQuide.md ">Инструкция к программе</a>
   <a href="./Binaries/Binaries.zip"><br>Рабочая portable версия</a>
  </p>
  <br>
   <p align="left">
   <strong>
     -Аппаратный криптографически устойчивый генератор случайных чисел(на основе шума p-n перехода транзистора)
   </strong>
  </p>
  <i>
  <p>
  Приведенный аппаратный криптографически устойчивый генератор случайных чисел не позволяет его напрямую подключить к существующему программному комплексу. После подачи питания происходит непрерывная генерация данных в файл на flash накопитель подключенный к схеме генератора. 
  </p>
  <p>
   Используя существующий исходный код и схемо технику-вы можете разработать свой ГСПЧ и использовать его в разных исследовательских работах.
  </p>
  </i>
  <p align="left">
  Исходники(Keil)-\HardwareProjects\HwRandom <br>
 <a href="./Doc/Rnd/RndHw.md">Внешний вид</a>
  </p>
 <br>
</p>
<p align="left">
   <strong>
     -Приложения для анализа ГПСЧ методом Монте-Карло
   </strong>
</p>
<p>
<i>
Простое приложение для анализа возможности использование данного ГСПЧ в криптографии.
Первоначальная версия реализована на Winforms, ведется переделка на web.
Данное приложение позволяет вам увидеть насколько хороший ваш ГСПЧ.
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
Автор не несет ответственности за использование данного кода в любых целях, неудобствах в его работе. Код предоставляется «так как есть». 
</p>


