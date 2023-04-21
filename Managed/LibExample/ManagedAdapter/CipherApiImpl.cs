using Microsoft.VisualBasic;
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
        /// Создает каталог куда будут помещены файлы.
        /// </summary>
        /// <param name="dstDirName"></param>
        /// <returns></returns>
        private bool CreateDistDir(string dstDirName)
        {
            try
            {
                if (!File.Exists(dstDirName))
                {
                    Directory.CreateDirectory(dstDirName);
                }

                return true;
            }
            catch (Exception e)
            {
                _lastError = $"Не удалось создать каталог хранения исходящих файлов.{e.Message}";
                return false;
            }
        }

        /// <summary>
        /// Временный метод для правильного выполнения всех действий по инициализации Qt библиотеки.
        /// В будущем все должно быть в dll.
        /// </summary>
        /// <returns></returns>
        private bool InitCipher()
        {
            _adapter.SetSingleModeKeyType();
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
        /// Шифрует файл.
        /// </summary>
        /// <param name="srcPathToFile"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        public bool CryptlFile(string srcPathToFile, string dstDir)
        {
            ClearLastError();
            
            if (!InitCipher()) return false;

            //Создает директорию куда будет помещен зашифрованный файл.
            if (!CreateDistDir(dstDir)) return false;

            if (!_adapter.CryptlFile(srcPathToFile, dstDir)) return false;

            _adapter.ClearCipherWorker();

            return true;
        }

        /// <summary>
        /// Декодирую файл.
        /// </summary>
        /// <param name="srcPathToFile"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        public bool DecCryptlFile(string srcPathToFile, string dstDir)
        {
            ClearLastError();
            
            if (!InitCipher()) return false;

            //Создает директорию куда будет помещен зашифрованный файл.
            if (!CreateDistDir(dstDir)) return false;

            if (!_adapter.DecryptlFile(srcPathToFile, dstDir, _password)) return false;

            _adapter.ClearCipherWorker();

            return true;
        }

        private bool Crypt(string srcPathToFile, string dstDir, Action<string> endIteration)
        {
            if (!_adapter.CryptlFile(srcPathToFile, dstDir)) return false;
            endIteration(srcPathToFile);

            return true;
        }

        /// <summary>
        /// Универсальный метод для обработки файлов.
        /// </summary>
        private bool ProcessFiles(string srcDir, string dstDir, Func<string, string, bool> processFunc,
            Action<string> endIteration)
        {
            var listFiles = Directory.GetFiles(srcDir);

            ClearLastError();
            if (!InitCipher()) return false;

            var result = Parallel.For(0, listFiles.Length, (int i, ParallelLoopState pls) =>
            {
                if (!processFunc(listFiles[i], dstDir))
                {
                    _lastError = $"Файл {listFiles[i]} {_adapter.LastError}";
                    pls.Break();
                }
                endIteration(listFiles[i]);
            });

            _adapter.ClearCipherWorker();

            if (!result.IsCompleted) return false;

            return true;
        }
        
        /// <summary>
        /// Шифрует все файлы из каталога. После кодирования каждого файла вызывает endIteration.
        /// </summary>
        /// <param name="srcDir"></param>
        /// <param name="dstDir"></param>
        /// <param name="endIteration"></param>
        /// <returns></returns>
        public bool CryptlFiles(string srcDir, string dstDir, Action<string> endIteration)
        {
            return ProcessFiles(srcDir, dstDir, (src, dst) =>
            {
                if(!_adapter.CryptlFile(src,dst)) return false;
                return true;
            },
            endIteration);
        }

        /// <summary>
        /// Дешифрирует файлы.
        /// </summary>
        /// <param name="srcDir"></param>
        /// <param name="dstDir"></param>
        /// <param name="endIteration"></param>
        /// <returns></returns>
        public bool DecryptFiles(string srcDir, string dstDir, Action<string> endIteration)
        {
            return ProcessFiles(srcDir, dstDir, (src, dst) =>
                {
                    if (!_adapter.DecryptlFile(src, dst, _password)) return false;
                    return true;
                },
                endIteration);
        }
    }
}
