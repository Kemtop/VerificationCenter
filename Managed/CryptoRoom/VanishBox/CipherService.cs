using CryptoRoomLib.KeyGenerator;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace VanishBox
{
    public class CipherService : ICipherService
    {
        /// <summary>
        /// Сообщение об ошибке.
        /// </summary>
        public string LastError { get; set; }

        private readonly KeyService _keyService;

        public CipherService()
        {
            _keyService = new KeyService();
        }

        /// <summary>
        /// Проверяет пароль для ключа.
        /// </summary>
        /// <returns></returns>
        public bool CheckPassword(string password, string pathToKey)
        {
            
            var res = _keyService.LoadKey(pathToKey);
            if (!res)
            {
                LastError = _keyService.LastError;
                return false;
            }

            res = _keyService.CheckPassword(password);
            if (!res)
            {
                LastError = _keyService.LastError;
                return false;
            }

            return true;
        }
    }
}
