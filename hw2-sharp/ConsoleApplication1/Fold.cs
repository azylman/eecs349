using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Fold
    {
        public Dictionary<String, String> trainingSet;
        public Dictionary<String, String> testingSet;
        public Fold(Dictionary<String, String> trainingSet, Dictionary<String, String> testingSet)
        {
            this.trainingSet = trainingSet;
            this.testingSet = testingSet;
        }
    }
}
