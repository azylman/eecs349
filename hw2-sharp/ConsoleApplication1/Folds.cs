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

        public Folds(String input)
        {
            typos = new SortedDictionary<String, String>();
            folds = new List<Fold>();
            StreamReader testingDataFile = new StreamReader(input);

            String data;
            while ((data = testingDataFile.ReadLine()) != null)
            {
                List<String> parts = new List<String>(data.Split('\t'));
                typos.Add(parts[0], parts[1]);
            }
        }

        public void makeN(int numFolds)
        {
            int foldSize = typos.Count/numFolds;

            for (int i = 0; i < foldSize; i++)
            {
                IDictionary<String, String> trainingSet = new Dictionary<String, String>();
                IDictionary<String, String> testingSet = new Dictionary<String, String>();

                int keyCount = 0;
                foreach (KeyValuePair<String, String> typo in typos)
                {
                    if (keyCount > foldSize * i && keyCount < (foldSize + 1) * i)
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

            writeFoldsToDisk();
        }

        private void writeFoldsToDisk()
        {
            foreach (Fold fold in folds)
            {
                
            }
        }
    }
}
