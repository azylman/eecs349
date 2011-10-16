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
            if (args.Length != 3)
            {
                Console.WriteLine("Error: expects three inputs (<file to check>, <dictionary>, <output path>)");
            }

	        // Create our file paths
            String dictPath = args[1];
            String testingPath = "wikipediatypoclean.txt";

	        Dict dict = new Dict(dictPath);
            Folds folds = new Folds(testingPath);

            Stopwatch timer = Stopwatch.StartNew();
            dict.hillClimber(folds.typos, true);
            long climbTime = timer.ElapsedMilliseconds;

            timer = Stopwatch.StartNew();
            double error = dict.measureError(folds.typos, true);
            long measureTime = timer.ElapsedMilliseconds;

            Console.WriteLine(error + " error rate in " + (climbTime + measureTime) + " ms (" + climbTime + " ms for climbing, " + measureTime + " ms for measuring.");
        }
    }
}
