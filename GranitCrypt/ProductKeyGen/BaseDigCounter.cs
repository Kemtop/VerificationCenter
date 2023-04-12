using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace KeyGen
{
    /// <summary>
    /// Счетчик базового числа. Используется только для тестирования.
    /// </summary>
    class BaseDigCounter
    {
        private static UInt32 baseDig=1;

        public BaseDigCounter()
        {
            baseDig = 1;
        }

        /// <summary>
        /// Возвращает базовое число. Инкриминирует его.
        /// </summary>
        /// <returns></returns>
       public UInt32 getBaseNum()
        {
            UInt32 d=baseDig;
            baseDig++;
            return d;
        }




    }
}
