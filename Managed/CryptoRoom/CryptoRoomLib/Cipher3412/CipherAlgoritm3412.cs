﻿namespace CryptoRoomLib.Cipher3412
{
    /// <summary>
    /// Реализация алгоритма ГОСТ 34.12-2015. 
    /// </summary>
    internal class CipherAlgoritm3412 : ICipherAlgoritm
    {
        private ulong[] _roundKeys;

        public CipherAlgoritm3412()
        {
            _roundKeys = new ulong[20];
        }

        /// <summary>
        /// Развертывание раундовых ключей.
        /// </summary>
        public void DeployDecryptRoundKeys(byte[] key)
        {
            Logic3412.DeploymentDecryptionRoundKeys(key, _roundKeys);
        }

        /// <summary>
        /// Расшифровывает блок данных.
        /// </summary>
        /// <param name="block"></param>
        public void DecryptBlock(ref Block128t block)
        {
            U128t data = new U128t(block.Low, block.Hi);
            
            Logic3412.DecryptBlock(ref data, _roundKeys);

            block.Low = data.Low;
            block.Hi = data.Hi;
        }
    }
}