using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Fold
    {
        public IDictionary<String, String> trainingSet;
        public IDictionary<String, String> testingSet;
        public Fold(IDictionary<String, String> trainingSet, IDictionary<String, String> testingSet)
        {
            this.trainingSet = trainingSet;
            this.testingSet = testingSet;
        }
    }
}
