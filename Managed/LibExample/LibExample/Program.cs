//Для запуска программы распакуйте Qt библиотеки Binaries\QtLibs.zip в \bin\Debug\net6.0\

using ManagedAdapter;

const string pathToSecretKey = "SecretKey.key"; //Путь к файлу ключевого контейнера.
const string passwordToUserKey = "12345678"; //Пароль к контейнеру секретного ключа.

//Ваша папка с файлами которые Вы хотите зашифровать.
string yoursDirPath = ".\\BestCatFotos";

Console.WriteLine("-----Пример работы с библиотекой----");

IManagedAdapter adapter = new CipherAdapterImpl(); //Взаимодействие с Qt библиотекой.

//Объект знающий как использовать функции библиотеки для достижения результата.
ICipherApi cipherApi = new CipherApiImpl(adapter, pathToSecretKey, passwordToUserKey);

if (!cipherApi.CryptlFile(yoursDirPath + "\\cat1.jpg"))
{
    Console.WriteLine($"При шифровании файла возникла ошибка {cipherApi.LastError}");
    return;
}

Console.WriteLine("-----Успешно----");


