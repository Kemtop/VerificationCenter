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
    public interface ICipherApi
    {
        /// <summary>
        /// Имя локального каталога хранения шифрованных файлов.
        /// </summary>
        string GetLocalStoreEncryptName();

        /// <summary>
        /// Имя локального каталога хранения шифрованных файлов.
        /// </summary>
        string GetLocalStoreDecryptName();

        /// <summary>
        ///  Шифрует файл.
        /// </summary>
        /// <param name="srcPath"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        bool CryptlFile(string srcPath);

        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        string LastError { get; }
    }
}
