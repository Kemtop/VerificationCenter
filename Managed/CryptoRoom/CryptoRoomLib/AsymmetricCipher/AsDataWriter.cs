using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.AsymmetricCipher
{
    /// <summary>
    /// Создает данные ассиметричной системы шифрования.
    /// </summary>
    internal class AsDataWriter
    {
        /// <summary>
        /// Длина типа блока.
        /// </summary>
        private const int TypeBlockLen = 1;

        /// <summary>
        /// Количество байт занимаемых сведениями о длине данных.
        /// </summary>
        private const int LenInfoSize = 4;

        /// <summary>
        ///Блоки данных
        /// </summary>
        public List<AsBlockData> Blocks;

        public AsDataWriter()
        {
            Blocks = new List<AsBlockData>();
        }

        /// <summary>
        /// Формирует блок - хеш открытого ключа получателя.
        /// </summary>
        public void AddRsaHash(byte[] hash)
        {
            Blocks.Add(new AsBlockData()
            {
                Type = AsBlockDataTypes.RsaPublicKeyHash,
                Data = hash
            });
        }

        /// <summary>
        /// Формирует блок - шифрованный сеансовый ключ.
        /// </summary>
        public void AddCryptedBlockKey(byte[] key)
        {
            Blocks.Add(new AsBlockData()
            {
                Type = AsBlockDataTypes.CryptSessionKey,
                Data = key
            });
        }

        /// <summary>
        /// Создает блок с вектором подписи.
        /// </summary>
        public void SignR(byte[] data)
        {
            Blocks.Add(new AsBlockData()
            {
                Type = AsBlockDataTypes.VectorR,
                Data = data
            });
        }

        /// <summary>
        /// Создает блок с вектором подписи.
        /// </summary>
        public void SignS(byte[] data)
        {
            Blocks.Add(new AsBlockData()
            {
                Type = AsBlockDataTypes.VectorS,
                Data = data
            });
        }

        /// <summary>
        /// На основании блоков формирует бинарную последовательность похожую на ASN1.
        /// [тип блока  1 байт][длина данных 4 байта][данные]
        /// </summary>
        /// <returns></returns>
        public byte[] GetData()
        {
            int size = 0;
            Blocks.ForEach((x) =>
            {
                size += x.Data.Length + TypeBlockLen + LenInfoSize;
            });

            byte[] data = new byte[size];
            int pos = 0;

            Blocks.ForEach((x) =>
            {
                //Тип блока.
                data[pos] = (byte)x.Type;
                pos ++;

                //Добавляю длину блока данных.
                var dataLen = GetBlockLen(x.Data);
                if (dataLen.Length != TypeBlockLen)
                {
                    new ArgumentException("Bad block len result.");
                }
                Buffer.BlockCopy(dataLen, 0, data, pos, dataLen.Length);
                pos += dataLen.Length;

                //Данные.
                Buffer.BlockCopy(x.Data, 0, data, pos, x.Data.Length);
                pos += x.Data.Length;
            });

            return data;
        }

        /// <summary>
        /// Возвращает длину блока данных.
        /// </summary>
        /// <param name="block"></param>
        /// <returns></returns>
        private byte[] GetBlockLen(byte[] block)
        {
            int len = block.Length;
            return BitConverter.GetBytes(len);
        }
    }
}
