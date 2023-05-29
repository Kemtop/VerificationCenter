using CryptoRoomLib.Cipher3412;
using CryptoRoomLib.CipherMode3413;
using CryptoRoomLib;
using CryptoRoomLib.KeyGenerator;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VanishBox
{
    public class CipherService : ICipherService
    {
        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        private readonly KeyService _keyService;

        public CipherService()
        {
            _keyService = new KeyService();
        }

        /// <summary>
        /// Проверяет пароль для ключа.
        /// </summary>
        /// <returns></returns>
        public bool CheckPassword(string password, string pathToKey)
        {
            
            var res = _keyService.LoadKey(pathToKey);
            if (!res)
            {
                LastError = _keyService.LastError;
                return false;
            }

            res = _keyService.CheckPassword(password);
            if (!res)
            {
                LastError = _keyService.LastError;
                return false;
            }

            return true;
        }

        /// <summary>
        /// Шифрует/расшифровывает файлы.
        /// </summary>
        /// <param name="paths"></param>
        /// <param name="direction"></param>
        /// <param name="sendInfo"></param>
        /// <param name="progressIteration"></param>
        /// <param name="textIteration"></param>
        /// <returns></returns>
        public bool RunOperation(string[] paths, bool direction, Action<string> sendInfo, 
            Action<int> progressIteration, Action<string> textIteration)
        {
            ulong blockCount = 0;
            ulong blockNum = 0;
            ulong decryptDataSize = 0;

            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            IBlockCipherMode cipherMode = new ModeCBC(algoritm);

            CipherWorker worker = new CipherWorker(_keyService, cipherMode);
            
            foreach (var file in paths)
            {
                textIteration($"Расшифровывание файла {file}");

                bool result = worker.DecryptingFile(file, "", "Test1.jpg",
                    (size) => { decryptDataSize = size; },
                    (max) => { blockCount = max; },
                    (number) =>
                    {
                        double progress = ((double)number / blockCount) * 100;
                        progressIteration((int)progress);
                    });

            }

            return true;
        }
    }
}
