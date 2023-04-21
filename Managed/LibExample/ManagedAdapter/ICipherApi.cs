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
        /// Сообщение об ошибке.
        /// </summary>
        string LastError { get; }

        /// <summary>
        /// Шифрует файл.
        /// </summary>
        /// <param name="srcPathToFile"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        bool CryptlFile(string srcPathToFile, string dstDir);

        /// <summary>
        /// Декодирую файл.
        /// </summary>
        /// <param name="srcPathToFile"></param>
        /// <param name="dstDir"></param>
        /// <returns></returns>
        bool DecCryptlFile(string srcPathToFile, string dstDir);

        /// <summary>
        /// Шифрует все файлы из каталога. После кодирования каждого файла вызывает endIteration.
        /// </summary>
        /// <param name="srcDir"></param>
        /// <param name="dstDir"></param>
        /// <param name="endIteration"></param>
        /// <returns></returns>
        bool CryptlFiles(string srcDir, string dstDir, Action<string> endIteration);

        /// <summary>
        /// Дешифрирует файлы.
        /// </summary>
        /// <param name="srcDir"></param>
        /// <param name="dstDir"></param>
        /// <param name="endIteration"></param>
        /// <returns></returns>
        bool DecryptFiles(string srcDir, string dstDir, Action<string> endIteration);
    }
}
