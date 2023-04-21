using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ManagedAdapter
{
    public interface IManagedAdapter
    {
        /// <summary>
        /// Текст ошибки возникшей после вызова метода.
        /// </summary>
        string LastError { get; }

        /// <summary>
        /// Задает тип ключа, с которым работает система-работа без удостоверяющего центра.
        /// </summary>
        /// <returns></returns>
        bool SetSingleModeKeyType();

        /// <summary>
        /// Загружает секретный ключ из файла контейнера.
        /// </summary>
        /// <param name="pathToKey"></param>
        /// <returns></returns>
        bool LoadSecretKey(string pathToKey);

        /// <summary>
        /// Проверяет правильность пароля к секретному ключу пользователя.
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        bool CheckPasswordForContainer(string password);

        /// <summary>
        /// Загружает ассиметричный ключ шифрования.
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        bool LoadAsymmetricKey(string password);

        /// <summary>
        /// Инициализирует механизм шифрования.
        /// </summary>
        /// <returns></returns>
        public bool InitCipherWorker();

        /// <summary>
        /// Инициализирует механизм шифрования.
        /// </summary>
        /// <returns></returns>
        public bool ClearCipherWorker();

        /// <summary>
        /// Загружает ключ подписи с использованием эллиптической кривой
        /// </summary>
        /// <param name="password"></param>
        /// <returns></returns>
        bool LoadSignedSecretKey(string password);

        /// <summary>
        /// //Генерирует 32 байтное число(int256) использую псевдослучайный генератор чисел библиотеки.
        /// mode =1, возвращает пустой массив. 
        /// </summary>
        /// <returns></returns>
        (bool result, byte[] data) Rand256(byte mode);

        /// <summary>
        /// Шифрует файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        bool CryptlFile(string srcPath, string dstDir);

        /// <summary>
        /// Расшифровывает файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <param name="dstDir"></param>
        /// <param name="passwordToKey"></param>
        /// <returns></returns>
        bool DecryptlFile(string srcPath, string dstDir, string passwordToKey);
    }
}
