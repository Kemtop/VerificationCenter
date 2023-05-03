using CryptoRoomLib.Cipher3412;

namespace CryptoRoomLib.Tests
{
    public class Tests
    {
        [SetUp]
        public void Setup()
        {
        }

        [Test]
        public void CryptoLibTest()
        {
            //Запускает внутренние тесты алгоритма.
            var self = new SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res.result, res.error);
        }
    }
}