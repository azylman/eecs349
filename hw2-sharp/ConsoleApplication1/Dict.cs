using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Dict
    {
	    private ISet<String> fullDict;
        private ISet<String> reducedDict;
        double deletionCost;
        double insertionCost;
        double substitutionCost;

        private double calculateLevenshteinDistance(String s, String t, bool verbose)
        {
            if (verbose)
            {
                Console.WriteLine("Deletion: " + deletionCost + ", insertion: " + insertionCost + ", substitution: " + substitutionCost);
            }
            int m = s.Length;
	        int n = t.Length;

            double[,] d = new double[m + 1, n + 1];

	        for (int i = 0; i < m; ++i) {
                d[i, 0] = i;
	        }
	        for (int i = 0; i < n; ++i) {
                d[0, i] = i;
	        }

	        for (int j = 0; j < n; ++j) {
		        for (int i = 0; i < m; ++i) {
			        if (s[i] == t[j]) {
                        double cost = d[i, j];
				        d[i + 1, j + 1] = cost;
			        } else {
                        double cost = Math.Min(
								        Math.Min(
                                            d[i, j + 1] + deletionCost,
									        d[i + 1, j] + insertionCost),
								        d[i, j] + substitutionCost);
                        d[i + 1, j + 1] = cost;
			        }
		        }
	        }

	        return d[m - 1, n - 1];
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

        public String getCorrectWord(String word, bool useReducedDataSet)
        {
            return getCorrectWord(word, useReducedDataSet, false);
        }

        private String getCorrectWord(String word, bool useReducedDataSet, bool verbose)
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
                double cost = calculateLevenshteinDistance(word, alternateWord, verbose);

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
            int failure = 0;
            int total = 0;
            foreach (KeyValuePair<String, String> typo in typos)
            {
                if (!useReducedDataSet || typo.Key[0] == 'a')
                {
                    String result = getCorrectWord(typo.Key, useReducedDataSet, verbose);
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
            deletionCost = 1.0;
            insertionCost = 1.0;
            substitutionCost = 1.0;
        }

        public void hillClimber(IDictionary<String, String> typos, bool useReducedDataSet)
        {
            resetCosts();

            double delta = .1;

            double error = measureError(typos, useReducedDataSet);
            while (delta > .0001)
            {
                decrementVariable(ref deletionCost, delta, ref error, typos, useReducedDataSet);
                decrementVariable(ref insertionCost, delta, ref error, typos, useReducedDataSet);
                decrementVariable(ref substitutionCost, delta, ref error, typos, useReducedDataSet);

                delta /= 10.0;
            }
        }

        private void decrementVariable(ref double var, double delta, ref double error, IDictionary<String, String> typos, bool useReducedDataSet)
        {
            double newError = error;
            do
            {
                error = newError;
                var -= delta;
                newError = measureError(typos, useReducedDataSet, false);
            } while (newError < error);
            var += delta; // At this point, our variable is one delta lower than we want.
        }
    }
}
