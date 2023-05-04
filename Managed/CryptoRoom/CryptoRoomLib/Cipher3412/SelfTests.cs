using CryptoRoomLib.Cipher3412.FastConst;

namespace CryptoRoomLib.Cipher3412
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
                Test128Type,
                DeployKeyRoundTest,
                GostExampleTest,
                GostWithRoundKeysTest,
                DecryptionTest
            };

            foreach (var test in tests)
            {
                if(!test()) return false;
            }
            
            return true;
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

                Buffer.BlockCopy(TestConst3412.Key, 32 * i, key, 0, 32);
                Logic3412.DeploymentEncryptionRoundKeys(key, roundKeys);

                //Получаем результат развертывания для данного ключа.
                Buffer.BlockCopy(TestConst3412.KeyDeploymentResult, i*20*8, etalonResult, 0, 20*8);

                if (!roundKeys.SequenceEqual(etalonResult))
                {
                    Error = $"Bad deploy encrypt test result: key={BitConverter.ToString(key)}";
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

            Error = "Error when check Gost test.";

            return false;
        }

        /// <summary>
        /// Тестирует работу типа данных.
        /// </summary>
        /// <returns></returns>
        private bool Test128Type()
        {
            if (TestConst3412.ShiftTestTable.Length != sizeof(ulong))
            {
                Error = $"Error tests shift operation. Bad test array size.";
                return false;
            }

            U128t t = new U128t();

            //Берем константу для теста, которая будет помещена в разные позиции бинарного представления числа.
            for (int i = 0; i < TestConst3412.ShiftTestTable.Length; i++)
            {
                t.Low = TestConst3412.ShiftTestDigit;
                t.SetByte(i, TestConst3412.ShiftTestLowDigit);

                if (t.Low != TestConst3412.ShiftTestTable[i])
                {
                    Error = $"Error tests shift operation.Iteration={i}";
                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Тестирование алгоритма декодирования.
        /// </summary>
        /// <returns></returns>
        private bool DecryptionTest()
        {
            var roundKeys = new ulong[20];
            var key = new byte[32];

            for (int i = 0; i < TestConst3412.DecryptTestKey.GetLength(0); i++)
            {
                Array.Clear(roundKeys, 0, roundKeys.Length);
                Array.Clear(key, 0, key.Length);

                Buffer.BlockCopy(TestConst3412.DecryptTestKey, 32 * i, key, 0, 32);

                Logic3412.DeploymentDecryptionRoundKeys(key, roundKeys);

                U128t data;
                data.Low = TestConst3412.DecryptTestInText[i, 0];
                data.Hi = TestConst3412.DecryptTestInText[i, 1];
                
                Logic3412.DecryptBlock(ref data, roundKeys);

                if (data.Low != TestConst3412.DecryptTestOutText[i, 0] ||
                    data.Hi != TestConst3412.DecryptTestOutText[i, 1])
                {
                    Error = $"Decrypt test error. Pos={i}";
                    return false;
                }
            }

            return true;
        }
    }
}
