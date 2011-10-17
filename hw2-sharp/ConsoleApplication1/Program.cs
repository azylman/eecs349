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

	        Dict dict = new Dict(dictPath);

            // 5.a
            // -----------------------------------------
            fiveA(dict);

            // 5.b
            // -----------------------------------------
            fiveB(dict);
        }

        static void fiveA(Dict dict) {
            Folds folds;

            Console.WriteLine("Wikipedia typo clean:");
            folds = new Folds("wikipediatypoclean.txt");
            validateFirstTypos(dict, folds, 30);

            Console.WriteLine("Wikipedia typo:");
            folds = new Folds("wikipediatypo.txt");
            validateFirstTypos(dict, folds, 30);

            Console.WriteLine("Synthetic data:");
            folds = new Folds("syntheticdata.txt");
            validateFirstTypos(dict, folds, 30);
        }

        static void fiveB(Dict dict)
        {
            Folds folds = new Folds("wikipediatypoclean.txt");
            // Validate with hill climbing
            crossValidation(dict, folds, 30, true);

            // Validate without hill climbing
            crossValidation(dict, folds, 30, false);
        }

        static void validateFold(Dict dict, Fold fold, bool useClimbing)
        {
            long climbTime = 0;
            Stopwatch timer;
            dict.resetCosts();

            if (useClimbing)
            {
                timer = Stopwatch.StartNew();
                dict.hillClimber(fold.trainingSet, false);
                climbTime = timer.ElapsedMilliseconds;
            }

            timer = Stopwatch.StartNew();
            double error = dict.measureError(fold.testingSet, false);
            long measureTime = timer.ElapsedMilliseconds;

            Console.WriteLine(error + " error rate in " + (climbTime + measureTime) + " ms (" + climbTime + " ms for climbing, " + measureTime + " ms for measuring.");
            Console.WriteLine("Computed costs: " + dict.getCosts().ToString());
        }

        static void crossValidation(Dict dict, Folds folds, int numFolds, bool useClimbing)
        {
            folds.makeN(numFolds);

            int foldNum = 0;
            foreach (Fold fold in folds.folds)
            {
                foldNum++;
                Console.WriteLine("Running against fold " + foldNum);
                validateFold(dict, fold, useClimbing);
            }
        }

        static void validateFirstTypos(Dict dict, Folds folds, int numTypos)
        {
            folds.makeWithSizeN(numTypos);

            validateFold(dict, folds.folds[0], true);
        }
    }
}
