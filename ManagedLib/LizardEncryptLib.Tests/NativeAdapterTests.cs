using LizardEncryptLib.LowLayerFunction;

namespace LizardEncryptLib.Tests
{
    /// <summary>
    /// Тесты доступа к низкоуровневой библиотеке.
    /// </summary>
    [TestFixture]
    public class NativeAdapterTests
    {
        private NativeAdapter _adapter;

        [SetUp]
        public void Setup()
        {
            _adapter = new NativeAdapter();
            _adapter.SetSingleModeKeyType();
        }

        [Test, Order(1)]
        [TestCase("SecretKey.key")]
        public void LoadSecretKey(string pathToKey)
        {
            var result = _adapter.LoadSecretKey(pathToKey);
            Assert.IsTrue(result, _adapter.GetLastError());
        }
        
        [Test, Order(2)]
        [TestCase("12345678")]
        public void CheckPasswordForContainer(string password)
        {
            var result = _adapter.CheckPasswordForContainer(password);
            Assert.IsTrue(result, _adapter.GetLastError());
        }

        [Test, Order(3)]
        [TestCase("12345678")]
        public void LoadAsymmetricKey(string password)
        {
            var result = _adapter.LoadAsymmetricKey(password);
            Assert.IsTrue(result, _adapter.GetLastError());
        }

        [Test, Order(4)]
        public void InitCipherWorker()
        {
            var result = _adapter.InitCipherWorker();
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.ClearCipherWorker();
            Assert.IsTrue(result, _adapter.GetLastError());
        }

        [Test, Order(5)]
        [TestCase("12345678")]
        public void LoadSignedSecretKey(string password)
        {
            var result = _adapter.LoadSignedSecretKey(password);
            Assert.IsTrue(result, _adapter.GetLastError());
        }

        [Test, Order(6)]
        [TestCase("SecretKey.key", "12345678", ".\\license.rtf", ".\\Encrypted_files")]
        public void CryptlFile(string pathToKey, string password, string srcPath, string dstDir)
        {
            var result = _adapter.LoadSecretKey(pathToKey);
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.CheckPasswordForContainer(password);
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.LoadAsymmetricKey(password);
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.InitCipherWorker();
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.LoadSignedSecretKey(password);
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.CryptlFile(srcPath, dstDir);
            Assert.IsTrue(result, _adapter.GetLastError());

            result = _adapter.ClearCipherWorker();
            Assert.IsTrue(result, _adapter.GetLastError());
        }
    }
}