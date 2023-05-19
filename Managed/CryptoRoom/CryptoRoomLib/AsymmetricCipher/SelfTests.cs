using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.AsymmetricCipher
{
    /// <summary>
    /// Набор встроенных тестов для проверки правильности алгоритма.
    /// </summary>
    public class SelfTests
    {
        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string Error { get; private set; }

        public SelfTests()
        {
            Error = string.Empty;
        }

        /// <summary>
        /// Общий метод тестирования всего алгоритма 34.12.
        /// </summary>
        /// <returns></returns>
        public bool RunTests()
        {
            List<Func<bool>> tests = new List<Func<bool>>
            {
                TestDecryptSessionKey
            };

            foreach (var test in tests)
            {
                if (!test()) return false;
            }

            return true;
        }

        /// <summary>
        /// Тест декодирования сессионного ключа.
        /// </summary>
        /// <returns></returns>
        private bool TestDecryptSessionKey()
        {
            KeyDecoder kd = new KeyDecoder();
            byte[] sessionKey;

            kd.DecryptSessionKey(AsymmetricCipher.TestConst.RsaPrivateKey, TestConst.CryptData, out sessionKey);

            if (!sessionKey.SequenceEqual(TestConst.SessionKey))
            {
                Error = "Не удалось расшифровать сеансовый ключ.";
                return false;
            }

            return true;
        }

    }
}
