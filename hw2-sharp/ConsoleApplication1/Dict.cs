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
	    private IDictionary<String, String> correctWords;
        bool useReducedDataSet;
        double hillClimbedDeletionCost;
        double hillClimbedInsertionCost;
        double hillClimbedSubstitutionCost;

        private double calculateLevenshteinDistance(String s, String t, bool simplifiedCosts)
        {
            int m = s.Length;
	        int n = t.Length;

            double[,] d = new double[m + 1, n + 1];

            double deletionCost;
            double insertionCost;
            double substitutionCost;
            if (simplifiedCosts)
            {
                deletionCost = 1;
                insertionCost = 1;
                substitutionCost = 1;
            }
            else
            {
                deletionCost = hillClimbedDeletionCost;
                insertionCost = hillClimbedInsertionCost;
                substitutionCost = hillClimbedSubstitutionCost;
            }

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
	        correctWords = new Dictionary<String, String>();
            hillClimbedDeletionCost = 1;
            hillClimbedInsertionCost = 1;
            hillClimbedSubstitutionCost = 1;

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

        public String getCorrectWord(String word, bool useReducedDataSet, bool simplifiedCosts)
        {
            ISet<String> dict = useReducedDataSet ? reducedDict : fullDict;

            if (dict.Contains(word))
            {
		        return word;
	        }

	        if (correctWords.ContainsKey(word)) {
		        return correctWords[word];
	        }

	        // Initialize this to the largest possible distance
            double shortestDistance = double.MaxValue;
	        String bestWord = "";
            foreach (String alternateWord in dict)
            {
                double cost = calculateLevenshteinDistance(word, alternateWord, simplifiedCosts);

		        if (cost < shortestDistance) {
			        bestWord = alternateWord;
			        shortestDistance = cost;
		        }
	        }

            correctWords.Add(word, bestWord);
	        return bestWord;
        }

        public double measureError(IDictionary<String, String> typos, bool useReducedDataSet)
        {
            int failure = 0;
            int total = 0;
            foreach (KeyValuePair<String, String> typo in typos)
            {
                if (!useReducedDataSet || typo.Key[0] == 'a')
                {
                    String result = getCorrectWord(typo.Key, useReducedDataSet, true);
                    if (result != typo.Value)
                    {
                        failure++;
                    }
                    total++;
                }
            }

            return (double)failure / (double)total;
        }

        // Hill climbing measureError
        public double measureError2(IDictionary<String, String> typos, bool useReducedDataSet)
        {
            return 0.0;
        }
    }
}
