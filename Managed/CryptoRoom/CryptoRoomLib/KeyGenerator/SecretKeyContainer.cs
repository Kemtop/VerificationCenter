using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CryptoRoomLib.KeyGenerator
{
    /// <summary>
    /// Контейнер секретного ключа.
    /// Большинство полей требуются для поддержки стандарта.
    /// </summary>
    internal class SecretKeyContainer
    {
        /// <summary>
        /// Версия  ключа
        /// </summary>
        public string KeyVersion { get; set; }

        /// <summary>
        /// Версия программы генератора ключа
        /// </summary>
        public string KeyGenVersion { get; set; }

        /// <summary>
        /// Наименование организации.
        /// </summary>
        public string OrgName { get; set; }

        /// <summary>
        /// Код организации.
        /// </summary>
        public string OrgCode { get; set; }

        /// <summary>
        /// Подразделение.
        /// </summary>
        public string Department { get; set; }

        /// <summary>
        /// Телефон.
        /// </summary>
        public string PhoneNumber { get; set; }

        /// <summary>
        /// Фамилия лица создавшего ключ.
        /// </summary>
        public string Familia { get; set; }

        /// <summary>
        /// Имя лица создавшего ключ.
        /// </summary>
        public string Imia { get; set; }

        /// <summary>
        /// Отчество лица создавшего ключ.
        /// </summary>
        public string Otchestvo { get; set; }

        /// <summary>
        /// Дата и время создания ключа.
        /// </summary>
        public DateTime DateBegin { get; set; }

        /// <summary>
        /// Дата и время окончания действия ключа.
        /// </summary>
        public DateTime DateEnd { get; set; }

        /// <summary>
        /// Шифрованный секретный ключ подписи файлов.
        /// </summary>
        public string CryptSignKey { get; set; }

        /// <summary>
        ///  Соль для декодирование ключа подписи.
        /// </summary>
        public string CryptSignSalt { get; set; }

        /// <summary>
        /// Вектор инициализации IV
        /// </summary>
        public string IV { get; set; }
    }
}
