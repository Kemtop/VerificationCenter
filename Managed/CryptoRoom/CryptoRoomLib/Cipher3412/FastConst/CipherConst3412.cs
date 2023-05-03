﻿namespace CryptoRoomLib.Cipher3412.FastConst
{
    /// <summary>
    /// Набор констант для быстрого алгоритма.
    /// </summary>
    internal static class CipherConst3412
    {
        internal static readonly ulong[] RoundConstantsLeft = new ulong[32]
        {
            0xb87a486c7276a26e,
            0xb3f490d8e4ec87dc,
            0x0b8ed8b4969a25b2,
            0xa52be3730b1bcd7b,
            0x1d51ab1f796d6f15,
            0x16df73abeff74aa7,
            0xaea53bc79d81e8c9,
            0x895605e6163659f6,
            0x312c4d8a6440fb98,
            0x3aa2953ef2dade2a,
            0x82d8dd5280ac7c44,
            0x2c7de6951d2d948d,
            0x9407aef96f5b36e3,
            0x9f89764df9c11351,
            0x27f33e218bb7b13f,
            0xd1ac0a0f2c6cb22f,
            0x69d642635e1a1041,
            0x62589ad7c88035f3,
            0xda22d2bbbaf6979d,
            0x7487e97c27777f54,
            0xccfda1105501dd3a,
            0xc77379a4c39bf888,
            0x7f0931c8b1ed5ae6,
            0x58fa0fe93a5aebd9,
            0xe0804785482c49b7,
            0xeb0e9f31deb66c05,
            0x5374d75dacc0ce6b,
            0xfdd1ec9a314126a2,
            0x45aba4f6433784cc,
            0x4e257c42d5ada17e,
            0xf65f342ea7db0310,
            0x619b141e58d8a75e
        };

        internal static readonly ulong[] RoundConstantsRight = new ulong[32]
        {
            0x019484dd10bd275d,
            0x02ebcb7920b94eba,
            0x037f4fa4300469e7,
            0x041555f240b19cb7,
            0x0581d12f500cbbea,
            0x06fe9e8b6008d20d,
            0x076a1a5670b5f550,
            0x082aaa2780a1fbad,
            0x09be2efa901cdcf0,
            0x0ac1615ea018b517,
            0x0b55e583b0a5924a,
            0x0c3fffd5c010671a,
            0x0dab7b08d0ad4047,
            0x0ed434ace0a929a0,
            0x0f40b071f0140efd,
            0x1054974ec3813599,
            0x11c01393d33c12c4,
            0x12bf5c37e3387b23,
            0x132bd8eaf3855c7e,
            0x1441c2bc8330a92e,
            0x15d54661938d8e73,
            0x16aa09c5a389e794,
            0x173e8d18b334c0c9,
            0x187e3d694320ce34,
            0x19eab9b4539de969,
            0x1a95f6106399808e,
            0x1b0172cd7324a7d3,
            0x1c6b689b03915283,
            0x1dffec46132c75de,
            0x1e80a3e223281c39,
            0x1f14273f33953b64,
            0x20a8ed9c45c16af1
        };

        internal static readonly byte[] roundConstants = new byte[512]
        {
            0x6e, 0xa2, 0x76, 0x72, 0x6c, 0x48, 0x7a, 0xb8, 0x5d, 0x27, 0xbd, 0x10, 0xdd, 0x84, 0x94, 0x01,
            0xdc, 0x87, 0xec, 0xe4, 0xd8, 0x90, 0xf4, 0xb3, 0xba, 0x4e, 0xb9, 0x20, 0x79, 0xcb, 0xeb, 0x02,
            0xb2, 0x25, 0x9a, 0x96, 0xb4, 0xd8, 0x8e, 0x0b, 0xe7, 0x69, 0x04, 0x30, 0xa4, 0x4f, 0x7f, 0x03,
            0x7b, 0xcd, 0x1b, 0x0b, 0x73, 0xe3, 0x2b, 0xa5, 0xb7, 0x9c, 0xb1, 0x40, 0xf2, 0x55, 0x15, 0x04,
            0x15, 0x6f, 0x6d, 0x79, 0x1f, 0xab, 0x51, 0x1d, 0xea, 0xbb, 0x0c, 0x50, 0x2f, 0xd1, 0x81, 0x05,
            0xa7, 0x4a, 0xf7, 0xef, 0xab, 0x73, 0xdf, 0x16, 0x0d, 0xd2, 0x08, 0x60, 0x8b, 0x9e, 0xfe, 0x06,
            0xc9, 0xe8, 0x81, 0x9d, 0xc7, 0x3b, 0xa5, 0xae, 0x50, 0xf5, 0xb5, 0x70, 0x56, 0x1a, 0x6a, 0x07,
            0xf6, 0x59, 0x36, 0x16, 0xe6, 0x05, 0x56, 0x89, 0xad, 0xfb, 0xa1, 0x80, 0x27, 0xaa, 0x2a, 0x08,
            0x98, 0xfb, 0x40, 0x64, 0x8a, 0x4d, 0x2c, 0x31, 0xf0, 0xdc, 0x1c, 0x90, 0xfa, 0x2e, 0xbe, 0x09,
            0x2a, 0xde, 0xda, 0xf2, 0x3e, 0x95, 0xa2, 0x3a, 0x17, 0xb5, 0x18, 0xa0, 0x5e, 0x61, 0xc1, 0x0a,
            0x44, 0x7c, 0xac, 0x80, 0x52, 0xdd, 0xd8, 0x82, 0x4a, 0x92, 0xa5, 0xb0, 0x83, 0xe5, 0x55, 0x0b,
            0x8d, 0x94, 0x2d, 0x1d, 0x95, 0xe6, 0x7d, 0x2c, 0x1a, 0x67, 0x10, 0xc0, 0xd5, 0xff, 0x3f, 0x0c,
            0xe3, 0x36, 0x5b, 0x6f, 0xf9, 0xae, 0x07, 0x94, 0x47, 0x40, 0xad, 0xd0, 0x08, 0x7b, 0xab, 0x0d,
            0x51, 0x13, 0xc1, 0xf9, 0x4d, 0x76, 0x89, 0x9f, 0xa0, 0x29, 0xa9, 0xe0, 0xac, 0x34, 0xd4, 0x0e,
            0x3f, 0xb1, 0xb7, 0x8b, 0x21, 0x3e, 0xf3, 0x27, 0xfd, 0x0e, 0x14, 0xf0, 0x71, 0xb0, 0x40, 0x0f,
            0x2f, 0xb2, 0x6c, 0x2c, 0x0f, 0x0a, 0xac, 0xd1, 0x99, 0x35, 0x81, 0xc3, 0x4e, 0x97, 0x54, 0x10,
            0x41, 0x10, 0x1a, 0x5e, 0x63, 0x42, 0xd6, 0x69, 0xc4, 0x12, 0x3c, 0xd3, 0x93, 0x13, 0xc0, 0x11,
            0xf3, 0x35, 0x80, 0xc8, 0xd7, 0x9a, 0x58, 0x62, 0x23, 0x7b, 0x38, 0xe3, 0x37, 0x5c, 0xbf, 0x12,
            0x9d, 0x97, 0xf6, 0xba, 0xbb, 0xd2, 0x22, 0xda, 0x7e, 0x5c, 0x85, 0xf3, 0xea, 0xd8, 0x2b, 0x13,
            0x54, 0x7f, 0x77, 0x27, 0x7c, 0xe9, 0x87, 0x74, 0x2e, 0xa9, 0x30, 0x83, 0xbc, 0xc2, 0x41, 0x14,
            0x3a, 0xdd, 0x01, 0x55, 0x10, 0xa1, 0xfd, 0xcc, 0x73, 0x8e, 0x8d, 0x93, 0x61, 0x46, 0xd5, 0x15,
            0x88, 0xf8, 0x9b, 0xc3, 0xa4, 0x79, 0x73, 0xc7, 0x94, 0xe7, 0x89, 0xa3, 0xc5, 0x09, 0xaa, 0x16,
            0xe6, 0x5a, 0xed, 0xb1, 0xc8, 0x31, 0x09, 0x7f, 0xc9, 0xc0, 0x34, 0xb3, 0x18, 0x8d, 0x3e, 0x17,
            0xd9, 0xeb, 0x5a, 0x3a, 0xe9, 0x0f, 0xfa, 0x58, 0x34, 0xce, 0x20, 0x43, 0x69, 0x3d, 0x7e, 0x18,
            0xb7, 0x49, 0x2c, 0x48, 0x85, 0x47, 0x80, 0xe0, 0x69, 0xe9, 0x9d, 0x53, 0xb4, 0xb9, 0xea, 0x19,
            0x05, 0x6c, 0xb6, 0xde, 0x31, 0x9f, 0x0e, 0xeb, 0x8e, 0x80, 0x99, 0x63, 0x10, 0xf6, 0x95, 0x1a,
            0x6b, 0xce, 0xc0, 0xac, 0x5d, 0xd7, 0x74, 0x53, 0xd3, 0xa7, 0x24, 0x73, 0xcd, 0x72, 0x01, 0x1b,
            0xa2, 0x26, 0x41, 0x31, 0x9a, 0xec, 0xd1, 0xfd, 0x83, 0x52, 0x91, 0x03, 0x9b, 0x68, 0x6b, 0x1c,
            0xcc, 0x84, 0x37, 0x43, 0xf6, 0xa4, 0xab, 0x45, 0xde, 0x75, 0x2c, 0x13, 0x46, 0xec, 0xff, 0x1d,
            0x7e, 0xa1, 0xad, 0xd5, 0x42, 0x7c, 0x25, 0x4e, 0x39, 0x1c, 0x28, 0x23, 0xe2, 0xa3, 0x80, 0x1e,
            0x10, 0x03, 0xdb, 0xa7, 0x2e, 0x34, 0x5f, 0xf6, 0x64, 0x3b, 0x95, 0x33, 0x3f, 0x27, 0x14, 0x1f,
            0x5e, 0xa7, 0xd8, 0x58, 0x1e, 0x14, 0x9b, 0x61, 0xf1, 0x6a, 0xc1, 0x45, 0x9c, 0xed, 0xa8, 0x20,
        };

        //Таблица экспонент (генератор = 0x02 в GF(256) по модулю g(x) = 111000011). 
        internal static readonly byte[] exponentialTable = new byte[256]
        {
            0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0xc3, 0x45, 0x8a, 0xd7, 0x6d, 0xda, 0x77, 0xee,
            0x1f, 0x3e, 0x7c, 0xf8, 0x33, 0x66, 0xcc, 0x5b, 0xb6, 0xaf, 0x9d, 0xf9, 0x31, 0x62, 0xc4, 0x4b,
            0x96, 0xef, 0x1d, 0x3a, 0x74, 0xe8, 0x13, 0x26, 0x4c, 0x98, 0xf3, 0x25, 0x4a, 0x94, 0xeb, 0x15,
            0x2a, 0x54, 0xa8, 0x93, 0xe5, 0x09, 0x12, 0x24, 0x48, 0x90, 0xe3, 0x05, 0x0a, 0x14, 0x28, 0x50,
            0xa0, 0x83, 0xc5, 0x49, 0x92, 0xe7, 0x0d, 0x1a, 0x34, 0x68, 0xd0, 0x63, 0xc6, 0x4f, 0x9e, 0xff,
            0x3d, 0x7a, 0xf4, 0x2b, 0x56, 0xac, 0x9b, 0xf5, 0x29, 0x52, 0xa4, 0x8b, 0xd5, 0x69, 0xd2, 0x67,
            0xce, 0x5f, 0xbe, 0xbf, 0xbd, 0xb9, 0xb1, 0xa1, 0x81, 0xc1, 0x41, 0x82, 0xc7, 0x4d, 0x9a, 0xf7,
            0x2d, 0x5a, 0xb4, 0xab, 0x95, 0xe9, 0x11, 0x22, 0x44, 0x88, 0xd3, 0x65, 0xca, 0x57, 0xae, 0x9f,
            0xfd, 0x39, 0x72, 0xe4, 0x0b, 0x16, 0x2c, 0x58, 0xb0, 0xa3, 0x85, 0xc9, 0x51, 0xa2, 0x87, 0xcd,
            0x59, 0xb2, 0xa7, 0x8d, 0xd9, 0x71, 0xe2, 0x07, 0x0e, 0x1c, 0x38, 0x70, 0xe0, 0x03, 0x06, 0x0c,
            0x18, 0x30, 0x60, 0xc0, 0x43, 0x86, 0xcf, 0x5d, 0xba, 0xb7, 0xad, 0x99, 0xf1, 0x21, 0x42, 0x84,
            0xcb, 0x55, 0xaa, 0x97, 0xed, 0x19, 0x32, 0x64, 0xc8, 0x53, 0xa6, 0x8f, 0xdd, 0x79, 0xf2, 0x27,
            0x4e, 0x9c, 0xfb, 0x35, 0x6a, 0xd4, 0x6b, 0xd6, 0x6f, 0xde, 0x7f, 0xfe, 0x3f, 0x7e, 0xfc, 0x3b,
            0x76, 0xec, 0x1b, 0x36, 0x6c, 0xd8, 0x73, 0xe6, 0x0f, 0x1e, 0x3c, 0x78, 0xf0, 0x23, 0x46, 0x8c,
            0xdb, 0x75, 0xea, 0x17, 0x2e, 0x5c, 0xb8, 0xb3, 0xa5, 0x89, 0xd1, 0x61, 0xc2, 0x47, 0x8e, 0xdf,
            0x7d, 0xfa, 0x37, 0x6e, 0xdc, 0x7b, 0xf6, 0x2f, 0x5e, 0xbc, 0xbb, 0xb5, 0xa9, 0x91, 0xe1, 0x01
        };


        //Таблица дискретных логарифмов (генератор = 0x02 в GF(256) по модулю g(x) = 111000011).  
        internal static readonly byte[] logarithmicTable = new byte[256]
        {
            0x00, 0x00, 0x01, 0x9d, 0x02, 0x3b, 0x9e, 0x97, 0x03, 0x35, 0x3c, 0x84, 0x9f, 0x46, 0x98, 0xd8,
            0x04, 0x76, 0x36, 0x26, 0x3d, 0x2f, 0x85, 0xe3, 0xa0, 0xb5, 0x47, 0xd2, 0x99, 0x22, 0xd9, 0x10,
            0x05, 0xad, 0x77, 0xdd, 0x37, 0x2b, 0x27, 0xbf, 0x3e, 0x58, 0x30, 0x53, 0x86, 0x70, 0xe4, 0xf7,
            0xa1, 0x1c, 0xb6, 0x14, 0x48, 0xc3, 0xd3, 0xf2, 0x9a, 0x81, 0x23, 0xcf, 0xda, 0x50, 0x11, 0xcc,
            0x06, 0x6a, 0xae, 0xa4, 0x78, 0x09, 0xde, 0xed, 0x38, 0x43, 0x2c, 0x1f, 0x28, 0x6d, 0xc0, 0x4d,
            0x3f, 0x8c, 0x59, 0xb9, 0x31, 0xb1, 0x54, 0x7d, 0x87, 0x90, 0x71, 0x17, 0xe5, 0xa7, 0xf8, 0x61,
            0xa2, 0xeb, 0x1d, 0x4b, 0xb7, 0x7b, 0x15, 0x5f, 0x49, 0x5d, 0xc4, 0xc6, 0xd4, 0x0c, 0xf3, 0xc8,
            0x9b, 0x95, 0x82, 0xd6, 0x24, 0xe1, 0xd0, 0x0e, 0xdb, 0xbd, 0x51, 0xf5, 0x12, 0xf0, 0xcd, 0xca,
            0x07, 0x68, 0x6b, 0x41, 0xaf, 0x8a, 0xa5, 0x8e, 0x79, 0xe9, 0x0a, 0x5b, 0xdf, 0x93, 0xee, 0xbb,
            0x39, 0xfd, 0x44, 0x33, 0x2d, 0x74, 0x20, 0xb3, 0x29, 0xab, 0x6e, 0x56, 0xc1, 0x1a, 0x4e, 0x7f,
            0x40, 0x67, 0x8d, 0x89, 0x5a, 0xe8, 0xba, 0x92, 0x32, 0xfc, 0xb2, 0x73, 0x55, 0xaa, 0x7e, 0x19,
            0x88, 0x66, 0x91, 0xe7, 0x72, 0xfb, 0x18, 0xa9, 0xe6, 0x65, 0xa8, 0xfa, 0xf9, 0x64, 0x62, 0x63,
            0xa3, 0x69, 0xec, 0x08, 0x1e, 0x42, 0x4c, 0x6c, 0xb8, 0x8b, 0x7c, 0xb0, 0x16, 0x8f, 0x60, 0xa6,
            0x4a, 0xea, 0x5e, 0x7a, 0xc5, 0x5c, 0xc7, 0x0b, 0xd5, 0x94, 0x0d, 0xe0, 0xf4, 0xbc, 0xc9, 0xef,
            0x9c, 0xfe, 0x96, 0x3a, 0x83, 0x34, 0xd7, 0x45, 0x25, 0x75, 0xe2, 0x2e, 0xd1, 0xb4, 0x0f, 0x21,
            0xdc, 0xac, 0xbe, 0x2a, 0x52, 0x57, 0xf6, 0x6f, 0x13, 0x1b, 0xf1, 0xc2, 0xce, 0x80, 0xcb, 0x4f
        };

        //Поле замены Пи для выполнения нелинейного биективного преобразования ГОСТ Р 34.12-2015 стр. 3. 
        internal static readonly byte[] Pi = new byte[256]
        {
            0xfc, 0xee, 0xdd, 0x11, 0xcf, 0x6e, 0x31, 0x16, 0xfb, 0xc4, 0xfa, 0xda, 0x23, 0xc5, 0x04, 0x4d,
            0xe9, 0x77, 0xf0, 0xdb, 0x93, 0x2e, 0x99, 0xba, 0x17, 0x36, 0xf1, 0xbb, 0x14, 0xcd, 0x5f, 0xc1,
            0xf9, 0x18, 0x65, 0x5a, 0xe2, 0x5c, 0xef, 0x21, 0x81, 0x1c, 0x3c, 0x42, 0x8b, 0x01, 0x8e, 0x4f,
            0x05, 0x84, 0x02, 0xae, 0xe3, 0x6a, 0x8f, 0xa0, 0x06, 0x0b, 0xed, 0x98, 0x7f, 0xd4, 0xd3, 0x1f,
            0xeb, 0x34, 0x2c, 0x51, 0xea, 0xc8, 0x48, 0xab, 0xf2, 0x2a, 0x68, 0xa2, 0xfd, 0x3a, 0xce, 0xcc,
            0xb5, 0x70, 0x0e, 0x56, 0x08, 0x0c, 0x76, 0x12, 0xbf, 0x72, 0x13, 0x47, 0x9c, 0xb7, 0x5d, 0x87,
            0x15, 0xa1, 0x96, 0x29, 0x10, 0x7b, 0x9a, 0xc7, 0xf3, 0x91, 0x78, 0x6f, 0x9d, 0x9e, 0xb2, 0xb1,
            0x32, 0x75, 0x19, 0x3d, 0xff, 0x35, 0x8a, 0x7e, 0x6d, 0x54, 0xc6, 0x80, 0xc3, 0xbd, 0x0d, 0x57,
            0xdf, 0xf5, 0x24, 0xa9, 0x3e, 0xa8, 0x43, 0xc9, 0xd7, 0x79, 0xd6, 0xf6, 0x7c, 0x22, 0xb9, 0x03,
            0xe0, 0x0f, 0xec, 0xde, 0x7a, 0x94, 0xb0, 0xbc, 0xdc, 0xe8, 0x28, 0x50, 0x4e, 0x33, 0x0a, 0x4a,
            0xa7, 0x97, 0x60, 0x73, 0x1e, 0x00, 0x62, 0x44, 0x1a, 0xb8, 0x38, 0x82, 0x64, 0x9f, 0x26, 0x41,
            0xad, 0x45, 0x46, 0x92, 0x27, 0x5e, 0x55, 0x2f, 0x8c, 0xa3, 0xa5, 0x7d, 0x69, 0xd5, 0x95, 0x3b,
            0x07, 0x58, 0xb3, 0x40, 0x86, 0xac, 0x1d, 0xf7, 0x30, 0x37, 0x6b, 0xe4, 0x88, 0xd9, 0xe7, 0x89,
            0xe1, 0x1b, 0x83, 0x49, 0x4c, 0x3f, 0xf8, 0xfe, 0x8d, 0x53, 0xaa, 0x90, 0xca, 0xd8, 0x85, 0x61,
            0x20, 0x71, 0x67, 0xa4, 0x2d, 0x2b, 0x09, 0x5b, 0xcb, 0x9b, 0x25, 0xd0, 0xbe, 0xe5, 0x6c, 0x52,
            0x59, 0xa6, 0x74, 0xd2, 0xe6, 0xf4, 0xb4, 0xc0, 0xd1, 0x66, 0xaf, 0xc2, 0x39, 0x4b, 0x63, 0xb6,
        };

        //Инверсия поля Пи. 
        internal static readonly byte[] InversedPi = new byte[256]
        {
            0xa5, 0x2d, 0x32, 0x8f, 0x0e, 0x30, 0x38, 0xc0, 0x54, 0xe6, 0x9e, 0x39, 0x55, 0x7e, 0x52, 0x91,
            0x64, 0x03, 0x57, 0x5a, 0x1c, 0x60, 0x07, 0x18, 0x21, 0x72, 0xa8, 0xd1, 0x29, 0xc6, 0xa4, 0x3f,
            0xe0, 0x27, 0x8d, 0x0c, 0x82, 0xea, 0xae, 0xb4, 0x9a, 0x63, 0x49, 0xe5, 0x42, 0xe4, 0x15, 0xb7,
            0xc8, 0x06, 0x70, 0x9d, 0x41, 0x75, 0x19, 0xc9, 0xaa, 0xfc, 0x4d, 0xbf, 0x2a, 0x73, 0x84, 0xd5,
            0xc3, 0xaf, 0x2b, 0x86, 0xa7, 0xb1, 0xb2, 0x5b, 0x46, 0xd3, 0x9f, 0xfd, 0xd4, 0x0f, 0x9c, 0x2f,
            0x9b, 0x43, 0xef, 0xd9, 0x79, 0xb6, 0x53, 0x7f, 0xc1, 0xf0, 0x23, 0xe7, 0x25, 0x5e, 0xb5, 0x1e,
            0xa2, 0xdf, 0xa6, 0xfe, 0xac, 0x22, 0xf9, 0xe2, 0x4a, 0xbc, 0x35, 0xca, 0xee, 0x78, 0x05, 0x6b,
            0x51, 0xe1, 0x59, 0xa3, 0xf2, 0x71, 0x56, 0x11, 0x6a, 0x89, 0x94, 0x65, 0x8c, 0xbb, 0x77, 0x3c,
            0x7b, 0x28, 0xab, 0xd2, 0x31, 0xde, 0xc4, 0x5f, 0xcc, 0xcf, 0x76, 0x2c, 0xb8, 0xd8, 0x2e, 0x36,
            0xdb, 0x69, 0xb3, 0x14, 0x95, 0xbe, 0x62, 0xa1, 0x3b, 0x16, 0x66, 0xe9, 0x5c, 0x6c, 0x6d, 0xad,
            0x37, 0x61, 0x4b, 0xb9, 0xe3, 0xba, 0xf1, 0xa0, 0x85, 0x83, 0xda, 0x47, 0xc5, 0xb0, 0x33, 0xfa,
            0x96, 0x6f, 0x6e, 0xc2, 0xf6, 0x50, 0xff, 0x5d, 0xa9, 0x8e, 0x17, 0x1b, 0x97, 0x7d, 0xec, 0x58,
            0xf7, 0x1f, 0xfb, 0x7c, 0x09, 0x0d, 0x7a, 0x67, 0x45, 0x87, 0xdc, 0xe8, 0x4f, 0x1d, 0x4e, 0x04,
            0xeb, 0xf8, 0xf3, 0x3e, 0x3d, 0xbd, 0x8a, 0x88, 0xdd, 0xcd, 0x0b, 0x13, 0x98, 0x02, 0x93, 0x80,
            0x90, 0xd0, 0x24, 0x34, 0xcb, 0xed, 0xf4, 0xce, 0x99, 0x10, 0x44, 0x40, 0x92, 0x3a, 0x01, 0x26,
            0x12, 0x1a, 0x48, 0x68, 0xf5, 0x81, 0x8b, 0xc7, 0xd6, 0x20, 0x0a, 0x08, 0x00, 0x4c, 0xd7, 0x74,
        };

        // L матрица преобразования над полем Галуа GF(256). 
        internal static readonly byte[,] LTransformationMatrix = new byte[16, 16]
        {
            { 0xcf, 0x6e, 0xa2, 0x76, 0x72, 0x6c, 0x48, 0x7a, 0xb8, 0x5d, 0x27, 0xbd, 0x10, 0xdd, 0x84, 0x94, },
            { 0x98, 0x20, 0xc8, 0x33, 0xf2, 0x76, 0xd5, 0xe6, 0x49, 0xd4, 0x9f, 0x95, 0xe9, 0x99, 0x2d, 0x20, },
            { 0x74, 0xc6, 0x87, 0x10, 0x6b, 0xec, 0x62, 0x4e, 0x87, 0xb8, 0xbe, 0x5e, 0xd0, 0x75, 0x74, 0x85, },
            { 0xbf, 0xda, 0x70, 0x0c, 0xca, 0x0c, 0x17, 0x1a, 0x14, 0x2f, 0x68, 0x30, 0xd9, 0xca, 0x96, 0x10, },
            { 0x93, 0x90, 0x68, 0x1c, 0x20, 0xc5, 0x06, 0xbb, 0xcb, 0x8d, 0x1a, 0xe9, 0xf3, 0x97, 0x5d, 0xc2, },
            { 0x8e, 0x48, 0x43, 0x11, 0xeb, 0xbc, 0x2d, 0x2e, 0x8d, 0x12, 0x7c, 0x60, 0x94, 0x44, 0x77, 0xc0, },
            { 0xf2, 0x89, 0x1c, 0xd6, 0x02, 0xaf, 0xc4, 0xf1, 0xab, 0xee, 0xad, 0xbf, 0x3d, 0x5a, 0x6f, 0x01, },
            { 0xf3, 0x9c, 0x2b, 0x6a, 0xa4, 0x6e, 0xe7, 0xbe, 0x49, 0xf6, 0xc9, 0x10, 0xaf, 0xe0, 0xde, 0xfb, },
            { 0x0a, 0xc1, 0xa1, 0xa6, 0x8d, 0xa3, 0xd5, 0xd4, 0x09, 0x08, 0x84, 0xef, 0x7b, 0x30, 0x54, 0x01, },
            { 0xbf, 0x64, 0x63, 0xd7, 0xd4, 0xe1, 0xeb, 0xaf, 0x6c, 0x54, 0x2f, 0x39, 0xff, 0xa6, 0xb4, 0xc0, },
            { 0xf6, 0xb8, 0x30, 0xf6, 0xc4, 0x90, 0x99, 0x37, 0x2a, 0x0f, 0xeb, 0xec, 0x64, 0x31, 0x8d, 0xc2, },
            { 0xa9, 0x2d, 0x6b, 0x49, 0x01, 0x58, 0x78, 0xb1, 0x01, 0xf3, 0xfe, 0x91, 0x91, 0xd3, 0xd1, 0x10, },
            { 0xea, 0x86, 0x9f, 0x07, 0x65, 0x0e, 0x52, 0xd4, 0x60, 0x98, 0xc6, 0x7f, 0x52, 0xdf, 0x44, 0x85, },
            { 0x8e, 0x44, 0x30, 0x14, 0xdd, 0x02, 0xf5, 0x2a, 0x8e, 0xc8, 0x48, 0x48, 0xf8, 0x48, 0x3c, 0x20, },
            { 0x4d, 0xd0, 0xe3, 0xe8, 0x4c, 0xc3, 0x16, 0x6e, 0x4b, 0x7f, 0xa2, 0x89, 0x0d, 0x64, 0xa5, 0x94, },
            { 0x6e, 0xa2, 0x76, 0x72, 0x6c, 0x48, 0x7a, 0xb8, 0x5d, 0x27, 0xbd, 0x10, 0xdd, 0x84, 0x94, 0x01, },
        };

        //Обратная матрица преобразования L над  полем Галуа GF(256).
        internal static readonly byte[,] inversedLTransformationMatrix = new byte[16, 16]
        {
            { 0x01, 0x94, 0x84, 0xdd, 0x10, 0xbd, 0x27, 0x5d, 0xb8, 0x7a, 0x48, 0x6c, 0x72, 0x76, 0xa2, 0x6e, },
            { 0x94, 0xa5, 0x64, 0x0d, 0x89, 0xa2, 0x7f, 0x4b, 0x6e, 0x16, 0xc3, 0x4c, 0xe8, 0xe3, 0xd0, 0x4d, },
            { 0x20, 0x3c, 0x48, 0xf8, 0x48, 0x48, 0xc8, 0x8e, 0x2a, 0xf5, 0x02, 0xdd, 0x14, 0x30, 0x44, 0x8e, },
            { 0x85, 0x44, 0xdf, 0x52, 0x7f, 0xc6, 0x98, 0x60, 0xd4, 0x52, 0x0e, 0x65, 0x07, 0x9f, 0x86, 0xea, },
            { 0x10, 0xd1, 0xd3, 0x91, 0x91, 0xfe, 0xf3, 0x01, 0xb1, 0x78, 0x58, 0x01, 0x49, 0x6b, 0x2d, 0xa9, },
            { 0xc2, 0x8d, 0x31, 0x64, 0xec, 0xeb, 0x0f, 0x2a, 0x37, 0x99, 0x90, 0xc4, 0xf6, 0x30, 0xb8, 0xf6, },
            { 0xc0, 0xb4, 0xa6, 0xff, 0x39, 0x2f, 0x54, 0x6c, 0xaf, 0xeb, 0xe1, 0xd4, 0xd7, 0x63, 0x64, 0xbf, },
            { 0x01, 0x54, 0x30, 0x7b, 0xef, 0x84, 0x08, 0x09, 0xd4, 0xd5, 0xa3, 0x8d, 0xa6, 0xa1, 0xc1, 0x0a, },
            { 0xfb, 0xde, 0xe0, 0xaf, 0x10, 0xc9, 0xf6, 0x49, 0xbe, 0xe7, 0x6e, 0xa4, 0x6a, 0x2b, 0x9c, 0xf3, },
            { 0x01, 0x6f, 0x5a, 0x3d, 0xbf, 0xad, 0xee, 0xab, 0xf1, 0xc4, 0xaf, 0x02, 0xd6, 0x1c, 0x89, 0xf2, },
            { 0xc0, 0x77, 0x44, 0x94, 0x60, 0x7c, 0x12, 0x8d, 0x2e, 0x2d, 0xbc, 0xeb, 0x11, 0x43, 0x48, 0x8e, },
            { 0xc2, 0x5d, 0x97, 0xf3, 0xe9, 0x1a, 0x8d, 0xcb, 0xbb, 0x06, 0xc5, 0x20, 0x1c, 0x68, 0x90, 0x93, },
            { 0x10, 0x96, 0xca, 0xd9, 0x30, 0x68, 0x2f, 0x14, 0x1a, 0x17, 0x0c, 0xca, 0x0c, 0x70, 0xda, 0xbf, },
            { 0x85, 0x74, 0x75, 0xd0, 0x5e, 0xbe, 0xb8, 0x87, 0x4e, 0x62, 0xec, 0x6b, 0x10, 0x87, 0xc6, 0x74, },
            { 0x20, 0x2d, 0x99, 0xe9, 0x95, 0x9f, 0xd4, 0x49, 0xe6, 0xd5, 0x76, 0xf2, 0x33, 0xc8, 0x20, 0x98, },
            { 0x94, 0x84, 0xdd, 0x10, 0xbd, 0x27, 0x5d, 0xb8, 0x7a, 0x48, 0x6c, 0x72, 0x76, 0xa2, 0x6e, 0xcf, },
        };
    }
}