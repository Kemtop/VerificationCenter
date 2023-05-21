﻿using CryptoRoomLib.AsymmetricCipher;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Security.Cryptography;
using CryptoRoomLib.Hash3411;
using System.Threading.Tasks;
using System.Text;
using CryptoRoomLib.Cipher3412;
using CryptoRoomLib.CipherMode3413;
using Newtonsoft.Json;
using System.Xml;
using System.Xml.Serialization;
using CryptoRoomLib.Models;

namespace CryptoRoomLib.KeyGenerator
{
    /// <summary>
    /// Генерирует закрытый ключ шифрования.
    /// </summary>
    public class SecretKeyMaker
    {
        /// <summary>
        /// Размер соли для кодирование ключа подписи.
        /// </summary>
        private const int SignKeySaltLength = 17;

        /// <summary>
        /// Алгоритм открытого ключа подписи.
        /// </summary>
        private const string PublicKeySignAlgoritmOid = "1.2.643.7.1.1.1.2";

        /// <summary>
        /// Идентификатор эллиптической кривой на основании которой создается ключ.
        /// </summary>
        private const string EcOid = "1.2.643.7.1.2.1.2.2";

        /// <summary>
        /// Константа защиты ключевого контейнера.
        /// </summary>
        private const string KeyChipperConstant = "17dfbfc9acfa787e242d75c7f0764bfd83e79eef08d4581a881527d92dbad1d4";

        /// <summary>
        /// Размер заголовка ключа. 16(iv) + 4(длина шифрованного блока)
        /// </summary>
        private const int KeyTitleSize = 20;

        //Размер Заголовока файла-служебная информация [7-байт служебной информации][Заголовок 47байт][Хэш контента 256бит]
        //[Длина контейнера ключа 4байта][[iv 16байт][4байта длина контейнера]Контейнер]
        private const int KeyHeadLen = 90;

        /// <summary>
        /// Первые 7 байт содержащих что то вроде версии ключа.
        /// </summary>
        private byte[] KeyHeadText = new byte[7]{0xf9,0xc5, 0xa8, 0xd3, 0x47, 0xb6, 0x3a};

        /// <summary>
        /// Название программы для которой используется ключ.
        /// </summary>
        private const string ProgramText = "Little Rose radials security protection system ";

        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        /// <summary>
        /// Последнее возникшее исключение.
        /// </summary>
        private string _lastException = string.Empty;

        /// <summary>
        /// Формирует секретный ключ, без генерации запроса на получение сертификата.
        /// </summary>
        public bool CreateKeyFileNoReq(string password, string pathToSave)
        {
            System.Text.Encoding.RegisterProvider(System.Text.CodePagesEncodingProvider.Instance);
            byte[] passwordArray = Encoding.GetEncoding(1251).GetBytes(password);

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

            СreateSignKey(container, passwordArray);
            CreateRSAKeyPair(container, 4096);
            
            container.KeyGenTimeStamp = DateTime.Now; //Время и дата когда ключ был сгенерирован.

            //Дополнительные параметры для использования в будущем.
            container.ReservedParameter0 = "0";
            container.ReservedParameter1 = "1";

            KeyСontainerToFile(container, pathToSave);
            
            return true;
        }

        /// <summary>
        /// Генерация ключевой пары для накладывания и проверки подписи.
        /// </summary>
        /// <param name="container"></param>
        private void СreateSignKey(SecretKeyContainer container, byte[] password)
        {
            //var salt = CipherTools.GenerateRand(SignKeySaltLength);

            byte[] salt = new byte[]
            {
                0x10, 0xea, 0x2c, 0x88, 0x25, 0x96, 0xf3, 0x12,
                0x55, 0xaa, 0xd4, 0xca, 0x5c, 0x58, 0xe7, 0xea,
                0x4d,
            };

            byte[] hashPassword = new byte[Hash3411.Hash3411.Hash256Size];

            HashedPassword(salt, password, hashPassword);

            //Генерирую вектор инициализации
            byte[] iv = new byte[16]
            {
                0x81, 0x4a, 0x88, 0x25, 0xbd, 0xb2, 0x13, 0x7e,
                0x53, 0xd8, 0x4c, 0x2b, 0x70, 0x53, 0xc1, 0xec
            };

            //Закрытый ключ.
            byte[] sc = new byte[64]
            {
                0x70, 0x02, 0xfa, 0x00, 0x11, 0x72, 0x01, 0x01,
                0x3e, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00,
                0x80, 0xfd, 0xe2, 0x03, 0xe9, 0xab, 0x32, 0x77,
                0x80, 0xe7, 0xbd, 0x00, 0x98, 0x01, 0x00, 0x00,
                0xf0, 0xac, 0x54, 0x24, 0xfe, 0xff, 0xff, 0xff,
                0x04, 0xe7, 0xbd, 0x00, 0x00, 0x00, 0x00, 0x00,
                0x00, 0x00, 0x00, 0x00, 0x14, 0x7b, 0x35, 0x77,
                0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfa, 0x00
            };

            //Шифрованный на хеши от соли и пароля закрытый ключ.
            byte[] cryptSc = new byte[64]
            {
                0xf3, 0x68, 0x3e, 0x66, 0x53, 0xf6, 0x43, 0xa9,
                0xe8, 0x76, 0x7c, 0x73, 0x0c, 0x42, 0xdb, 0x60,
                0xc0, 0x20, 0x0f, 0x56, 0xa9, 0xf1, 0x9d, 0x51,
                0x88, 0xec, 0x87, 0x07, 0x53, 0xac, 0xfb, 0xb6,
                0x96, 0x0e, 0x53, 0x85, 0x38, 0x20, 0x1c, 0xf6,
                0x32, 0x2e, 0x32, 0x04, 0x4f, 0xb2, 0x07, 0x1c,
                0x2e, 0x82, 0x79, 0x20, 0xb0, 0x0b, 0xa0, 0xda,
                0xa4, 0x53, 0x6b, 0x8a, 0xdf, 0xe7, 0xeb, 0x5d
            };


            CryptEcSecretKey(hashPassword, iv, sc);

            container.CryptSignKey = Convert.ToHexString(sc).ToLower();
            container.IvCryptSignKey = Convert.ToHexString(iv).ToLower();
            container.SaltCryptSignKey = Convert.ToHexString(salt).ToLower();

            container.OpenSignKeyPointX = "3316a15ccbb75a466e733d45b394abb8677664c9313ab44d26f690af43b46a36744a3689ecc4aaba6004d400dd85127b243f6339de27feb8628e352fe892a3e8";
            container.OpenSignKeyPointY = "4717de15337ae417d19d856c2daae1070c0c0aac03334ee11ce5305f0fa092172ee4c24c00d959b11f8577f8382c0c1722e6915b8823770b2d51b0d25b023a7c";

            container.PublicKeyAlgoritmOid = PublicKeySignAlgoritmOid;
            container.EcOid = EcOid;
        }

        /// <summary>
        /// Шифрует секретный ключ проверки подписи  ГОСТ 34.11-2012 на хэши.
        /// </summary>
        public void CryptEcSecretKey(byte[] cipherKey, byte[] iv, byte[] ecKey)
        {
            ICipherAlgoritm algoritm = new ModifyAlgoritm3412();
            algoritm.DeployDecryptRoundKeys(cipherKey);
            ModeCFB cfb = new ModeCFB(algoritm);

            cfb.CfbEncrypt(ecKey, iv);
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

        /// <summary>
        /// Хэширует пароль с солью.
        /// </summary>
        /// <param name="salt"></param>
        /// <param name="password"></param>
        /// <param name="result"></param>
        public void HashedPassword(byte[] salt, byte[] password, byte[] result)
        {
            byte[] data = new byte[password.Length + salt.Length];

            Buffer.BlockCopy(password,0,data,0,password.Length);
            Buffer.BlockCopy(salt, 0, data, password.Length, salt.Length);

            var hasher = new Hash3411.Hash3411();
            hasher.Hash256(data, result);
        }

        /// <summary>
        /// Сохраняет контейнер ключа в файл
        /// </summary>
        /// <param name="container"></param>
        /// <param name="PathToSave"></param>
        /// <returns></returns>
        private bool KeyСontainerToFile(SecretKeyContainer container, string pathToSave)
        {
            XmlDocument xmlDocument = new XmlDocument();
            XmlSerializer serializer = new XmlSerializer(container.GetType());
            var emptyNamespaces = new XmlSerializerNamespaces(new[] { XmlQualifiedName.Empty });

            var strXml = string.Empty;

            using (MemoryStream stream = new MemoryStream())
            {
                serializer.Serialize(stream, container, emptyNamespaces);
                stream.Position = 0;
                xmlDocument.Load(stream);
                StringWriter sw = new StringWriter();
                XmlTextWriter xw = new XmlTextWriter(sw);
                xmlDocument.WriteTo(xw);

                strXml = sw.ToString();

                xmlDocument.Save("key.grk");
                stream.Close();
            }
            
            if(!PackContainer(strXml, out byte[] package))
            {
                LastError = "Ошибка W0: Ошибка упаковки.";
                return false;
            }

            //Вычисляет хэш256 контейнера.
            var hasher = new Hash3411.Hash3411();
            byte[] hashContainer = new byte[Hash3411.Hash3411.Hash256Size];

            hasher.Hash256(package, hashContainer);

            //Дополнение файла служебной информацией
            byte[] fileHead = new byte[KeyHeadLen];//Заголовок файла-служебная информация
            SetServiceInformation(fileHead, hashContainer, package.Length);

            return true;
        }
        
        private void SetServiceInformation(byte[] head, byte[] hash, int conteinerLen)
        {
            //Формирую заголовок файла [7-байт служебной информации]
             Buffer.BlockCopy(KeyHeadText, 0, head, 0,KeyHeadText.Length);

            //Название программы ProgramText [Заголовок 47байт]
            byte[] programText = Encoding.ASCII.GetBytes(ProgramText);
            Buffer.BlockCopy(programText, 0, head, KeyHeadText.Length, programText.Length);

            //Хэш 32 байта
            Buffer.BlockCopy(hash, 0, head, KeyHeadText.Length + programText.Length, hash.Length);

            //Размер блока данных 4байта
            byte[] len = BitConverter.GetBytes(conteinerLen);
            Buffer.BlockCopy(len, 0, head, KeyHeadText.Length + programText.Length + hash.Length, len.Length);
        }
        
        /// <summary>
        /// Запаковывает(шифрует) контейнер секретного ключа.
        /// </summary>
        /// <param name="container"></param>
        /// <returns></returns>
        private bool PackContainer(string container, out byte[] package)
        {
            try
            {
                //Генерация вектора инициализации для шифрования
                byte[] iv = CipherTools.GenerateRand(16);

                System.Text.Encoding.RegisterProvider(System.Text.CodePagesEncodingProvider.Instance);

                byte[] containerArray = Encoding.GetEncoding(1251).GetBytes(container);

                var cipherKey = Convert.FromHexString(KeyChipperConstant);

                //Выполняю шифрование в режиме обратной связи по шифротексту
                CryptEcSecretKey(cipherKey, iv, containerArray);

                //Размер заголовка. 16(iv) + 4(длина не шифрованного блока)
                package = new byte[KeyTitleSize + containerArray.Length];

                Buffer.BlockCopy(iv, 0, package, 0, iv.Length);

                byte[] len = BitConverter.GetBytes(containerArray.Length);
                Buffer.BlockCopy(len, 0, package, iv.Length, KeyTitleSize - iv.Length);

                Buffer.BlockCopy(containerArray, 0, package, KeyTitleSize, containerArray.Length);
            }
            catch (Exception e)
            {
                package = new byte[0];
                _lastException = e.Message;
                return false;
            }

            return true;
        }
    }
}