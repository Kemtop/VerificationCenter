using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LizardEncryptLib.CipherWorker
{
    /// <summary>
    /// Механизм кодирования/декодирования файлов на физическом диске.
    /// </summary>
    public class LizardCipherImpl : IFileCipherWorker
    {
        private string _lastError;

        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError
        {
            get => _lastError;
        }
        
        /// <summary>
        /// Имя локального каталога хранения шифрованных файлов.
        /// </summary>
        public const string LocalStoreEncryptName = "Encrypted_files";

        /// <summary>
        /// Имя локального каталога хранения шифрованных файлов.
        /// </summary>
        public const string LocalStoreDecryptName = "Decrypted_files";

        /// <summary>
        /// Шифрует файлы.
        /// </summary>
        /// <param name="pathsToFiles"></param>
        /// <param name="keyPath"></param>
        /// <param name="keyContainerPassword"></param>
        /// <returns></returns>
        public bool CryptFiles(List<string> pathsToFiles, string keyPath, string keyContainerPassword)
        {
            var (result, dstDir) = CreateDistDir(pathsToFiles.First());
            if (!result) return false;


            return true;
        }

        /// <summary>
        /// Выполняет операцию расшифровки выбранных файлов.
        /// </summary>
        /// <param name="pathsToFiles"></param>
        /// <param name="keyPath"></param>
        /// <param name="keyContainerPassword"></param>
        /// <returns></returns>
        public bool DecryptFiles(List<string> pathsToFiles, string keyPath, string keyContainerPassword)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Создает каталог куда будут помещены файлы.
        /// </summary>
        /// <param name="pathToCryptFile"></param>
        /// <param name="dstDirName"></param>
        /// <param name="localStoreName"></param>
        /// <returns></returns>
        public (bool, string dstDirName) CreateDistDir(string pathToCryptFile, string localStoreName = LocalStoreEncryptName)
        {
            try
            {
                string dstDirName = $"{Path.GetDirectoryName(pathToCryptFile)}\\{localStoreName}";

                if (!File.Exists(dstDirName))
                {
                    Directory.CreateDirectory(dstDirName);
                }

                return (true, dstDirName);
            }
            catch (Exception e)
            {
                _lastError = $"Не удалось создать каталог хранения исходящих файлов.{e.Message}";
                return (false, string.Empty);
            }
        }
    }
}
