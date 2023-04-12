using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace TestGranit
{
    public class User
    {
        public string Id { get; set; }
        public string UserName { get; set; }
        public string KeyFile { get; set; }
        public string CertFile { get; set; }
        public string BasePath { get; set; }
        public string Password { get; set; }
        public int CountSigns { get; set; }

        public string CheckKeys()
        {
            if (!File.Exists(KeyFile)) return "Не найден закрытый ключ пользователя " + UserName;
            if (!File.Exists(CertFile)) return "Не найден сертификат пользователя " + UserName;
            return "";
        }

        public string TestKeys(GranitAPI api)
        {
            api.ClearLibr();
            string err = api.LoadSecretKey(KeyFile.Replace("\\", "/"));
            if (!string.IsNullOrEmpty(err)) return err;
            if (!api.ChkPassword(Password, ref err)) return err;
            if (!api.uCheckKeyCert(ref err, CertFile.Replace("\\", "/"))) return err;
            return "";
        }

        public bool SignData(GranitAPI api, string mess, ref string KeyStamp, ref string SignStr, ref string Err)
        {
            api.ClearLibr();
            Err = api.LoadSecretKey(KeyFile.Replace("\\", "/"));
            if (!api.ChkPassword(Password, ref Err)) return false;
            if (!api.OpenCertDb(Application.StartupPath + "\\plugins", BasePath, ref Err)) return false;
            if (!string.IsNullOrEmpty(Err)) return false;
            bool res = api.SignData(Encoding.UTF8.GetBytes(mess), ref KeyStamp, ref SignStr, ref Err);
            api.ClearLibr();
            return res;
        }
    }
}
