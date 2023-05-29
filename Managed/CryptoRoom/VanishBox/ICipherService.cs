using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VanishBox
{
    public interface ICipherService
    {
        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        /// <summary>
        /// Проверяет пароль для ключа.
        /// </summary>
        /// <returns></returns>
        bool CheckPassword(string password, string pathToKey);
    }
}
