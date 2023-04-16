namespace MonteCarloAnalyzer.DAL
{
    public interface IGenerateRandomService
    {
        /// <summary>
        /// Генерирует посоледовательность бит и сохраняет ее в файл.
        /// </summary>
        /// <param name="fileName"></param>
        /// <param name="byteLength"></param>
        /// <returns></returns>
        (bool res, string error) GenerateRandom(string fileName, int byteLength);
    }
}
