using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.CipherMode3413
{
    /// <summary>
    /// Реализация режима работы блочного шифра - режим простой замены с зацеплением,
    /// согласно ГОСТ 34.13-2015
    /// (Cipher Block Chaining, СВС)
    /// </summary>
    internal class ModeCBC
    {
        private readonly ICipherAlgoritm _algoritm;
        public ModeCBC(ICipherAlgoritm algoritm)
        {
            _algoritm = algoritm;
        }

        /// <summary>
        /// Итерация в режиме простой замены(ГОСТп5.4стр16)
        /// P-открытый текст,MSB-значением n разрядов регистра сдвига с большими номерами,
        /// C-результирующий шифртекст
        ///
        /// part 16 байт - Блок сообщения подлежащего шифрованию
        /// MSB 16 байт - значением n разрядов регистра сдвига с большими номерами
        /// C_block[16] - результирующий шифротекст
        /// </summary>
        internal void DeсryptIterationCBC(ref Block128t part, Block128t msb, ref Block128t cBlock)
        {
            _algoritm.DecryptBlock(ref cBlock); //Расшифровываю блок.
            XorBlocks(ref cBlock, ref msb, ref part); //Складываю  блоки.
        }

        /// <summary>
        /// Складывает блоки длиной 16 байт по модулю 2.
        /// </summary>
        /// <param name="block1"></param>
        /// <param name="block2"></param>
        /// <param name="result"></param>
        internal void XorBlocks(ref Block128t block1, ref Block128t block2, ref Block128t result)
        {
            result.Low = block1.Low ^ block2.Low;
            result.Hi = block1.Hi ^ block2.Hi;
        }
    }
}
