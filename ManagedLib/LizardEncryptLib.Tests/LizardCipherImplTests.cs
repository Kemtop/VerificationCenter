using LizardEncryptLib.CipherWorker;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LizardEncryptLib.Tests
{
    [TestFixture]
    internal class LizardCipherImplTests
    {
        private const string password = "12345678";
        private const string key = "";

        private static readonly object[] _sourceFiles =
        {
            new object[] {new List<string>
            {
                ".\\file1.txt",
                ".\\file2.txt",
            }}
        };
        
        [Test, TestCaseSource(nameof(_sourceFiles))]
        public void CryptFiles(List<string> pathsToFiles)
        {
            LizardCipherImpl li = new LizardCipherImpl();
            bool result = li.CryptFiles(pathsToFiles, key, password);
            Assert.IsTrue(result, li.LastError);
        }
    }
}
