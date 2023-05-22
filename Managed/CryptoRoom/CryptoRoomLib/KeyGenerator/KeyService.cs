using System;
using System.Collections.Generic;
using System.Data.SqlTypes;
using System.Diagnostics.Metrics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.KeyGenerator
{
    /// <summary>
    /// Содержит все необходимые методы для чтения секретного ключа(файл который храниться у пользователя) системы.
    /// </summary>
    public class KeyService
    {
        /// <summary>
        /// Последнее сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        /// <summary>
        /// Считывает контейнер ключа из файла. Расшифровывает его, помещает в объект ключа KeyContainer.
        /// </summary>
        /// <param name="pathToSecretKey"></param>
        /// <returns></returns>
        public bool LoadKey(string pathToSecretKey)
        {
            //Проверка существования файла
            if (!File.Exists(pathToSecretKey))
            {
                LastError = $"ОШИБКА Л0:Не удалось найти файл ключа по указанному пути: {pathToSecretKey}. Убедитесь что флеш накопитель вставлен в компьютер.";
                return false;
            }

            //Проверка секретного ключа и получение контейнера.
            if (!CheckSK(pathToSecretKey, out byte[] container)) return false;
            
            return true;
        }

        /// <summary>
        /// Проверяет секретный ключ, в случае ошибки возвращает false.
        /// Проверка поверхностная. Проверяется соответствие заголовка и контрольной суммы контейнера ключа
        /// Возвращает размер контейнера ключа container_length
        /// </summary>
        /// <param name="pathToSecretKey"></param>
        /// <returns></returns>
        private bool CheckSK(string pathToSecretKey, out byte[] container)
        {
            container = new byte[0];

            FileInfo fileInfo = new FileInfo(pathToSecretKey);
            long fileSize = fileInfo.Length;

            if (fileSize == 0)
            {
                LastError = "ОШИБКА Р0:Не удалось открыть файл секретного ключа.";
                return false;
            }

            if (fileSize < SecretKeyMaker.KeyHeadLen)
            {
                LastError = "ОШИБКА Р1:Не корректный размер файла секретного ключа.";
                return false;
            }
            
            try
            {
                byte[] title = new byte[SecretKeyMaker.KeyHeadLen];
                
                using (var fs = new FileStream(pathToSecretKey, FileMode.Open))
                {
                    fs.Read(title, 0, SecretKeyMaker.KeyHeadLen);

                    //Проверка правильности заголовка
                    int containerLen = CheckHeadFile(title, fileSize); //Длина контейнера ключа.
                                           
                    if (containerLen == 0)
                    {
                        fs.Close();
                        return false;
                    }

                    //Читаю содержимое контейнера.
                    container = new byte[containerLen];
                    fs.Read(container, 0, containerLen);

                    if(!CheckContainerHash( container, title))
                    {
                        fs.Close();
                        return false;
                    }

                    fs.Close();
                }
            }
            catch (Exception e)
            {
                LastError = $"ОШИБКА Р2:Не удалось считать файл секретного ключа. Исключение:{e.Message}";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Проверяет заголовок файла. Возвращает длину контейнера.
        /// </summary>
        /// <returns></returns>
        private int CheckHeadFile(byte[] title, long fileSize)
        {
            //[7-байт служебной информации]
            byte[] headText = new byte[SecretKeyMaker.KeyHeadText.Length];
            Buffer.BlockCopy(title, 0, headText, 0, SecretKeyMaker.KeyHeadText.Length);

            if (!headText.SequenceEqual(SecretKeyMaker.KeyHeadText))
            {
                LastError = "ОШИБКА Р4: Файл секретного ключа не предназначен для данной системы.";
                return 0;
            }

            //[Заголовок 47байт]
            byte[] programNameText = new byte[SecretKeyMaker.ProgramText.Length];
            Buffer.BlockCopy(title, SecretKeyMaker.KeyHeadText.Length, programNameText, 0, SecretKeyMaker.ProgramText.Length);

            byte[] programNameTextEtalon = Encoding.ASCII.GetBytes(SecretKeyMaker.ProgramText);

            if (!programNameText.SequenceEqual(programNameTextEtalon))
            {
                LastError = "ОШИБКА Р5: Файл секретного ключа не предназначен для данной системы.";
                return 0;
            }

            //Проверка длины блока контейнера
            byte[] containerLenArr = new byte[SecretKeyMaker.KeyContainerLen];
            Buffer.BlockCopy(title, SecretKeyMaker.KeyHeadLen - SecretKeyMaker.KeyContainerLen, containerLenArr, 0, SecretKeyMaker.KeyContainerLen);

            int containerLen = BitConverter.ToInt32(containerLenArr);

            //Длина ключа не может быть менее чем заголовок + контейнер.
            if (fileSize < SecretKeyMaker.KeyHeadLen + containerLen)
            {
                LastError = "ОШИБКА Р6:Не верный размер файла секретного ключа.";
                return 0;
            }

            return containerLen;
        }

        /// <summary>
        /// Проверяет не был ли поврежден(изменен) контейнер.
        /// Вычисляет хеш от данных контейнера и сравнивает с той что записана в заголовке файла.
        /// </summary>
        /// <param name="container"></param>
        /// <param name="title"></param>
        /// <returns></returns>
        private bool CheckContainerHash(byte[] container, byte[] title)
        {
            //Вычисляет хэш256 контейнера.
            var hasher = new Hash3411.Hash3411();
            byte[] hashContainer = new byte[Hash3411.Hash3411.Hash256Size];

            hasher.Hash256(container, hashContainer);
            
            byte[] hashFromHead = new byte[Hash3411.Hash3411.Hash256Size];

            int hashPos = SecretKeyMaker.KeyHeadLen - SecretKeyMaker.KeyContainerLen - Hash3411.Hash3411.Hash256Size;
            Buffer.BlockCopy(title, hashPos, hashFromHead, 0,Hash3411.Hash3411.Hash256Size);

            if (!hashFromHead.SequenceEqual(hashContainer))
            {
                LastError = "ОШИБКА Р9: Ошибка контрольной суммы. Файл поврежден или был изменен третьим лицом.";
                return false;
            }

            return true;
        }
    }
}
