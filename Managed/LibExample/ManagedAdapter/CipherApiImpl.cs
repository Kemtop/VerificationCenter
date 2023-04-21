using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ManagedAdapter
{
    /// <summary>
    /// АPI с необходимым набором функций.
    /// </summary>
    public class CipherApiImpl : ICipherApi
    {
        private string _lastError;
        private readonly string _pathToKey;
        private readonly string _password;
        private readonly IManagedAdapter _adapter;
        public CipherApiImpl(IManagedAdapter adapter ,string pathToKey, string password)
        {
            _adapter = adapter;
            _pathToKey = pathToKey;
            _password = password;
        }

        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError
        {
            get
            {
                if (String.IsNullOrEmpty(_lastError))
                {
                    _lastError = _adapter.LastError;
                }

                return _lastError;
            }
        }

        /// <summary>
        /// Имя локального каталога хранения шифрованных файлов.
        /// </summary>
        private const string LocalStoreEncryptName = "Encrypted_files";

        /// <summary>
        /// Имя локального каталога хранения шифрованных файлов.
        /// </summary>
        private const string LocalStoreDecryptName = "Decrypted_files";

        public string GetLocalStoreEncryptName() => LocalStoreEncryptName;
        public string GetLocalStoreDecryptName() => LocalStoreEncryptName;

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

        /// <summary>
        /// Временный метод для правильного выполнения всех действий по инициализации Qt библиотеки.
        /// В будущем все должно быть в dll.
        /// </summary>
        /// <returns></returns>
        private bool InitCipher()
        {
            //Создать делегат Func нельзя и список! Все функция разные!
            if (!_adapter.LoadSecretKey(_pathToKey)) return false;
            if (!_adapter.CheckPasswordForContainer(_password)) return false;
            if (!_adapter.LoadAsymmetricKey(_password)) return false;
            if (!_adapter.InitCipherWorker()) return false;
            if (!_adapter.LoadSignedSecretKey(_password)) return false;

            return true;
        }

        /// <summary>
        /// Очищает сообщение об ошибке.
        /// </summary>
        private void ClearLastError()
        {
            _lastError = string.Empty;
        }

        /// <summary>
        ///  Шифрует файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <returns></returns>
        public bool CryptlFile(string srcPath)
        {
            return CryptlFile(srcPath,  LocalStoreEncryptName);
        }
        
        /// <summary>
        ///  Шифрует файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        public bool CryptlFile(string srcPath, string dstDir = LocalStoreEncryptName)
        {
            ClearLastError();

            _adapter.SetSingleModeKeyType();
            if (!InitCipher()) return false;

            //Создает предустановленную директорию куда будет помещен зашифрованный файл.
            (bool result, string pathToCryptDir) = CreateDistDir(srcPath);
            if (!result) return false;

            if (!_adapter.CryptlFile(srcPath, pathToCryptDir)) return false;

            _adapter.ClearCipherWorker();

            return true;
        }
    }
}
