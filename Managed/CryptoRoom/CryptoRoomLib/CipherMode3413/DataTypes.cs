using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.CipherMode3413
{
    /// <summary>
    /// Регистр для операций гаммирования.
    /// </summary>
    struct Register256t
    {
        /// <summary>
        /// Младший блок(16байт).
        /// </summary>
        public Block128t LSB;

        /// <summary>
        /// Старший блок(16байт).
        /// </summary>
        public Block128t MSB;

        /// <summary>
        /// Копирует данные из массива.
        /// </summary>
        public void FromArray(ulong[] arr)
        {
           LSB.Low = arr[0];
           LSB.Hi = arr[1];

           MSB.Low = arr[2];
           MSB.Hi = arr[3];
        }
        
        /// <summary>
        /// Возвращает младший блок(16байт).
        /// </summary>
        /// <returns></returns>
        public Block128t GetLSB()
        {
            return LSB;
        }

        /// <summary>
        /// Сдвигаю регистр вправо на 16 байт.
        /// </summary>
        public void RightShift()
        {
            LSB = MSB;
        }
    }
}
