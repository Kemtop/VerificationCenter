//Для запуска программы распакуйте Qt библиотеки Binaries\QtLibs.zip в \bin\Debug\net6.0\

using ManagedAdapter;

const string pathToSecretKey = "SecretKey.key"; //Путь к файлу ключевого контейнера.
const string passwordToUserKey = "12345678"; //Пароль к контейнеру секретного ключа.

const string yoursDirPath = ".\\BestCatFotos"; //Ваша папка с файлами которые Вы хотите зашифровать.
const string pathToCryptDir = ".\\Encrypted_files"; //Куда будут сохранены зашифрованные файлы.
const string pathToDecryptDir = ".\\Decrypted_files"; //Куда будут сохранены расшифрованные файлы.
const string fileName = "cat1.jpg";
const string cryptFileName = "cat1.jpg.crypt";


Console.WriteLine("-----Пример работы с библиотекой----");

IManagedAdapter adapter = new CipherAdapterImpl(); //Взаимодействие с Qt библиотекой.

//Объект знающий как использовать функции библиотеки для достижения результата.
ICipherApi cipherApi = new CipherApiImpl(adapter, pathToSecretKey, passwordToUserKey);

Console.WriteLine($"Шифруем файл {fileName}");

if (!cipherApi.CryptlFile(Path.Combine(yoursDirPath, fileName) , pathToCryptDir))
{
    Console.WriteLine($"При шифровании файла возникла ошибка {cipherApi.LastError}");
    return;
}

Console.WriteLine($"Успешно зашифрован. Результат сохранен в {pathToCryptDir}");
Console.WriteLine($"Расшифровываю файл  {cryptFileName}");

if (!cipherApi.DecCryptlFile(Path.Combine(pathToCryptDir, cryptFileName), pathToDecryptDir))
{
    Console.WriteLine($"Возникла ошибка {cipherApi.LastError}");
    return;
}

Console.WriteLine($"Успешно. Результат сохранен в {pathToDecryptDir}");

Console.WriteLine($"Кодирую все файлы из {yoursDirPath}");

if (!cipherApi.CryptlFiles(yoursDirPath, pathToCryptDir, PrintProcess))
{
    Console.WriteLine($"Возникла ошибка {cipherApi.LastError}");
    return;
}

Console.WriteLine($"Декодирую все файлы из {pathToDecryptDir}");

if (!cipherApi.DecryptFiles(pathToCryptDir, pathToDecryptDir, PrintProcess))
{
    Console.WriteLine($"Возникла ошибка {cipherApi.LastError}");
    return;
}

Console.WriteLine($"Поздравляю! Все коты в безопасности. Посмотреть на них можно в {pathToDecryptDir}");

void PrintProcess(string file)
{
    Console.WriteLine($"Обработан файл {Path.GetFileName(file)}");
}