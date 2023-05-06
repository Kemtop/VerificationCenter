using CryptoRoomLib.AsymmetricCipher;
using static System.Reflection.Metadata.BlobBuilder;

namespace CryptoRoomLib
{
    /// <summary>
    /// Разметка кодируемых/декодируемых файлов.
    /// </summary>
    internal static class FileFormat
    {
        /*
         * Формат заголовка файла:
         * [7 байт версия][Заголовок 47 байт][ХЭШ ГОСТ 34.11 сообщения идущего за ним 64байт][Размер блока шифрованных данных 8байт]
	        7+47+64=118
        */

        /// <summary>
        /// Позиция в файле с которой начинается кодируемая информация.
        /// В начале идет определение размера блока данных.
        /// </summary>
        internal static readonly int BeginDataBlock = 118;

        /// <summary>
        /// ...[Размер блока шифрованных данных 8байт]
        /// Размер блока содержащего информацию о длине кодируемых данных(байт).
        /// </summary>
        internal static readonly int DataSizeInfo = 8;

        /// <summary>
        /// Размер блока содержащего начальный вектор(байт).
        /// </summary>
        internal static readonly int IvSize = 32;

        /// <summary>
        /// Размер блока содержащего контрольную сумму.
        /// </summary>
        internal static readonly int EndInfoSize = 118;

        /// <summary>
        /// Размер заголовка данных ассиметричной системы.
        /// Заголовок 5 байт [тип][длина]
        /// </summary>
        internal static readonly int AsymmetricHeadSize = 5;

        /// <summary>
        /// Считывает из файла размер блока шифрованных данных.
        /// </summary>
        /// <param name="inFile"></param>
        /// <returns></returns>
        internal static ulong ReadDataSize(FileStream inFile)
        {
            //Устанавливаю текущую позицию(считываемый файл) на начало определения размера блока данных
            inFile.Position = FileFormat.BeginDataBlock;

            //Считывание размера блока данных
            byte[] arrLen = new byte[DataSizeInfo];

            inFile.Read(arrLen, 0, DataSizeInfo);

            //Получаю размер всего блока данных.
           return BitConverter.ToUInt64(arrLen, 0);
        }

        /// <summary>
        /// Считывает начальный вектор из файла.
        /// </summary>
        internal static byte[] ReadIV(FileStream inFile, ulong dataLen)
        {
            //Вычисляю позицию в которой заканчиваются данные и начинается iv
            dataLen = dataLen + (ulong)(EndInfoSize + DataSizeInfo - IvSize);
            inFile.Position = (long)dataLen;

            byte[] iv = new byte[IvSize];
            inFile.Read(iv, 0, IvSize);

            return iv;
        }
    }
}
