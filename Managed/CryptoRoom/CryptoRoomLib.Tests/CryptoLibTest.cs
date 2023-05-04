using CryptoRoomLib.Cipher3412;
using CryptoRoomLib.CipherMode3413;

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
            var self = new Cipher3412.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        [Test]
        public void CipherMode3413()
        {
            var self = new CipherMode3413.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }
    }
}