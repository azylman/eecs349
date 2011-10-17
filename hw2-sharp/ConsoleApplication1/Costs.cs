using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ConsoleApplication1
{
    class Costs
    {
        private double _deletionCost;
        private double _insertionCost;
        private double _substitutionCost;

        public Costs(double deletionCost, double insertionCost, double substitutionCost)
        {
            _deletionCost = deletionCost;
            _insertionCost = insertionCost;
            _substitutionCost = substitutionCost;
        }

        public double deletionCost
        {
            get { return _deletionCost; }
        }

        public double insertionCost
        {
            get { return _insertionCost; }
        }

        public double substitutionCost
        {
            get { return _substitutionCost; }
        }

        public List<Costs> getChildren(double delta)
        {
            List<Costs> children = new List<Costs>();
            children.Add(new Costs(deletionCost, insertionCost, substitutionCost - delta));
            children.Add(new Costs(deletionCost, insertionCost - delta, substitutionCost));
            children.Add(new Costs(deletionCost, insertionCost - delta, substitutionCost - delta));
            children.Add(new Costs(deletionCost - delta, insertionCost, substitutionCost));
            children.Add(new Costs(deletionCost - delta, insertionCost, substitutionCost - delta));
            children.Add(new Costs(deletionCost - delta, insertionCost - delta, substitutionCost));
            children.Add(new Costs(deletionCost - delta, insertionCost - delta, substitutionCost - delta));
            
            return children;
        }

        public bool Equals(Costs other)
        {
            return deletionCost == other.deletionCost && insertionCost == other.insertionCost && substitutionCost == other.substitutionCost;
        }

        public String ToString()
        {
            return "Deletion cost: " + deletionCost + ", insertion cost: " + insertionCost + ", substitution cost: " + substitutionCost;
        }
    }
}
