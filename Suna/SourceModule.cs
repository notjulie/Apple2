using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Suna
{
   /// <summary>
   /// A source module.  Currently this is a single file, though it remains to be
   /// seen how I decide to structure things with multi-file builds.
   /// </summary>
   public class SourceModule
   {
      /// <summary>
      /// Initializes a new instance of class SourceModule
      /// </summary>
      /// <param name="text"></param>
      public SourceModule(string text)
      {
         this.ModuleText = text;
      }

      /// <summary>
      /// Gets the module's text
      /// </summary>
      public string ModuleText
      {
         get;
         private set;
      }

      internal TextReader OpenReader()
      {
         return new StringReader(ModuleText);
      }
   }
}
