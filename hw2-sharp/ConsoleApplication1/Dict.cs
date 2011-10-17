using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Dict
    {
	    private ISet<String> fullDict;
        private ISet<String> reducedDict;
        Costs costs;

        private double levenshteinDistance(String s, String t, bool verbose, Costs costs)
        {
            int m = s.Length;
            int n = t.Length;

            double[,] d = new double[m + 1, n + 1];

            for (int i = 0; i < m + 1; ++i)
            {
                d[i, 0] = i;
            }
            for (int i = 0; i < n + 1; ++i)
            {
                d[0, i] = i;
            }

            for (int j = 0; j < n; ++j)
            {
                for (int i = 0; i < m; ++i)
                {
                    if (s[i] == t[j])
                    {
                        double cost = d[i, j];
                        d[i + 1, j + 1] = cost;
                    }
                    else
                    {
                        double cost = Math.Min(
                                        Math.Min(
                                            d[i, j + 1] + costs.deletionCost,
                                            d[i + 1, j] + costs.insertionCost),
                                        d[i, j] + costs.substitutionCost);
                        d[i + 1, j + 1] = cost;
                    }
                }
            }

            return d[m, n];
        }

        private double levenshteinDistance(String s, String t, bool verbose)
        {
            return levenshteinDistance(s, t, verbose, costs);
        }

        public Dict(String dictPath)
        {
            fullDict = new HashSet<String>();
            reducedDict = new HashSet<String>();
            resetCosts();

	        StreamReader dictFile = new StreamReader(dictPath);

            String item;
	        while ((item = dictFile.ReadLine()) != null) {
                fullDict.Add(item);
                if (item[0] == 'a')
                {
                    reducedDict.Add(item);
                }
	        }
        }

        public String findClosestWord(String word)
        {
            return findClosestWord(word, false);
        }

        private String findClosestWord(String word, bool useReducedDataSet)
        {
            return findClosestWord(word, costs, useReducedDataSet, false);
        }

        private String findClosestWord(String word, Costs costs, bool useReducedDataSet, bool verbose)
        {
            ISet<String> dict = useReducedDataSet ? reducedDict : fullDict;

            if (dict.Contains(word))
            {
                return word;
            }

            // Initialize this to the largest possible distance
            double shortestDistance = double.MaxValue;
            String bestWord = "";
            foreach (String alternateWord in dict)
            {
                
                double cost = levenshteinDistance(word, alternateWord, verbose, costs);
                if (cost < shortestDistance)
                {
                    bestWord = alternateWord;
                    shortestDistance = cost;
                }
            }

            return bestWord;
        }

        public double measureError(IDictionary<String, String> typos, bool useReducedDataSet)
        {
            return measureError(typos, useReducedDataSet, false);
        }

        private double measureError(IDictionary<String, String> typos, bool useReducedDataSet, bool verbose)
        {
            return measureError(costs, typos, useReducedDataSet, verbose);
        }

        private double measureError(Costs costs, IDictionary<String, String> typos, bool useReducedDataSet, bool verbose)
        {
            int failure = 0;
            int total = 0;
            foreach (KeyValuePair<String, String> typo in typos)
            {
                if (!useReducedDataSet || typo.Key[0] == 'a')
                {
                    String result = findClosestWord(typo.Key, costs, useReducedDataSet, verbose);
                    if (result != typo.Value)
                    {
                        failure++;
                    }
                    total++;
                }
            }

            return (double)failure / (double)total;
        }

        public void resetCosts()
        {
            costs = new Costs(1.0, 1.0, 1.0);
        }

        public void hillClimber(IDictionary<String, String> typos, bool useReducedDataSet)
        {
            resetCosts();

            int maxSteps = 30;
            double delta = .1;

            int steps = 0;
            double error = measureError(typos, useReducedDataSet);
            while (steps < maxSteps)
            {
                steps++;

                Console.Write("Step " + steps + " has an error of ");

                Stopwatch timer = Stopwatch.StartNew();
                Costs newCost = getNextChild(costs, delta, typos, useReducedDataSet);
                long stepTime = timer.ElapsedMilliseconds;
                costs = newCost;

                Console.WriteLine("and took " + stepTime + " ms");
            }
        }

        private Costs getNextChild(Costs parent, double delta, IDictionary<String, String> typos, bool useReducedDataSet)
        {
            double lowestError = measureError(typos, useReducedDataSet, false);
            Costs bestCost = parent;
            List<Costs> children = parent.getChildren(delta);

            Random rnd = new Random();

            double probOfUsingBestChild = .75;
            // Sometimes, use a random child instead of the best child
            if (rnd.NextDouble() > probOfUsingBestChild)
            {
                Costs child = children[rnd.Next(0, children.Count - 1)];

                Console.Write(measureError(child, typos, useReducedDataSet, false) + " ");
                return child;
            }

            foreach (Costs child in children)
            {
                double newError = measureError(child, typos, useReducedDataSet, false);
                if (newError < lowestError)
                {
                    lowestError = newError;
                    bestCost = child;
                }
            }

            Console.Write(lowestError + " ");
            return bestCost;
        }

        public Costs getCosts()
        {
            return costs;
        }
    }
}
