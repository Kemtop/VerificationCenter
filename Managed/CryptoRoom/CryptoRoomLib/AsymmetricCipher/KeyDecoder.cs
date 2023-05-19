using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Cryptography;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading.Tasks;
using static System.Collections.Specialized.BitVector32;

namespace CryptoRoomLib.AsymmetricCipher
{
    /// <summary>
    /// Декодирует сеансовый ключ шифрования.
    /// </summary>
    internal class KeyDecoder
    {
        //Добавить проверку соответсвия ключевой пары.

        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string Error { get; private set; }

        /// <summary>
        /// Декодирует сеансовый ключ шифрования.
        /// </summary>
        public bool DecryptSessionKey(Span<byte> privateKeyPtr, byte[] cryptData, out byte[] encryptKey)
        {
            int bytesRead = 0;
            using (RSACryptoServiceProvider provider = new RSACryptoServiceProvider())
            {
                provider.ImportPkcs8PrivateKey(privateKeyPtr, out bytesRead);

                try
                {
                    encryptKey = provider.Decrypt(TestConst.CryptData, true);
                }
                catch (Exception e)
                {
                    Error = $"Ошибка AS3: Не удалось расшифровать сеансовый ключ. Возможно файл был предназначен не вам. Возникло исключение:{e.Message}";
                    encryptKey = new byte[] { };
                    return false;
                }
            }

            return true;
        }
    }
}
