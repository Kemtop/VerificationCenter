using System.Security.Cryptography;

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
        /// Сравнивает открытый и закрытый ключ. Соответствует ли закрытый ключ открытому.
        /// </summary>
        /// <param name="privateKeyPtr"></param>
        /// <param name="publicKeyPtr"></param>
        /// <returns></returns>
        public bool CheckKeyPair(Span<byte> privateKeyPtr, Span<byte> publicKeyPtr)
        {
            int bytesRead = 0;
            using (RSACryptoServiceProvider privateKey = new RSACryptoServiceProvider())
            using (RSA rsa = RSA.Create())
            {
                privateKey.ImportPkcs8PrivateKey(privateKeyPtr, out bytesRead);
                var privateParam = privateKey.ExportParameters(true);

                rsa.ImportSubjectPublicKeyInfo(publicKeyPtr, out bytesRead);
                var publicKeyParam = rsa.ExportParameters(false);

                if (privateParam.Modulus == null)
                {
                    Error = "Отсутствует значение модуля для закрытого ключа.";
                    return false;
                }

                if (publicKeyParam.Modulus == null)
                {
                    Error = "Отсутствует значение модуля для открытого ключа.";
                    return false;
                }

                if (!privateParam.Modulus.SequenceEqual(publicKeyParam.Modulus))
                {
                    Error = "Закрытый ключ не соответствует открытому.";
                    return false;
                }
            }

            return true;
        }
        
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
