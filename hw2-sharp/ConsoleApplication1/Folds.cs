using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace ConsoleApplication1
{
    class Folds
    {
        public IDictionary<String, String> typos;
        public IList<Fold> folds;
        private String name;

        public Folds(String input)
        {
            typos = new SortedDictionary<String, String>();
            folds = new List<Fold>();
            name = Path.GetFileNameWithoutExtension(input);

            StreamReader testingDataFile = new StreamReader(input);

            String data;
            while ((data = testingDataFile.ReadLine()) != null)
            {
                List<String> parts = new List<String>(data.Split('\t'));
                // Ignore malformed data
                if (parts.Count > 1 && !typos.ContainsKey(parts[0]))
                {
                    typos.Add(parts[0], parts[1]);
                }
            }
        }

        public void makeN(int numFolds)
        {
            int foldSize = typos.Count / numFolds;

            createFolds(numFolds, foldSize);

            writeFoldsToDisk();
        }

        public void makeWithSizeN(int foldSize)
        {
            int numFolds = typos.Count / foldSize;

            createFolds(numFolds, foldSize);

            writeFoldsToDisk();
        }

        private void createFolds(int numFolds, int foldSize)
        {
            for (int i = 0; i < numFolds; i++)
            {
                IDictionary<String, String> trainingSet = new Dictionary<String, String>();
                IDictionary<String, String> testingSet = new Dictionary<String, String>();

                int keyCount = 0;
                foreach (KeyValuePair<String, String> typo in typos)
                {
                    if (keyCount >= foldSize * i && keyCount < foldSize * (i + 1))
                    {
                        trainingSet.Add(typo.Key, typo.Value);
                    }
                    else
                    {
                        testingSet.Add(typo.Key, typo.Value);
                    }
                    keyCount++;
                }

                folds.Add(new Fold(trainingSet, testingSet));
            }
        }

        private void writeFoldsToDisk()
        {
            int foldCount = 0;
            foreach (Fold fold in folds)
            {
                foldCount++;

                using (StreamWriter swTrain = new StreamWriter(name + foldCount + "train.txt"))
                {
                    foreach (KeyValuePair<String, String> trainingData in fold.trainingSet)
                    {
                        swTrain.WriteLine(trainingData.Key + "\t" + trainingData.Value);
                    }
                }

                using (StreamWriter swTest = new StreamWriter(name + foldCount + "test.txt"))
                {
                    foreach (KeyValuePair<String, String> testingData in fold.testingSet)
                    {
                        swTest.WriteLine(testingData.Key + "\t" + testingData.Value);
                    }
                }
            }
        }
    }
}
