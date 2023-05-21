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

        [Test]
        public void CryptoLibTest()
        {
            //��������� ���������� ����� ���������.
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

        [Test]
        public void AsymmetricCipherTest()
        {
            var self = new CryptoRoomLib.AsymmetricCipher.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        [Test]
        public void SecretKeyMakerTest()
        {
            SecretKeyMaker maker = new SecretKeyMaker();
            var res = maker.CreateKeyFileNoReq("��������������99EngLater", "key.grk");
            //var res = maker.CreateKeyFileNoReq("12345678");
            
            Assert.IsTrue(res);
        }

        /// <summary>
        /// ���� ��� ������� ���� 34.11-2012.
        /// </summary>
        [Test]
        public void Hash3411Test()
        {
            var self = new Hash3411.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }

        /// <summary>
        /// ���� ���������� ��������� ���������� �����.
        /// </summary>
        [Test]
        public void KeyGeneratorTest()
        {
            var self = new KeyGenerator.SelfTests();
            var res = self.RunTests();

            Assert.IsTrue(res, self.Error);
        }
    }
}