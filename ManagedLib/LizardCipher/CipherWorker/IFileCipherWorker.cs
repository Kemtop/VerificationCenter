using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LizardEncryptLib.CipherWorker
{
    /// <summary>
    /// Механизм кодирования/декодирования файлов на физическом диске.
    /// </summary>
    public interface IFileCipherWorker
    {
        /// <summary>
        /// Шифрует файлы.
        /// </summary>
        /// <param name="pathsToFiles"></param>
        /// <param name="keyPath"></param>
        /// <param name="keyContainerPassword"></param>
        /// <returns></returns>
        bool CryptFiles(List<string> pathsToFiles, string keyPath, string keyContainerPassword);

        /// <summary>
        /// Выполняет операцию расшифровки выбранных файлов.
        /// </summary>
        /// <param name="pathsToFiles"></param>
        /// <param name="keyPath"></param>
        /// <param name="keyContainerPassword"></param>
        /// <returns></returns>
        bool DecryptFiles(List<string> pathsToFiles, string keyPath, string keyContainerPassword);
    }
}
