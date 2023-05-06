namespace CryptoRoomLib.AsymmetricCipher
{
    /// <summary>
    /// Типы блоков ассиметричной системы шифрования.
    /// </summary>
    internal enum AsBlockDataTypes
    {
        /// <summary>
        /// Вектор r подписи ГОСТ 34.10-2012.
        /// </summary>
        VectorR = 116,

        /// <summary>
        /// Вектор s подписи ГОСТ 34.10-2012.
        /// </summary>
        VectorS = 117,
        
        /// <summary>
        /// Уникальное значение открытого ключа подписанта. Находиться в БД открытых ключей.
        /// </summary>
        SignKeyIndex = 118
    }
}
