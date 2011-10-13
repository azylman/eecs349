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

            int success = 0;
            int total = 0;
            Stopwatch timer = Stopwatch.StartNew();
	        foreach (KeyValuePair<String, String> testData in testingData) {
		        String result = dict.getCorrectWord(testData.Key);
		        //Console.Write(testData.Key + " took " + time + " milliseconds to evaluate to " + result + " ");
		        if (result == testData.Value) {
			        //Console.WriteLine("which is CORRECT!");
                    success++;
		        } else {
			        //Console.WriteLine("but it should be " + testData.Value);
		        }
                total++;
	        }
            long time = timer.ElapsedMilliseconds;

            Console.WriteLine((double) success/(double) total + " success rate in " + time + " milliseconds.");
        }
    }
}
