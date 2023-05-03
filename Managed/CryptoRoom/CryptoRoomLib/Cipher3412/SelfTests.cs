namespace CryptoRoomLib.Cipher3412
{
    /// <summary>
    /// Набор встроенных тестов для проверки правильности алгоритма.
    /// </summary>
    public class SelfTests
    {
        private string _error;

        public SelfTests()
        {
            _error = string.Empty;
        }

        /// <summary>
        /// Общий метод тестирования всего алгоритма 34.12.
        /// </summary>
        /// <returns></returns>
        public (bool result, string error) RunTests()
        {
            List<Func<bool>> tests = new List<Func<bool>>
            {
                DeployKeyRoundTest,
                GostExampleTest,
                GostWithRoundKeysTest
            };

            foreach (var test in tests)
            {
                if(!test()) return (false, _error);
            }
            
            return (true, string.Empty);
        }

        /// <summary>
        /// Тест развертывания раундовых ключей.
        /// </summary>
        /// <returns></returns>
        private bool DeployKeyRoundTest()
        {
            var roundKeys = new ulong[20];
            var key = new byte[32];
            var etalonResult = new ulong[20];

            //Перебираю все ключи шифрования из тестового набора.
            for (int i = 0; i < TestConst3412.Key.GetLength(0); i++)
            {
                Array.Clear(roundKeys, 0, roundKeys.Length);
                Array.Clear(key, 0, key.Length);

                System.Buffer.BlockCopy(TestConst3412.Key, 32 * i, key, 0, 32);
                Logic3412.DeploymentEncryptionRoundKeys(key, roundKeys);

                //Получаем результат развертывания для данного ключа.
                System.Buffer.BlockCopy(TestConst3412.KeyDeploymentResult, i*20*8, etalonResult, 0, 20*8);

                if (!roundKeys.SequenceEqual(etalonResult))
                {
                    _error = $"Bad deploy encrypt test result: key={BitConverter.ToString(key)}";
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Тест на основании значения ключа, текста, и результата приведенного в ГОСТ.
        /// </summary>
        /// <returns></returns>
        private bool GostExampleTest()
        {
            var key = new byte[32];
            var roundKeys = new ulong[20];

            //Используя ключ из примера Гост, разворачиваем раундовые ключи шифрования.
            System.Buffer.BlockCopy(TestConst3412.GostKey, 0, key, 0, 32);
            Logic3412.DeploymentEncryptionRoundKeys(key, roundKeys);

            return GostExampleTest(roundKeys, key);
        }

        /// <summary>
        /// Тест на основании значения ключа, значений итерационных ключей, текста, и результата приведенного в ГОСТ.
        /// </summary>
        /// <returns></returns>
        private bool GostWithRoundKeysTest()
        {
            var key = new byte[32];
            System.Buffer.BlockCopy(TestConst3412.GostKey, 0, key, 0, 32);

            return GostExampleTest(TestConst3412.GostRoundKeys, key);
        }

        /// Тест на основании значения ключа, текста, и результата приведенного в ГОСТ.
        /// </summary>
        /// <returns></returns>
        private bool GostExampleTest(ulong[] roundKeys, byte[] key)
        {
            //Формируем шифротекст.
            U128t data = new U128t();
            var temp = new byte[8];

            System.Buffer.BlockCopy(TestConst3412.GostTextToCipher, 0, temp, 0, 8);
            data.Low = BitConverter.ToUInt64(temp, 0);

            System.Buffer.BlockCopy(TestConst3412.GostTextToCipher, 8, temp, 0, 8);
            data.Hi = BitConverter.ToUInt64(temp, 0);

            //Шифруем блок.
            Logic3412.EncryptBlock(ref data, roundKeys);
           
            if (TestConst3412.GostCipherResult[0] == data.Low && TestConst3412.GostCipherResult[1] == data.Hi)
                return true;

            _error = "Error when check Gost test.";

            return false;
        }
    }
}
