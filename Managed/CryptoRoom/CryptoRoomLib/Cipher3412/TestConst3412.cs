namespace CryptoRoomLib.Cipher3412
{
    /// <summary>
    /// Набор тестовых констант для тестирования алгоритма ГОСТ 34.12.
    /// </summary>
    internal static class TestConst3412
    {
        /// <summary>
        /// Набор тестовых ключей шифрования. Размер ключа-32байта.
        /// </summary>
        public static readonly byte[,] Key = 
        {
            {
                0xe0, 0x9b, 0x75, 0x37, 0x3d, 0xe9, 0xb7, 0xfa,
                0x81, 0x7c, 0x93, 0x4d, 0x0f, 0x5d, 0x63, 0x80,
                0x80, 0xb3, 0x77, 0x43, 0xe3, 0xc1, 0xb0, 0x9c,
                0x29, 0xb4, 0x31, 0xe5, 0x7d, 0x8a, 0x8e, 0x60
            },
            {
                0x2d, 0xf0, 0xb7, 0x6f, 0xae, 0x29, 0x87, 0xf7,
                0x25, 0xc7, 0x72, 0x70, 0x09, 0xf1, 0x1f, 0x57,
                0x4a, 0x96, 0x8d, 0xfc, 0x78, 0x44, 0xe2, 0x1f,
                0xca, 0xc1, 0xf3, 0xd9, 0xfb, 0x2c, 0xaa, 0x33
            },
            {
                0xee, 0xa0, 0x09, 0xcc, 0x52, 0x38, 0x01, 0x35,
                0x37, 0x59, 0x74, 0xb3, 0x0f, 0x6a, 0x81, 0xc4,
                0xe2, 0x8b, 0x9f, 0x57, 0x8e, 0xba, 0x0a, 0xcc,
                0x43, 0x78, 0x64, 0x57, 0x00, 0xc3, 0x12, 0xe3,
            }
        };

        /// <summary>
        /// Результат развертывания раундового ключа для Key.
        /// </summary>
        public static readonly ulong[,] KeyDeploymentResult =
        {
            {
                0xfab7e93d37759be0,
                0x80635d0f4d937c81,
                0x9cb0c1e34377b380,
                0x608e8a7de531b429,
                0x0982da6c39086c07,
                0x9b6c842caf4a6746,
                0xec97e3f0e4539c01,
                0x7008256d9e0762db,
                0xf074032b6afa0108,
                0x8b2445afc4bfd62b,
                0xe72bb3154528f968,
                0x8bd58b62fbbea0fe,
                0x08042b93a2b8e1e2,
                0x49e6902e3404bfc8,
                0x065593b674a14f23,
                0xd952753831d58064,
                0xee2c918874a10241,
                0x02517cb717cd2800,
                0x5040f0da343975e7,
                0x3818bcfdfceae51e,
            },
            {
                0xf78729ae6fb7f02d,
                0x571ff1097072c725,
                0x1fe24478fc8d964a,
                0x33aa2cfbd9f3c1ca,
                0x7b72c8e78b8a1a2f,
                0x7300731bba10239f,
                0x8e6daa296e1a4886,
                0xd113950057b5b15f,
                0x59b9a01bcc7277d6,
                0x9c7c0bd118c4647c,
                0xcee1256048ea2a34,
                0x25daf9b0bb2cc58a,
                0xfd295c28f31e789f,
                0x89633ed5fe2b6db0,
                0x59bf56d4fff95378,
                0xff500516547371b0,
                0x5b2231da05acc8c7,
                0x6ccb99e01eed11f9,
                0xd7d3e7278f7a4a3b,
                0xb398d6a19bbc495e
            },
            {
                0x35013852cc09a0ee,
                0xc4816a0fb3745937,
                0xcc0aba8e579f8be2,
                0xe312c30057647843,
                0x08799c7e5e06dab0,
                0x6ca984e02ffa24f0,
                0xb3e75865c1524cf2,
                0xb44dfc83b7752724,
                0xdf511d69a9cff8de,
                0xc14325c48202eb34,
                0x3551c20214881310,
                0xb9b91bc555431a43,
                0x3c24162f6e6f1ea2,
                0x4c0260194eb053e5,
                0xe23303c2cf6129ea,
                0xdf589e9e10fc46ae,
                0x2003637e4e0de265,
                0xe838be02c5fdc037,
                0xbe74bf3f34750916,
                0xf71003d96e56e510
            }
        };
    }
}
