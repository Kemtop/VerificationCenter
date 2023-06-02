using CryptoRoomLib.AsymmetricInformation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CryptoRoomLib.Sign;
using System.Security.Cryptography;
using System.Drawing;
using System.Numerics;
using CryptoRoomLib.Models;

namespace CryptoRoomLib
{
    /// <summary>
    /// Содержит методы для шифрования файлов.
    /// </summary>
    public class CipherWorker
    {
        /// <summary>
        /// Режим работы блочного шифра.
        /// </summary>
        private readonly IBlockCipherMode _blockCipherMode;

        /// <summary>
        /// Последнее сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        public CipherWorker(IBlockCipherMode blockCipherMode)
        {
            _blockCipherMode = blockCipherMode; 
        }

        /// <summary>
        /// Расшифровывает файл.
        /// </summary>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
        /// <returns></returns>
        public bool DecryptingFile(string srcPath, string resultFileName, byte[] privateAsymmetricKey,
            string ecOid, EcPoint ecPublicKey,
            Action<ulong> setDataSize, Action<ulong> setMaxBlockCount, Action<ulong> endIteration,
            Action<string> sendProcessText)
        {
            var commonInfo = FileFormat.ReadFileInfo(srcPath);
            if (commonInfo == null)
            {
                LastError = FileFormat.LastError;
                return false;
            }

            KeyDecoder kd = new KeyDecoder();
            byte[] decryptKey;

            var decryptResult = kd.DecryptSessionKey(privateAsymmetricKey,  commonInfo.CryptedSessionKey,
                out decryptKey);

            commonInfo.SessionKey = decryptKey;

            //В реальном коде вывести сообщение об ошибке.
            if (!decryptResult)
            {
                LastError = kd.Error;
                return false;
            }

            sendProcessText("Проверка подписи");
            var signTools = new SignTools();
            if (!signTools.CheckSign(srcPath, commonInfo, ecOid, ecPublicKey))
            {
                LastError = signTools.LastError;
                return false;
            }

            sendProcessText("Расшифровывание файла");
            bool result = _blockCipherMode.DecryptData(srcPath, resultFileName, commonInfo,
                setDataSize, setMaxBlockCount, endIteration);

            if (!result)
            {
                LastError = _blockCipherMode.LastError;
                return false;
            }

            return true;
        }

        /// <summary>
        /// Шифрует файл.
        /// </summary>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="ecOid">Идентификатор эллиптической кривой.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
        /// <returns></returns>
        public bool CryptingFile(string srcPath, string resultFileName, byte[] publicAsymmetricKey, string ecOid, byte[] signPrivateKey, EcPoint ecPublicKey,
            Action<ulong> setDataSize, Action<ulong> setMaxBlockCount,
            Action<ulong> endIteration, Action<string> sendMessage, Action<int> signProcessStatus)
        {
            CommonFileInfo info = new CommonFileInfo();

            FileInfo fi = new FileInfo(srcPath);
            info.FileLength = (ulong)fi.Length; //Размер исходного файла в байтах.
            
            info.FileHead = FileFormat.CreateCryptFileTitle(info.FileLength, _blockCipherMode.Algoritm);
            info.SessionKey = CipherTools.GenerateRand(32); //Формирую случайное число размером 32байта, которое является сеансовым ключом.
            KeyDecoder kd = new KeyDecoder();
            byte[] cryptKey;

            var encryptResult = kd.EncryptSessionKey(publicAsymmetricKey, info.SessionKey, out cryptKey);
            if (!encryptResult)
            {
                LastError = kd.Error;
                return false;
            }

            //Сведения о шифрованном сеансовом ключе, в виде блоков ASN1.
            var asWriter = new AsDataWriter();
            asWriter.AddRsaHash(new byte[37]);
            asWriter.AddCryptedBlockKey(cryptKey);

            info.BlockData = asWriter.GetData();

            bool result = _blockCipherMode.CryptData(srcPath, resultFileName, info, 
                setDataSize, setMaxBlockCount, endIteration);

            if (!result)
            {
                LastError = _blockCipherMode.LastError;
                return false;
            }

            var signTools = new SignTools();
            if (!signTools.SignFile(resultFileName, sendMessage, ecOid, signPrivateKey, ecPublicKey))
            {
                LastError = signTools.LastError;
                return false;
            }

            return true;
        }
    }
}
