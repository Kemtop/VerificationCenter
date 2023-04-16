using LizardEncryptLib.LowLayerFunction;
using System.IO;

namespace MonteCarloAnalyzer.DAL
{
    public class GenerateRandomServiceImpl : IGenerateRandomService
    {
        /// <summary>
        /// Генерирует посоледовательность бит и сохраняет ее в файл.
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="byteLength"></param>
        /// <returns></returns>
        public (bool res, string error) GenerateRandom(string fileName, int byteLength)
        {
            NativeAdapter adapter = new NativeAdapter();

            const int seadSize = 32;
            int size = (byteLength / seadSize) * seadSize;
            byte [] buffer = new byte[size];
            int point = 0;

            while (point < size)
            {
                var rnd = adapter.Rand256(0);
                if (!rnd.result) return (false, adapter.LastError());
                rnd.data.CopyTo(buffer, point);
                point += seadSize;
            }

            File.WriteAllBytes(fileName, buffer);

            return (true, string.Empty);
        }
    }
}
