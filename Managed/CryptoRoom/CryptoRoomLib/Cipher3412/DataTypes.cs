namespace CryptoRoomLib.Cipher3412
{
    /// <summary>
    /// 128 битный тип.
    /// </summary>
    struct U128t
    {
        /// <summary>
        /// Младший 64 битный блок.
        /// </summary>
        public ulong Low;

        /// <summary>
        /// Старший 64 битный блок.
        /// </summary>
        public ulong Hi;

        /// <summary>
        /// Возвращает байт из указанной позиции в 128битном(16байтном) числе.
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        public byte GetByte(byte index)
        {
            if (index < 8)
            {
                return GetByte(index, Low);
            }
            else
            {
                return GetByte((byte)(index - 8), Hi);
            }
        }

        /// <summary>
        /// Возвращает байт из 64 битного(8байтного) числа.
        /// </summary>
        /// <param name="index"></param>
        /// <param name="digit"></param>
        /// <returns></returns>
        private byte GetByte(byte index, ulong digit)
        {
            switch (index)
            {
                case 0: return (byte)(digit & 0xFF);
                case 1: return (byte)((digit & 0xFF00) >> 8);
                case 2: return (byte)((digit & 0xFF0000) >> 16);
                case 3: return (byte)((digit & 0xFF000000) >> 24);
                case 4: return (byte)((digit & 0xFF00000000) >> 32);
                case 5: return (byte)((digit & 0xFF0000000000) >> 40);
                case 6: return (byte)((digit & 0xFF000000000000) >> 48);
                case 7: return (byte)((digit & 0xFF00000000000000) >> 56);
                default: return 0;
            }
        }
    }
}