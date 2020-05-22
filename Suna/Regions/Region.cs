using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna.Regions
{
   /// <summary>
   /// Represents a region in the code; this is generally just either a normal
   /// Suna region or a region marked as Javascript.
   /// </summary>
   class Region
   {
      #region Private Fields

      private StringBuilder contents = new StringBuilder();

      #endregion

      #region Public Methods

      public void AddLine(string line)
      {
         contents.AppendLine(line);
      }

      public TextReader OpenReader()
      {
         return new StringReader(contents.ToString());
      }

      #endregion

      #region Base Class Overrides

      public override string ToString()
      {
         return contents.ToString();
      }

      #endregion
   }
}
