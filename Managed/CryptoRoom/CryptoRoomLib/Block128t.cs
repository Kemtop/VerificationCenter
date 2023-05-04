using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib
{
    /// <summary>
    /// 128 битный тип данных.
    /// </summary>
    internal struct Block128t
    {
        /// <summary>
        /// Младший 64 битный блок.
        /// </summary>
        public ulong Low;

        /// <summary>
        /// Старший 64 битный блок.
        /// </summary>
        public ulong Hi;
    }
}
