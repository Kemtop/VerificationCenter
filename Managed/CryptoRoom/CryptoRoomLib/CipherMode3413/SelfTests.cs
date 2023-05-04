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
                DeсryptIterationCBC
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

            byte[] key = new byte[32]
            {
                0xee, 0xa0, 0x09, 0xcc, 0x52, 0x38, 0x01, 0x35,
                0x37, 0x59, 0x74, 0xb3, 0x0f, 0x6a, 0x81, 0xc4,
                0xe2, 0x8b, 0x9f, 0x57, 0x8e, 0xba, 0x0a, 0xcc,
                0x43, 0x78, 0x64, 0x57, 0x00, 0xc3, 0x12, 0xe3
            };

            algoritm.DeployDecryptRoundKeys(key);
            
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
    }
}
