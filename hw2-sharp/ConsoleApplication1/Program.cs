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
            Folds folds = new Folds(testingPath);

            Stopwatch timer = Stopwatch.StartNew();
            double error = dict.measureError(folds.typos, true);
            long time = timer.ElapsedMilliseconds;

            Console.WriteLine(error + " error rate in " + time + " milliseconds.");
        }
    }
}
