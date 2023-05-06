using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.AsymmetricCipher
{
    /// <summary>
    /// Хранит блоки данных, в которых передается иформация для ассиметричного шифрования.
    /// </summary>
    internal class AsBlockData
    {
        /// <summary>
        /// Тип блока.
        /// </summary>
        public AsBlockDataTypes Type { get; set; }

        /// <summary>
        /// Данные блока.
        /// </summary>
        public byte[] Data { get; set; }
    }
}
