namespace CryptoRoomLib.Sign
{
    /// <summary>
    /// Класс для создания ЭЦП ГОСТ 34.11-2012.
    /// </summary>
    internal class CreateSign
    {
        /// <summary>
        /// Подписывает сообщение message, используя закрытый ключ d и точку P.
        /// </summary>
        /// <returns></returns>
        public Signature Sign(BigInteger d, byte[] message, EcPoint p)
        {
            if (d == 0) throw new ArgumentException("Private key for sign can't be 0.");

            //Узнаю количество бит порядка эллиптической кривой.
            int qSize = p.Q.bitCount();

            var alpha = new BigInteger(CalculatedHash(message, qSize));
            var q = p.Q;
            var e = alpha % q;
            if (e == 0) e = 1;

            EcPoint c = new EcPoint();
            BigInteger r = new BigInteger();
            BigInteger s = new BigInteger();

            //Шаг 3 Генерирую псеводослучайное число k; 0<k<q
            var k = new BigInteger(CipherTools.GenerateRand(qSize / 8));

            do
            {
                c = PointMath.Multiply(k, p); //Шаг 4. Вычисляю точку эллиптической кривой C=k*G;
                r = c.X % q; //Определить r = x mod n, где x — x-координата точки C.
                s = ((r * d) + (k * e)) % q;
            } while ((r == 0) || (s == 0));

            //Проверка векторов на выполнение условий 0 < r < q , 0 < s < q.
            if (!((0 < r) && (r < q))) new ArgumentException("CreateSign::Sign bad vector 'r'");
            if (!((0 < s) && (s < q))) new ArgumentException("CreateSign::Sign bad vector 's'");

            return new Signature()
            {
                R = r,
                S = s,
                Qlength = qSize
            };
        }

        /// <summary>
        /// Проверяет подпись sign, сообщения message, используя откр. ключ Q. В случае успеха возвращает true.
        /// </summary>
        public bool Verify(byte[] message, Signature sign, EcPoint pointQ, EcPoint p)
        {
            BigInteger r = sign.R;
            BigInteger s = sign.S;

            var q = p.Q;

            if ((r < 1) || (r > (q - 1)) || (s < 1) || (s > (q - 1))) return false;

            //Иначе умножение перестанет работать, если векторы будут равны 0. Возникнет исключение. 
            if ((r == 0) || (s == 0)) return false;

            BigInteger alpha = new BigInteger(CalculatedHash(message, q.bitCount()));
            BigInteger e = alpha % q;
            if (e == 0) e = 1;

            BigInteger v = e.modInverse(q);
            BigInteger z1 = (s * v) % q;
            BigInteger z2 = p.Q + ((-(r * v)) % q);

            //Так как нет реализации умножения точки на нуль, делаем проверку.
            if (z1 <= 0) return false;
            if (z2 <= 0) return false;

            EcPoint a = PointMath.Multiply(z1, p);
            EcPoint b = PointMath.Multiply(z2, pointQ);
            EcPoint c = PointMath.Plus(a, b);

            BigInteger vectorR = c.X % q;

            if (vectorR == r) return true;
            
            return false;
        }

        /// <summary>
        /// Вычисляет хеш сообщения.
        /// </summary>
        /// <param name="message"></param>
        /// <param name="bitCount"></param>
        /// <returns></returns>
        private byte[] CalculatedHash(byte[] message, int bitCount)
        {
            Hash3411.Hash3411 hasher = new Hash3411.Hash3411();
            byte[] hashResult;

            if (bitCount < 500)
            {
                hashResult = new byte[32];
                hasher.Hash256(message, hashResult);
            }
            else
            {
                hashResult = new byte[64];
                hasher.Hash512(message, hashResult);
            }

            return hashResult;
        }
    }
}
