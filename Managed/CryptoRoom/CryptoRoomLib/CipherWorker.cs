using CryptoRoomLib.AsymmetricCipher;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib
{
    /// <summary>
    /// Содержит методы для шифрования файлов.
    /// </summary>
    public class CipherWorker
    {
        /// <summary>
        /// Сервис для работы с секретным ключом пользователя.
        /// </summary>
        private readonly IKeyService _keyService;

        /// <summary>
        /// Режим работы блочного шифра.
        /// </summary>
        private readonly IBlockCipherMode _blockCipherMode;

        /// <summary>
        /// Последнее сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        public CipherWorker(IKeyService keyService, IBlockCipherMode blockCipherMode)
        {
            _keyService = keyService;
            _blockCipherMode = blockCipherMode; 
        }

        /// <summary>
        /// Расшифровывает файл.
        /// </summary>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
        /// <returns></returns>
        public bool DecryptingFile(string srcPath, string resultFileName, Action<ulong> setDataSize, Action<ulong> setMaxBlockCount,
            Action<ulong> endIteration)
        {
            ulong blockCount = 0;
            ulong blockNum = 0;
            ulong decryptDataSize = 0;

            return _blockCipherMode.DecryptData(srcPath, resultFileName, setDataSize, setMaxBlockCount, endIteration,
                //Чтение данных ассиметричной системы.
                (usefulDataSize, inFile) =>
                {
                    var asymmetricData = new AsDataReader();
                    asymmetricData.Read(inFile, usefulDataSize);
                    asymmetricData.CheckAll(); //Добавить возврат ошибки. Можно расспаралелить.

                    var sessionKey = asymmetricData.GetCryptedSessionKey();

                    if (sessionKey == null)
                    {
                        LastError = asymmetricData.Error;
                        return null; //В реальном коде вывести сообщение об ошибке.
                    }
                       
                    KeyDecoder kd = new KeyDecoder();
                    byte[] decryptKey;

                    var decryptResult = kd.DecryptSessionKey(_keyService.GetPrivateAsymmetricKey(), sessionKey, out decryptKey);

                    //В реальном коде вывести сообщение об ошибке.
                    if (!decryptResult)
                    {
                        LastError = kd.Error;
                        return null;
                    }

                    return decryptKey;
                }
            );
        }
    }
}
