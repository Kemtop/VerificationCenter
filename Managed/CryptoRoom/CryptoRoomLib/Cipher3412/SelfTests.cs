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

        public (bool result, string error) RunTests()
        {
            if (!DeployKeyRoundTest()) return (false, _error);

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
    }
}
