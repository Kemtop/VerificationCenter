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
                GostDecryptCbc,
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
        /// Тест режима работы блочного шифра на основание таблицы из ГОСТ.
        /// А.2.4 Режим простой замены с зацеплением
        /// </summary>
        /// <returns></returns>
        bool GostDecryptCbc()
        {
            Register256t register = new Register256t();// Регистр размером m = kn =  2*16
            Block128t lsb = new Block128t();//значением n разрядов регистра сдвига с большими номерами

            Block128t сBlock = new Block128t(); //Входящий шифротекст.
            Block128t pBlock = new Block128t(); //Исходящий декодированный текст.
            Block128t tmpBlock = new Block128t(); //Хранит временные данные

            //Данные из примера.
            var gostKey = TestConst3413.ExampleA2_4_key;
            var gostIv = TestConst3413.ExampleA2_4_iv;
            var cryptedText = TestConst3413.ExampleA2_4_cryptedText;
            var text = TestConst3413.ExampleA2_4_text;
            
            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            ModeCBC cbc = new ModeCBC(algoritm);

            algoritm.DeployDecryptRoundKeys(gostKey);

            //Заполнение регистра данными IV
            register.FromArray(gostIv);

            int testWordSize = cryptedText.GetLength(1);
            byte[] buffer = new byte[testWordSize]; 

            for (int i = 0; i < cryptedText.GetLength(0); i++)
            {
                Buffer.BlockCopy(cryptedText, i * testWordSize,
                    buffer, 0, testWordSize);

                сBlock.FromArray(buffer);
                cbc.IterationCBC(ref register, ref tmpBlock, ref lsb, ref сBlock, ref pBlock); //Расшифровываю

                Block128t etalon = new Block128t(); //Эталон из примеров.

                Buffer.BlockCopy(text, i * testWordSize,
                    buffer, 0, testWordSize);
                
                etalon.FromArray(buffer);

                if (!etalon.Compare(pBlock))
                {
                    Error = $"Error test CipherMode3413 GostDecryptCbc. Pos={i}";
                    return false;
                }
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

            ulong blockCount = 0;
            ulong blockNum = 0;
            ulong decryptDataSize = 0;

            algoritm.DeployDecryptRoundKeys(TestConst3413.Key);
            cbc.DecryptData("Test.crypt", "Test.jpg",
                (size) => { decryptDataSize = size;}, 
                (max) => { blockCount = max; },
                (number) => { blockNum = number; }
            );

            return true;
        }
    }
}
