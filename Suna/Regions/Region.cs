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
   public class Region
   {
      #region Private Fields

      private StringBuilder contents = new StringBuilder();

      #endregion

      #region Public Methods

      /// <summary>
      /// Adds a line of text to the region
      /// </summary>
      /// <param name="line"></param>
      public void AddLine(string line)
      {
         contents.AppendLine(line);
      }

      /// <summary>
      /// Opens a reader based on the contents of the region
      /// </summary>
      /// <returns></returns>
      public TextReader OpenReader()
      {
         return new StringReader(contents.ToString());
      }

      #endregion

      #region Base Class Overrides

      /// <summary>
      /// Returns a string representation of the object
      /// </summary>
      /// <returns></returns>
      public override string ToString()
      {
         return contents.ToString();
      }

      #endregion
   }
}
