using CryptoRoomLib.Cipher3412;

namespace CryptoRoomLib.CipherMode3413
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

        /// <summary>
        /// Общий метод тестирования всего алгоритма.
        /// </summary>
        /// <returns></returns>
        public bool RunTests()
        {
            List<Func<bool>> tests = new List<Func<bool>>
            {
                XorBlocksTest,
                DeсryptIterationCBC,
                DecryptData
            };

            foreach (var test in tests)
            {
                if (!test()) return false;
            }

            return true;
        }

        /// <summary>
        /// Тестирование метода сложения блоков длиной 16 байт по модулю 2.
        /// </summary>
        /// <returns></returns>
        private bool XorBlocksTest()
        {
            ModeCBC cbc = new ModeCBC(new CipherAlgoritm3412());

            Block128t cBlock;
            cBlock.Low = 0x14353cca5619e7bd;
            cBlock.Hi = 0xe6b24748662b9dc1;
            
            Block128t msb;
            msb.Low = 0x14353cca5642174c;
            msb.Hi = 0xe6b24748662b9dc1;

            Block128t part;
            part.Low = 0x00000000005bf0f1;
            part.Hi = 0x0000000000000000;

            Block128t result = new Block128t();

            cbc.XorBlocks(ref cBlock, ref msb, ref result);

            if (part.Low != result.Low || part.Hi != result.Hi)
            {
                Error = "Error test CipherMode3413 XorBlocks.";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Тестирование итерации в режиме простой замены.
        /// </summary>
        /// <returns></returns>
        private bool DeсryptIterationCBC()
        {
            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            ModeCBC cbc = new ModeCBC(algoritm);

            Block128t cBlock;
            cBlock.Low = 0xda3ecc31a05c9124;
            cBlock.Hi = 0x04139dc14ab5b347;
            
            algoritm.DeployDecryptRoundKeys(TestConst3413.Key);
            
            Block128t msb;
            msb.Low = 0x14353cca5642174c;
            msb.Hi = 0xe6b24748662b9dc1;

            Block128t result = new Block128t();

            cbc.DeсryptIterationCBC(ref result, msb, ref cBlock);

            //Правильный результат шифрования.
            Block128t etalon;
            etalon.Low = 0x00000000005bf0f1;
            etalon.Hi = 0x0000000000000000;

            if (etalon.Low != result.Low || etalon.Hi != result.Hi)
            {
                Error = "Error test CipherMode3413 DeсryptIterationCBC.";
                return false;
            }

            return true;
        }

        /// <summary>
        /// Тест расшифровки данных.
        /// </summary>
        /// <returns></returns>
        bool DecryptData()
        {
            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            ModeCBC cbc = new ModeCBC(algoritm);
            
            ulong[] iv =
            {
                0x14353cca5642174c,
                0xe6b24748662b9dc1,
                0xe297262e0534dfa4,
                0x54396d4ef127d6ce
            };

            algoritm.DeployDecryptRoundKeys(TestConst3413.Key);
            cbc.DecryptData(iv, "Test.crypt", "Test.jpg", 142);

            return true;
        }

    }
}
