using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Suna.Blocks;

namespace Suna.Link
{
   /// <summary>
   /// Linker class; it's job is to take a module and convert it to code.  Compilation
   /// is done as-needed, so we get called from the top level of the process and invoke
   /// compilation as necessary.
   /// </summary>
   class Linker
   {
      #region Types / Constants

      private enum Error
      {
         NoMain
      }

      #endregion

      #region Private Fields

      private LinkContext context;

      #endregion

      #region Public Methods

      /// <summary>
      /// Links the given module
      /// </summary>
      /// <param name="context"></param>
      /// <returns></returns>
      public LinkedModule Link(LinkContext context)
      {
         // save parameters
         this.context = context;

         // find the "main" block
         MainBlock main = context.SourceModule.Main;
         if (main == null)
            throw new CompileException(Error.NoMain);

         // let main compile itself in the global context
         main.Compile(context);

         // now what?
         throw new NotImplementedException();
      }

      #endregion
   }
}
