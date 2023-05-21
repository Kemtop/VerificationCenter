﻿using CryptoRoomLib.AsymmetricCipher;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.KeyGenerator
{
    /// <summary>
    /// Генерирует закрытый ключ шифрования.
    /// </summary>
    public class SecretKeyMaker
    {
        /// <summary>
        /// Формирует секретный ключ, без генерации запроса на получение сертификата.
        /// </summary>
        public bool CreateKeyFileNoReq()
        {
            SecretKeyContainer container = new SecretKeyContainer();

            var settings = SettingsLib.Settings;

            container.KeyVersion = settings.KeyVersion;
            container.KeyGenVersion = settings.KeyGenVersion;

            container.OrgName = settings.OrgName;
            container.Department = settings.Department;
            container.OrgCode = settings.Department;
            container.Familia = settings.Familia;
            container.Imia = settings.Imia;
            container.Otchestvo = settings.Otchestvo;
            container.PhoneNumber = settings.PhoneNumber;

            container.DateBegin = DateTime.Now;
            container.DateEnd = DateTime.Now.AddYears(1);
            
            CreateRSAKeyPair(container, 4096);
            
            return true;
        }

        /// <summary>
        /// Генерация ключевой пары для накладывания и проверки подписи.
        /// </summary>
        /// <param name="container"></param>
        private void СreateSignKey(SecretKeyContainer container)
        {

        }

        /// <summary>
        /// Шифрует секретный ключ проверки подписи  ГОСТ 34.11-2012 на хэши.
        /// </summary>
        public void CryptEcSecretKey(byte[] cipherKey, byte[] iv, byte[] ecKey)
        {
            
        }


        /// <summary>
        /// Создает ключевые паря для алгоритма RSA.
        /// </summary>
        private void CreateRSAKeyPair(SecretKeyContainer container, int keySize)
        {
            using (RSA rsa = RSA.Create())
            {
                rsa.KeySize = keySize;

                var privateKey = rsa.ExportPkcs8PrivateKey();
                var publicKey = rsa.ExportSubjectPublicKeyInfo();
            }
        }
    }
}
