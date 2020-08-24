using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;
using Suna.JS;
using Suna.Link;
using Suna.Regions;
using Suna.Tokens;

namespace Suna
{
   /// <summary>
   /// Compiler class
   /// </summary>
   public sealed class SunaCompiler
   {
      #region Public Methods

      /// <summary>
      /// Compiles the given module
      /// </summary>
      /// <param name="sourceModule"></param>
      [System.Diagnostics.CodeAnalysis.SuppressMessage("Performance", "CA1822:Mark members as static",
         Justification = "TODO: determine the right structure here")]
      public LinkedModule Compile(SourceModule sourceModule)
      {
         Contract.Requires(sourceModule != null);

         // package up our inputs to the linker
         LinkContext linkContext = new LinkContext();
         linkContext.AddModule(sourceModule);

         // load any imports that haven't already been loaded
         linkContext.LoadImports();

         // link
         Linker linker = new Linker();
         linker.Link(linkContext);

         // return the result
         return linkContext.LinkedModule;
      }

      #endregion
   }
}
