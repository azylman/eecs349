using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Dict
    {
	    private HashSet<String> dict;
	    private Dictionary<String, String> correctWords;

        private int calculateLevenshteinDistance(String s, String t)
        {
            int m = s.Length;
	        int n = t.Length;

            int[,] d = new int[m + 1, n + 1];
	        int deletionCost = 1;
	        int insertionCost = 1;
	        int substitutionCost = 1;

	        for (int i = 0; i < m; ++i) {
                d[i, 0] = i;
	        }
	        for (int i = 0; i < n; ++i) {
                d[0, i] = i;
	        }

	        for (int j = 0; j < n; ++j) {
		        for (int i = 0; i < m; ++i) {
			        if (s[i] == t[j]) {
                        int cost = d[i, j];
				        d[i + 1, j + 1] = cost;
			        } else {
				        int cost = Math.Min(
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
            dict = new HashSet<String>();
	        correctWords = new Dictionary<String, String>();

	        StreamReader dictFile = new StreamReader(dictPath);

            String item;
	        while ((item = dictFile.ReadLine()) != null) {
		        dict.Add(item);
	        }
        }

        public String getCorrectWord(String word)
        {
	        if (dict.Contains(word)) {
		        return word;
	        }

	        if (correctWords.ContainsKey(word)) {
		        return correctWords[word];
	        }

	        // Initialize this to the largest possible distance
	        int shortestDistance = int.MaxValue;
	        String bestWord = "";
	        foreach (String alternateWord in dict) {
                int cost = calculateLevenshteinDistance(word, alternateWord);

		        if (cost < shortestDistance) {
			        bestWord = alternateWord;
			        shortestDistance = cost;
		        }
	        }

            correctWords.Add(word, bestWord);
	        return bestWord;
        }

        public double measureError(Dictionary<String, String> typos)
        {
            int failure = 0;
            int total = 0;
            foreach (KeyValuePair<String, String> typo in typos)
            {
                String result = getCorrectWord(typo.Key);
                //Console.Write(testData.Key + " took " + time + " milliseconds to evaluate to " + result + " ");
                if (result == typo.Value)
                {
                    //Console.WriteLine("which is CORRECT!");
                }
                else
                {
                    failure++;
                    //Console.WriteLine("but it should be " + testData.Value);
                }
                total++;
            }

            return (double)failure / (double)total;
        }
    }
}
