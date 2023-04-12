using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestGranit
{
    public class DbTable
    {
        public string Name { get; set; }
        public string Query { get; set; }
        public List<Sign> Signs { get; set; }

        public override string ToString()
        {
            return Name;
        }
    }

    public class Sign
    {
        public string SignColName { get; set; }
        public string KeyColName { get; set; }
        public string UserColName { get; set; }
    }
}
