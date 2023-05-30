using CryptoRoomLib.Cipher3412;
using CryptoRoomLib.CipherMode3413;
using CryptoRoomLib.KeyGenerator;

namespace CryptoRoomLib.Tests
{
    public class Tests
    {
        [SetUp]
        public void Setup()
        {
        }

        [Test, Order(1)]
        public void CryptoLibTest()
        {
            //Запускает внутренние тесты алгоритма.
            var self = new Cipher3412.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        [Test, Order(2)]
        public void CipherMode3413()
        {
            var self = new CipherMode3413.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        [Test, Order(3)]
        public void AsymmetricCipherTest()
        {
            var self = new CryptoRoomLib.AsymmetricCipher.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        //[Test, Order(4)]
        //public void SecretKeyMakerTest()
        //{
        //    SecretKeyMaker maker = new SecretKeyMaker();
        //    var res = maker.CreateKeyFileNoReq("ТестовыйПароль99EngLater", "key.grk");
        //    //var res = maker.CreateKeyFileNoReq("12345678");

        //    Assert.IsTrue(res);
        //}

        /// <summary>
        /// Загружает секретный ключ и проверяет его.
        /// </summary>
        [Test, Order(5)]
        public void KeyServiceLoadKeyTest()
        {
            KeyService service = new KeyService();
            var res = service.LoadKey("key.grk");
            
            Assert.IsTrue(res, service.LastError);
        }

        /// <summary>
        /// Читает содержимое одного ключа и копирует в другой.
        /// </summary>
        [Test, Order(6)]
        public void CopySecretKeyTest()
        {
            KeyService service = new KeyService();
            var res = service.LoadKey("key.grk");
            Assert.IsTrue(res, service.LastError);

            SecretKeyMaker maker = new SecretKeyMaker();
            res = maker.SaveToFile(service.KeyContainer, "copyKey.grk");
            Assert.IsTrue(res, maker.LastError);
        }

        /// <summary>
        /// Проверяет пароль для секретного ключа.
        /// </summary>
        [Test, Order(7)]
        public void CheckPasswordTest()
        {
            KeyService service = new KeyService();
            var res = service.LoadKey("key.grk");
            Assert.IsTrue(res, service.LastError);

            res = service.CheckPassword("12345678");
            Assert.IsTrue(res, service.LastError);

            res = service.CheckPassword("123456789");
            Assert.IsFalse(res, service.LastError);
        }

        /// <summary>
        /// Тест хеш функции ГОСТ 34.11-2012.
        /// </summary>
        [Test, Order(8)]
        public void Hash3411Test()
        {
            var self = new Hash3411.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        /// <summary>
        /// Тест алгоритмов генерации секретного ключа.
        /// </summary>
        [Test, Order(9)]
        public void KeyGeneratorTest()
        {
            var self = new KeyGenerator.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        /// <summary>
        /// Расшифровывает файл.
        /// </summary>
        [Test, Order(10)]
        public void DecryptingFile()
        {
            KeyService keyService = new KeyService();
            var res = keyService.LoadKey("key.grk");
            Assert.IsTrue(res, keyService.LastError);

            res = keyService.CheckPassword("12345678");
            Assert.IsTrue(res, keyService.LastError);

            ICipherAlgoritm algoritm = new CipherAlgoritm3412();
            IBlockCipherMode cipherMode = new ModeCBC(algoritm);
            
            CipherWorker worker = new CipherWorker(keyService, cipherMode);

            ulong blockCount = 0;
            ulong blockNum = 0;
            ulong decryptDataSize = 0;

            res = worker.DecryptingFile("Test.crypt", "Test1.jpg",
                (size) => { decryptDataSize = size; },
                (max) => { blockCount = max; },
                (number) => { blockNum = number; });

            Assert.IsTrue(res);
        }

    }
}