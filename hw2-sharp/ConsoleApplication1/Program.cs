using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            String path = "";
	        // Create our file paths
            String dictPath = path + "3esl.txt";
	        String testingPath = path + "wikipediatypoclean.txt";

	        Dict dict = new Dict(dictPath);
	        Dictionary<String, String> testingData = new Dictionary<String, String>();

	        StreamReader testingDataFile = new StreamReader(testingPath);

            String data;
	        while ((data = testingDataFile.ReadLine()) != null) {
		        List<String> parts = new List<String>(data.Split('\t'));
		        testingData.Add(parts[0], parts[1]);
	        }

            Stopwatch timer = Stopwatch.StartNew();
            double error = dict.measureError(testingData);
            long time = timer.ElapsedMilliseconds;

            Console.WriteLine(error + " error rate in " + time + " milliseconds.");
        }
    }
}
