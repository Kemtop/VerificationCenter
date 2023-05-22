using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib
{
    /// <summary>
    /// Режим работы блочного шифра.
    /// </summary>
    public interface IBlockCipherMode
    {
        /// <summary>
        /// Декодирует файл.
        /// </summary>
        /// <param name="cryptfile"></param>
        /// <param name="outfile"></param>
        /// <param name="setMaxBlockCount">Возвращает количество обрабатываемых блоков в файле.</param>
        /// <param name="endIteration">Возвращает номер обработанного блока. Необходим для движения ProgressBar на форме UI.</param>
        /// <param name="setDataSize">Возвращает размер декодируемых данных.</param>
       bool DecryptData(string cryptfile, string outfile, Action<ulong> setDataSize,
            Action<ulong> setMaxBlockCount,
            Action<ulong> endIteration, Func<ulong, FileStream, byte[]> asReader);
    }
}
