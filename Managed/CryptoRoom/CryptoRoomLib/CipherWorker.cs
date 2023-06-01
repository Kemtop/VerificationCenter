using CryptoRoomLib.AsymmetricCipher;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CryptoRoomLib.Sign;
using System.Security.Cryptography;
using System.Drawing;
using System.Numerics;

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
        /// Размер файла в байтах, который будет подписан. Свыше указанного размера – файл подписан не будет (операция будет выполняться долго).
        /// </summary>
        private const int SignAllowSize = 1900000000;

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

        /// <summary>
        /// Шифрует файл.
        /// </summary>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="ecOid">Идентификатор эллиптической кривой.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
        /// <returns></returns>
        public bool CryptingFile(string srcPath, string resultFileName, string ecOid, byte[] signPrivateKey,
            Action<ulong> setDataSize, Action<ulong> setMaxBlockCount,
            Action<ulong> endIteration, Action<string> sendMessage, Action<int> signProcessStatus)
        {
            ulong blockCount = 0;
            ulong blockNum = 0;
            ulong decryptDataSize = 0;

            bool result = _blockCipherMode.CryptData(srcPath, resultFileName, setDataSize, setMaxBlockCount, endIteration,
                //Чтение данных ассиметричной системы.
                (sessionKey) =>
                {
                    KeyDecoder kd = new KeyDecoder();
                    byte[] cryptKey;

                    var encryptResult = kd.EncryptSessionKey(_keyService.GetPublicAsymmetricKey(), sessionKey, out cryptKey);
                    if (!encryptResult)
                    {
                        LastError = kd.Error;
                        return null;
                    }

                    var asWriter = new AsDataWriter();
                    asWriter.AddRsaHash(new byte[37]);
                    asWriter.AddCryptedBlockKey(cryptKey);

                    return asWriter.GetData();
                }
            );

            if (!result) return false;

            if (!SignFile(resultFileName, sendMessage, ecOid, signPrivateKey, signProcessStatus)) return false;

            return true;
        }

        /// <summary>
        /// Подпись файла.
        /// </summary>
        /// <param name="file"></param>
        /// <param name="sendMessage"></param>
        /// <returns></returns>
        private bool SignFile(string srcfile, Action<string> sendMessage, string ecOid, byte[] signPrivateKey, Action<int> signProcessStatus)
        {
            FileInfo fi = new FileInfo(srcfile);

            if (fi.Length > SignAllowSize)
            {
                sendMessage($"Файл  более {String.Format("{0:0.00}", (float)SignAllowSize / 1024 / 1024)}Мб, подписан не будет.");
                return true;
            }

            signProcessStatus(1); //Информирую UI о начале процесса подписи.
            
            using (FileStream inFile = new FileStream(srcfile, FileMode.Open, FileAccess.ReadWrite))
            {
                inFile.Seek(FileFormat.BeginDataBlock, SeekOrigin.Begin);
                byte[] fileData = new byte[fi.Length - FileFormat.BeginDataBlock];
                inFile.Read(fileData);

                var curve = EcGroups.GetCurve(ecOid);
                if (curve == null)
                {
                    LastError = "Ошибка Sg3.1: Не удалось определить параметры кривой.";
                    return false;
                }

                //Создаю точку с указанными в кривой координатами точки P
                EcPoint p = new EcPoint(curve, true);

                CreateSign signGen = new CreateSign();
                var sign = signGen.Sign(new BigInteger(signPrivateKey), fileData, p);

                var asWriter = new AsDataWriter();
                asWriter.SignR(sign.VectorR.ToArray().Select(c => (byte)c).ToArray());
                asWriter.SignS(sign.VectorR.ToArray().Select(c => (byte)c).ToArray());

                inFile.Write(asWriter.GetData());
                inFile.Close();
            }

            return true;
        }
    }
}
