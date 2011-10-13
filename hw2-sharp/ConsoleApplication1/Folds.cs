﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Folds
    {
        SortedDictionary<String, String> typos;
        List<Dictionary<String, String>> trainingSets;
        List<Dictionary<String, String>> testingSets;

        public Folds(Dictionary<String, String> typos)
        {
            this.typos = new SortedDictionary<String, String>(typos);
            trainingSets = null;
            testingSets = null;
        }

        public void makeN(int numFolds)
        {
            int foldSize = typos.Count/numFolds;
            List<Dictionary<String, String>> trainingSets = new List<Dictionary<String, String>>();
            List<Dictionary<String, String>> testingSets = new List<Dictionary<String, String>>();
            for (int i = 0; i < foldSize; i++)
            {
                int keyCount = 0;
                Dictionary<String, String> trainingSet = new Dictionary<String, String>();
                Dictionary<String, String> testingSet = new Dictionary<String, String>();

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

                trainingSets.Add(trainingSet);
                testingSets.Add(trainingSet);

                this.trainingSets = trainingSets;
                this.testingSets = testingSets;
            }

            writeFoldsToDisk();
        }

        private void writeFoldsToDisk()
        {
            if (trainingSets == null || testingSets == null)
            {
                return;
            }

            // Write folds to disk
        }
    }
}
