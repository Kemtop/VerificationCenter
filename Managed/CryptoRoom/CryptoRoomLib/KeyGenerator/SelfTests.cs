﻿using CryptoRoomLib.Cipher3412;
using CryptoRoomLib.CipherMode3413;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.KeyGenerator
{
    /// <summary>
    /// Набор встроенных тестов для проверки правильности алгоритма.
    /// </summary>
    public class SelfTests : SelfTestsBase
    {
        public SelfTests()
        {
            AppendFunc(CryptEcSecretKeyTest);   
        }

        /// <summary>
        /// Тест. Шифрует секретный ключ проверки подписи  ГОСТ 34.11-2012 на хэши.
        /// </summary>
        /// <returns></returns>
        private bool CryptEcSecretKeyTest()
        {
            byte[] cipherKey = new byte[]
            {
                0x6f, 0x1e, 0xa3, 0x4d, 0xe5, 0xf0, 0x6b, 0x84,
                0x59, 0x6e, 0x5e, 0x20, 0xcc, 0x04, 0x32, 0x34,
                0x9e, 0x38, 0xce, 0x5d, 0x08, 0x56, 0xae, 0x35,
                0xd5, 0xec, 0x37, 0x0b, 0x02, 0x4e, 0x16, 0xd6,
            };

            byte[] iv = new byte[]
            {
                0xdd, 0xef, 0x3f, 0x72, 0xa8, 0xf2, 0x4a, 0x0d,
                0xe9, 0xe2, 0x02, 0x35, 0x04, 0x39, 0x0d, 0x17, //
                0x20, 0xca, 0xdd, 0x81, 0xd0, 0xca, 0xf6, 0x00,
                0xb8, 0x47, 0x8f, 0x00, 0x05, 0x00, 0x00, 0x00,
            };

            byte[] ecKey = new byte[] 
            {
                0x58, 0x76, 0xdd, 0xf5, 0x96, 0xfa, 0xdd, 0xe3,
                0x32, 0x0f, 0xa9, 0xa4, 0xf9, 0x7a, 0xd5, 0x86,
                0x13, 0x0e, 0xc1, 0xa1, 0x69, 0x54, 0x07, 0x3c,
                0x93, 0xfe, 0x12, 0xa6, 0xbb, 0xc5, 0x4e, 0x59,
                0x24, 0x0b, 0x23, 0x09, 0x07, 0x10, 0x02, 0x20,
                0x2f, 0xd0, 0x05, 0x04, 0x1f, 0xf6, 0x5b, 0x25,
                0x89, 0xbb, 0x80, 0x2a, 0xfc, 0xf2, 0xf3, 0x4e,
                0x05, 0xe5, 0x6b, 0x94, 0x2b, 0xa8, 0xdb, 0xde,
            };

            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            algoritm.DeployDecryptRoundKeys(cipherKey);
            ModeCFB cfb = new ModeCFB(algoritm);

            SecretKeyMaker sm = new SecretKeyMaker();
            sm.CryptEcSecretKey(cipherKey, iv, ecKey);

            return true;
        }
    }
}
