using LizardEncryptLib.LowLayerFunction;

namespace LizardEncryptLib.Tests
{
    /// <summary>
    /// Тесты доступа к низкоуровневой библиотеке.
    /// </summary>
    [TestFixture]
    public class NativeAdapterTests
    {
        private ILowLayerFunctions _adapter;

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
    }
}