﻿namespace CryptoRoomLib.AsymmetricCipher
{
    /// <summary>
    /// Типы блоков ассиметричной системы шифрования.
    /// </summary>
    internal enum AsBlockDataTypes
    {
        /// <summary>
        /// Блок содержащий шифрованный сеансовый ключ.
        /// </summary>
        CryptSessionKey = 115,

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
