using NUnit.Framework;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ManagedAdapter;

namespace Lib.Tests
{
    /// <summary>
    /// Тесты доступа к низкоуровневой библиотеке.
    /// </summary>
    [TestFixture]
    internal class ManagedAdapterTest
    {
        private IManagedAdapter _adapter;

        [SetUp]
        public void Setup()
        {
            _adapter = new CipherAdapterImpl();
            _adapter.SetSingleModeKeyType();
        }

        /// <summary>
        /// Создает каталог для сохранения обрабатываемых файлов.
        /// </summary>
        private void CreateDstDir(string dstDirName)
        {
            if (!File.Exists(dstDirName))
            {
                Directory.CreateDirectory(dstDirName);
            }
        }

        [Test, Order(1)]
        [TestCase("SecretKey.key")]
        public void LoadSecretKey(string pathToKey)
        {
            var result = _adapter.LoadSecretKey(pathToKey);
            Assert.IsTrue(result, _adapter.LastError);
        }

        [Test, Order(2)]
        [TestCase("12345678")]
        public void CheckPasswordForContainer(string password)
        {
            var result = _adapter.CheckPasswordForContainer(password);
            Assert.IsTrue(result, _adapter.LastError);
        }

        [Test, Order(3)]
        [TestCase("12345678")]
        public void LoadAsymmetricKey(string password)
        {
            var result = _adapter.LoadAsymmetricKey(password);
            Assert.IsTrue(result, _adapter.LastError);
        }

        [Test, Order(4)]
        public void InitCipherWorker()
        {
            var result = _adapter.InitCipherWorker();
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.ClearCipherWorker();
            Assert.IsTrue(result, _adapter.LastError);
        }

        [Test, Order(5)]
        [TestCase("12345678")]
        public void LoadSignedSecretKey(string password)
        {
            var result = _adapter.LoadSignedSecretKey(password);
            Assert.IsTrue(result, _adapter.LastError);
        }

        [Test, Order(6)]
        public void Rand256MemoryAccess()
        {
            //Тест правильности выделения размера массива-должен вернуться пустой массив.
            var result = _adapter.Rand256(1);
            Assert.IsTrue(result.result, _adapter.LastError);

            bool isZeros = result.data.All(x => x == 0);
            Assert.IsTrue(isZeros, "Функция вернула значения отличные от 0.");
        }

        [Test, Order(7)]
        public void Rand256()
        {
            var result = _adapter.Rand256(0);
            Assert.IsTrue(result.result, _adapter.LastError);
            TestContext.WriteLine($"rnd32={string.Join(" ", result.data.Select(x => x.ToString("X2")))}");
        }

        [Test, Order(8)]
        [TestCase("SecretKey.key", "12345678", ".\\LICENSE.txt", ".\\Encrypted_files")]
        public void CryptlFile(string pathToKey, string password, string srcPath, string dstDir)
        {
            CreateDstDir(dstDir);
            //Полотно выглядит криво-но пока dll не переделана, используем TDD.
            var result = _adapter.LoadSecretKey(pathToKey);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.CheckPasswordForContainer(password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.LoadAsymmetricKey(password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.InitCipherWorker();
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.LoadSignedSecretKey(password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.CryptlFile(srcPath, dstDir);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.ClearCipherWorker();
            Assert.IsTrue(result, _adapter.LastError);
        }

        [Test, Order(9)]
        [TestCase("SecretKey.key", "12345678", ".\\Encrypted_files\\LICENSE.txt.crypt", ".\\Decrypted_files")]
        public void DecryptlFile(string pathToKey, string password, string srcPath, string dstDir)
        {
            CreateDstDir(dstDir);
            //Полотно выглядит криво-но пока dll не переделана, используем TDD.
            var result = _adapter.LoadSecretKey(pathToKey);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.CheckPasswordForContainer(password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.LoadAsymmetricKey(password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.InitCipherWorker();
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.LoadSignedSecretKey(password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.DecryptlFile(srcPath, dstDir, password);
            Assert.IsTrue(result, _adapter.LastError);

            result = _adapter.ClearCipherWorker();
            Assert.IsTrue(result, _adapter.LastError);
        }
    }
}
