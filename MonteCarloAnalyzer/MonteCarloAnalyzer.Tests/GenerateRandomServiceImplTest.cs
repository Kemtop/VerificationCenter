using MonteCarloAnalyzer.DAL;

namespace MonteCarloAnalyzer.Tests
{
    [TestFixture]
    public class GenerateRandomServiceImplTest
    {
        [SetUp]
        public void Setup()
        {
        }

        [Test]
        [TestCase(4000000)]
        public void Genarate(int byteLength)
        {
            GenerateRandomServiceImpl rnd = new GenerateRandomServiceImpl();
            var result = rnd.GenerateRandom("rnd.bin", byteLength);

            Assert.IsTrue(result.res, result.error);
        }
    }
}